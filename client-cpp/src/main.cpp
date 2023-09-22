//
// Created by Peyton Seigo on 2023-09-14.
//

#include <iostream>
#include <map>
#include <set>
#include <thread>
#include <utility>

#include "functional/expected.h"
#include "functional/overloaded.h"

#include "data/currency.h"

#include "networking/types.h"
#include "networking/connection.h"
#include "networking/json_coder.h"
#include "networking/pub_sub.h"
#include "networking/crdt.h"
#include "networking/vector_clocks.h"

#include "message.h"

#include "store/store_category.h"
#include "store/store_item.h"

using std::cout, std::endl;
using std::string;

struct Node
{
    NodeId id;
    PubSub pub_sub;
    CRDT<std::string, StoreItem> store_items;
    vector_clocks::Timestamp timestamp;

    explicit Node(NodeId id) : id{std::move(id)} {}

    void register_nodes(const std::vector<NodeId>& node_ids)
    {
        vector_clocks::init_new_nodes(timestamp, node_ids);
    }

    /*
    void receive(const Message& message)
    {
        vector_clocks::on_receive(id, timestamp, message.timestamp);
    }
     */
};

std::string encode_message(const NodeId& source, const vector_clocks::Timestamp& timestamp, const std::string& payload)
{
    Message message;
    message.source = source;
    message.data = payload;
    message.timestamp = timestamp;
    const auto encoded_message = JsonCoder<Message>{}.encode(message);
    return encoded_message;
};

auto parse_pub_sub_payload(std::string payload) -> std::optional<std::pair<std::pair<NodeId, vector_clocks::Timestamp>, StoreItem>>
{
    auto expected_message = JsonCoder<Message>{}.decode(payload);
    if (!expected_message) {
        std::ostringstream oss;
        const auto error = expected_message.error();
        oss << "[error] failed to decode message; error: " << error << '\n';
        std::cerr << oss.str() << std::flush;
        return std::nullopt;
    }
    const Message message = expected_message.get();

    auto expected_store_item = JsonCoder<StoreItem>{}.decode(message.data);
    if (!expected_store_item) {
        std::ostringstream oss;
        const auto error = expected_store_item.error();
        oss << "[error] failed to decode message; error: " << error << '\n';
        std::cerr << oss.str() << std::flush;
        return std::nullopt;
    }
    const StoreItem store_item = expected_store_item.get();

    using std::make_pair;
    return make_pair(make_pair(message.source, message.timestamp), store_item);
};

StoreItem read_and_print_store_item(const Node& node, const StoreItemId& store_item_id, const JsonCoder<StoreItem>& coder)
{
    const auto expected_store_item = node.store_items.read(store_item_id);
    if (!expected_store_item.has_value()) {
        std::ostringstream oss;
        oss << "expected to read store item. store_item_id = '" << store_item_id << "'; node_id: '" << node.id << "'";
        throw std::runtime_error(oss.str());
    }
    const auto& store_item = expected_store_item.value();

    using std::to_string;
    const auto encoded_store_item = coder.encode(store_item);
    std::ostringstream oss;
    oss << "[info][" << node.id << "] current value of '" << store_item.id << "' is: `" << encoded_store_item << "`";

    std::cout << oss.str() << std::endl;

    return store_item;
}

