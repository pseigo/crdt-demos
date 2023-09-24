//
// Created by Peyton Seigo on 2023-09-14.
//

#include <iostream>
#include <map>
#include <set>
#include <string_view>
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
};

// ==============[ Inter-node messages ]==============
std::string encode_message(const NodeId& source, const vector_clocks::Timestamp& timestamp, const std::string& payload)
{
    Message message;
    message.source = source;
    message.timestamp = timestamp;
    message.data = payload;
    const auto encoded_message = JsonCoder<Message>{}.encode(message);
    return encoded_message;
};

void broadcast_change(Node& source_node,
                      StoreItem store_item,
                      const JsonCoder<StoreItem>& coder,
                      const std::string_view channel,
                      const std::string_view topic)
{
    // ASSUME: Synchronous timing, reliable broadcast; it's fine to mutate the node's state, don't need to 'undo' if it can't fail
    vector_clocks::before_send(source_node.id, source_node.timestamp);

    const auto encoded_store_item = coder.encode(store_item);
    const auto encoded_message = encode_message(source_node.id, source_node.timestamp, encoded_store_item);
    source_node.pub_sub.broadcast(channel, topic, encoded_message);
}

struct PubSubMessage
{
    NodeId source;
    vector_clocks::Timestamp timestamp;
    StoreItem store_item;
};

std::optional<PubSubMessage> decode_pub_sub_message(std::string payload)
{
    // Decode raw payload
    auto expected_message = JsonCoder<Message>{}.decode(payload);
    if (!expected_message) {
        std::ostringstream oss;
        const auto error = expected_message.error();
        oss << "[error] failed to decode message; error: " << error << '\n';
        std::cerr << oss.str() << std::flush;
        return std::nullopt;
    }
    const Message message = expected_message.get();

    // Decode StoreItem from payload's data field
    auto expected_store_item = JsonCoder<StoreItem>{}.decode(message.data);
    if (!expected_store_item) {
        std::ostringstream oss;
        const auto error = expected_store_item.error();
        oss << "[error] failed to decode message; error: " << error << '\n';
        std::cerr << oss.str() << std::flush;
        return std::nullopt;
    }
    const StoreItem store_item = expected_store_item.get();

    return std::optional(PubSubMessage{
        std::move(message.source),
        std::move(message.timestamp),
        std::move(store_item)
    });
};