void two_nodes()
{
    std::vector<NodeId> node_ids = {"node_1", "node_2"};

    auto node_1 = Node(node_ids.at(0));
    auto node_2 = Node(node_ids.at(1));

    node_1.register_nodes(node_ids);
    node_2.register_nodes(node_ids);

    // PubSub/Broadcast settings
    constexpr auto channel = "crdt";
    constexpr auto topic = "store_items";

    node_1.pub_sub.connect(node_2.pub_sub);
    node_2.pub_sub.connect(node_1.pub_sub);

    node_1.pub_sub.add_handler(channel, topic, [&](string payload) {
        auto expected_pair = parse_pub_sub_payload(payload);
        if (!expected_pair.has_value()) {
            return;
        }
        auto [source_timestamp_pair, store_item] = expected_pair.value();
        auto [source, timestamp] = source_timestamp_pair;

        vector_clocks::merge(node_1.timestamp, timestamp);
        if (source != node_1.id) {
            vector_clocks::increment(node_1.timestamp, node_1.id);
        }

        const auto id = store_item.id;
        node_1.store_items.add(node_1.timestamp, id, std::move(store_item));
    });
    node_2.pub_sub.add_handler(channel, topic, [&](string payload) {
        auto expected_pair = parse_pub_sub_payload(payload);
        if (!expected_pair.has_value()) {
            return;
        }
        auto [source_timestamp_pair, store_item] = expected_pair.value();
        auto [source, timestamp] = source_timestamp_pair;

        vector_clocks::merge(node_2.timestamp, timestamp);
        if (source != node_2.id) {
            vector_clocks::increment(node_2.timestamp, node_2.id);
        }

        const auto id = store_item.id;
        node_2.store_items.add(node_2.timestamp, id, std::move(store_item));
    });

    // Serialization
    const auto store_item_coder = JsonCoder<StoreItem>();

    // node 1 initializes a StoreItem
    const StoreItemId store_item_id = sample_data<StoreItem>().front().id;

    node_1.store_items.add(
            node_1.timestamp,
            store_item_id,
            sample_data<StoreItem>().front());

    read_and_print_store_item(node_1, store_item_id, store_item_coder);

    // node 1 broadcasts message 1
    {
        const auto maybe_store_item = node_1.store_items.read(store_item_id);
        if (!maybe_store_item) {
            throw std::runtime_error("expected to read store item after it was added");
        }
        const auto store_item = maybe_store_item.value();

        vector_clocks::before_send(node_1.id, node_1.timestamp);

        const auto encoded_store_item = store_item_coder.encode(store_item);
        const auto encoded_message = encode_message(node_1.id, node_1.timestamp, encoded_store_item);
        node_1.pub_sub.broadcast(channel, topic, encoded_message);
    }

    // node 2 makes a change
    {
        auto store_item = read_and_print_store_item(node_2, store_item_id, store_item_coder);
        store_item.price *= 2;

        vector_clocks::before_send(node_2.id, node_2.timestamp);

        const auto encoded_store_item = store_item_coder.encode(store_item);
        const auto encoded_message = encode_message(node_2.id, node_2.timestamp, encoded_store_item);
        node_2.pub_sub.broadcast(channel, topic, encoded_message);
    }

    read_and_print_store_item(node_2, store_item_id, store_item_coder);

    // node 1 makes a change
    {
        auto store_item = read_and_print_store_item(node_1, store_item_id, store_item_coder);
        store_item.name = "Nanofiber Cloth";
        store_item.price *= 2;

        vector_clocks::before_send(node_1.id, node_1.timestamp);

        const auto encoded_store_item = store_item_coder.encode(store_item);
        const auto encoded_message = encode_message(node_1.id, node_1.timestamp, encoded_store_item);
        node_1.pub_sub.broadcast(channel, topic, encoded_message);
    }

    read_and_print_store_item(node_1, store_item_id, store_item_coder);
    read_and_print_store_item(node_2, store_item_id, store_item_coder);
}

void three_nodes()
{
    std::vector<NodeId> node_ids = {"node_1", "node_2", "node_3"};

    auto node_1 = Node(node_ids.at(0));
    auto node_2 = Node(node_ids.at(1));
    auto node_3 = Node(node_ids.at(2));

    node_1.register_nodes(node_ids);
    node_2.register_nodes(node_ids);
    node_3.register_nodes(node_ids);

    // PubSub/Broadcast settings
    constexpr auto channel = "crdt";
    constexpr auto topic = "store_items";

    node_1.pub_sub.connect(node_2.pub_sub);
    node_1.pub_sub.connect(node_3.pub_sub);
    node_2.pub_sub.connect(node_1.pub_sub);
    node_2.pub_sub.connect(node_3.pub_sub);
    node_3.pub_sub.connect(node_1.pub_sub);
    node_3.pub_sub.connect(node_2.pub_sub);

    node_1.pub_sub.add_handler(channel, topic, [&](string payload) {
        auto expected_pair = parse_pub_sub_payload(payload);
        if (!expected_pair.has_value()) {
            return;
        }
        auto [source_timestamp_pair, store_item] = expected_pair.value();
        auto [source, timestamp] = source_timestamp_pair;

        vector_clocks::merge(node_1.timestamp, timestamp);
        if (source != node_1.id) {
            vector_clocks::increment(node_1.timestamp, node_1.id);
        }

        const auto id = store_item.id;
        node_1.store_items.add(node_1.timestamp, id, std::move(store_item));
    });
    node_2.pub_sub.add_handler(channel, topic, [&](string payload) {
        auto expected_pair = parse_pub_sub_payload(payload);
        if (!expected_pair.has_value()) {
            return;
        }
        auto [source_timestamp_pair, store_item] = expected_pair.value();
        auto [source, timestamp] = source_timestamp_pair;

        vector_clocks::merge(node_2.timestamp, timestamp);
        if (source != node_2.id) {
            vector_clocks::increment(node_2.timestamp, node_2.id);
        }

        const auto id = store_item.id;
        node_2.store_items.add(node_2.timestamp, id, std::move(store_item));
    });
    node_3.pub_sub.add_handler(channel, topic, [&](string payload) {
        auto expected_pair = parse_pub_sub_payload(payload);
        if (!expected_pair.has_value()) {
            return;
        }
        auto [source_timestamp_pair, store_item] = expected_pair.value();
        auto [source, timestamp] = source_timestamp_pair;

        vector_clocks::merge(node_3.timestamp, timestamp);
        if (source != node_3.id) {
            vector_clocks::increment(node_3.timestamp, node_3.id);
        }

        const auto id = store_item.id;
        node_3.store_items.add(node_3.timestamp, id, std::move(store_item));
    });

    // Serialization
    const auto store_item_coder = JsonCoder<StoreItem>();

    // node 1 initializes a StoreItem
    const StoreItemId store_item_id = sample_data<StoreItem>().front().id;

    node_1.store_items.add(
            node_1.timestamp,
            store_item_id,
            sample_data<StoreItem>().front());

    read_and_print_store_item(node_1, store_item_id, store_item_coder);

    // node 1 broadcasts message 1
    {
        const auto maybe_store_item = node_1.store_items.read(store_item_id);
        if (!maybe_store_item) {
            throw std::runtime_error("expected to read store item after it was added");
        }
        const auto store_item = maybe_store_item.value();

        vector_clocks::before_send(node_1.id, node_1.timestamp);

        const auto encoded_store_item = store_item_coder.encode(store_item);
        const auto encoded_message = encode_message(node_1.id, node_1.timestamp, encoded_store_item);
        node_1.pub_sub.broadcast(channel, topic, encoded_message);
    }

    // node 2 makes a change
    {
        auto store_item = read_and_print_store_item(node_2, store_item_id, store_item_coder);
        store_item.price *= 2;

        // (print node 3's value as well)
        read_and_print_store_item(node_3, store_item_id, store_item_coder);

        vector_clocks::before_send(node_2.id, node_2.timestamp);

        const auto encoded_store_item = store_item_coder.encode(store_item);
        const auto encoded_message = encode_message(node_2.id, node_2.timestamp, encoded_store_item);
        node_2.pub_sub.broadcast(channel, topic, encoded_message);
    }

    read_and_print_store_item(node_2, store_item_id, store_item_coder);

    // node 1 makes a change
    {
        auto store_item = read_and_print_store_item(node_1, store_item_id, store_item_coder);
        store_item.name = "Nanofiber Cloth";
        store_item.price *= 2;

        vector_clocks::before_send(node_1.id, node_1.timestamp);

        const auto encoded_store_item = store_item_coder.encode(store_item);
        const auto encoded_message = encode_message(node_1.id, node_1.timestamp, encoded_store_item);
        node_1.pub_sub.broadcast(channel, topic, encoded_message);
    }

    read_and_print_store_item(node_1, store_item_id, store_item_coder);
    read_and_print_store_item(node_2, store_item_id, store_item_coder);

    // node 3 makes a change
    {
        auto store_item = read_and_print_store_item(node_3, store_item_id, store_item_coder);
        store_item.name = "Supernanofiber Cloth";
        store_item.price *= 10;

        vector_clocks::before_send(node_3.id, node_3.timestamp);

        const auto encoded_store_item = store_item_coder.encode(store_item);
        const auto encoded_message = encode_message(node_3.id, node_3.timestamp, encoded_store_item);
        node_3.pub_sub.broadcast(channel, topic, encoded_message);
    }

    read_and_print_store_item(node_1, store_item_id, store_item_coder);
    read_and_print_store_item(node_2, store_item_id, store_item_coder);
    read_and_print_store_item(node_3, store_item_id, store_item_coder);
}

int main()
{
    //two_nodes();
    three_nodes();
}

void configure()
{
    const auto config = Connection::Configuration{"http://localhost:4000", 30};
    Connection connection{config};

    const auto print_status = [&](const auto& connection) {
        std::cout << "[info] connection status: " << connection << std::endl;
    };

    //print_status(connection);

    connection.connect();

    /*
    for ( size_t i = 0; i < 2; i++) {
        print_status(connection);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
     */
}