void process_message(Node& node, string raw_message)
{
    // Decode message
    auto expected_pub_sub_message = decode_pub_sub_message(raw_message);
    if (!expected_pub_sub_message.has_value()) {
        throw std::runtime_error("failed to decode PubSub message");
    }
    auto message = expected_pub_sub_message.value();

    // Update node's timestamp
    vector_clocks::merge(node.timestamp, message.timestamp);

    // Increment node's event count if the message came from another node
    if (message.source != node.id) {
        vector_clocks::increment(node.timestamp, node.id);
    }

    // Update node's StoreItem CRDT
    const auto id = message.store_item.id;
    node.store_items.add(node.timestamp, id, std::move(message.store_item));

    // Log success
    std::ostringstream oss;
    oss << "[info][" << node.id << "] processed message and updated StoreItem CRDT\n";
    std::cout << oss.str() << std::flush;
}
// ==============[ END: Inter-node messages ]==============

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

    node_1.pub_sub.add_handler(channel, topic, [&](string raw_message) { process_message(node_1, raw_message); });
    node_2.pub_sub.add_handler(channel, topic, [&](string raw_message) { process_message(node_2, raw_message); });

    // Serialization
    const auto store_item_coder = JsonCoder<StoreItem>();

    // node 1 initializes a StoreItem
    const StoreItemId store_item_id = sample_data<StoreItem>().front().id;

    node_1.store_items.add(
            node_1.timestamp,
            store_item_id,
            sample_data<StoreItem>().front());

    read_and_print_store_item(node_1, store_item_id, store_item_coder);


    { // node 1 broadcasts message 1
        const auto maybe_store_item = node_1.store_items.read(store_item_id);
        if (!maybe_store_item) {
            throw std::runtime_error("expected to read store item after it was added");
        }
        const auto store_item = maybe_store_item.value();

        broadcast_change(node_1, store_item, store_item_coder, channel, topic);
    }

    { // node 2 makes a change
        auto store_item = read_and_print_store_item(node_2, store_item_id, store_item_coder);
        store_item.price *= 2;

        broadcast_change(node_2, store_item, store_item_coder, channel, topic);
    }

    read_and_print_store_item(node_2, store_item_id, store_item_coder);

    { // node 1 makes a change
        auto store_item = read_and_print_store_item(node_1, store_item_id, store_item_coder);
        store_item.name = "Nanofiber Cloth";
        store_item.price *= 2;

        broadcast_change(node_1, store_item, store_item_coder, channel, topic);
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

    node_1.pub_sub.add_handler(channel, topic, [&](string raw_message) { process_message(node_1, raw_message); });
    node_2.pub_sub.add_handler(channel, topic, [&](string raw_message) { process_message(node_2, raw_message); });
    node_3.pub_sub.add_handler(channel, topic, [&](string raw_message) { process_message(node_3, raw_message); });

    // Serialization
    const auto store_item_coder = JsonCoder<StoreItem>();

    // node 1 initializes a StoreItem
    const StoreItemId store_item_id = sample_data<StoreItem>().front().id;

    node_1.store_items.add(
            node_1.timestamp,
            store_item_id,
            sample_data<StoreItem>().front());

    read_and_print_store_item(node_1, store_item_id, store_item_coder);

    { // node 1 broadcasts this StoreItem for the first time
        const auto maybe_store_item = node_1.store_items.read(store_item_id);
        if (!maybe_store_item) {
            throw std::runtime_error("expected to read store item after it was added");
        }
        const auto store_item = maybe_store_item.value();

        broadcast_change(node_1, store_item, store_item_coder, channel, topic);
    }

    read_and_print_store_item(node_3, store_item_id, store_item_coder);

    { // node 2 makes a change
        auto store_item = read_and_print_store_item(node_2, store_item_id, store_item_coder);
        store_item.price *= 2;

        broadcast_change(node_2, store_item, store_item_coder, channel, topic);
    }

    read_and_print_store_item(node_2, store_item_id, store_item_coder);

    // node 1 makes a change
    {
        auto store_item = read_and_print_store_item(node_1, store_item_id, store_item_coder);
        store_item.name = "Nanofiber Cloth";
        store_item.price *= 2;

        broadcast_change(node_1, store_item, store_item_coder, channel, topic);
    }

    read_and_print_store_item(node_1, store_item_id, store_item_coder);
    read_and_print_store_item(node_2, store_item_id, store_item_coder);

    // node 3 makes a change
    {
        auto store_item = read_and_print_store_item(node_3, store_item_id, store_item_coder);
        store_item.name = "Supernanofiber Cloth";
        store_item.price *= 10;

        broadcast_change(node_3, store_item, store_item_coder, channel, topic);
    }

    read_and_print_store_item(node_1, store_item_id, store_item_coder);
    read_and_print_store_item(node_2, store_item_id, store_item_coder);
    read_and_print_store_item(node_3, store_item_id, store_item_coder);
}

int main()
{
    std::cout << "=====================[ Demo: Two Nodes ]=====================\n\n" << std::flush;
    two_nodes();

    std::cout << "\n\n" << std::flush;

    std::cout << "=====================[ Demo: Three Nodes ]=====================\n\n" << std::flush;
    three_nodes();
}

void configure()
{
    const auto config = Connection::Configuration{"http://localhost:4000", 30};
    Connection connection{config};

    const auto print_status = [&](const auto& connection) {
        std::cout << "[info] connection status: " << connection << std::endl;
    };

    print_status(connection);

    connection.connect();

    /*
    for ( size_t i = 0; i < 2; i++) {
        print_status(connection);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
     */
}
