//
// Created by Peyton Seigo on 2023-09-14.
//

// TODO: Implement Lamport timestamps
// TODO: Investigate Vector clock timestamps (*after* Lamport's is working)

// TODO: Watch lectures on reliable broadcast
// TODO: Implement reliable broadcast in Phoenix backend
// TODO: Connect to Phoenix backend
// TODO: Listen for broadcasts

#include <chrono>
#include <iostream>
#include <set>
#include <thread>

#include "functional/expected.h"
#include "functional/overloaded.h"

#include "data/currency.h"
#include "data/timestamp.h"

#include "networking/connection.h"
#include "networking/json_coder.h"
#include "networking/pub_sub.h"
#include "networking/crdt.h"

#include "store/store_category.h"
#include "store/store_item.h"

using std::cout, std::endl;
using std::string;

int main()
{
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

    ////////////////////////////////////

    using Key = std::string;
    using Value = StoreItem;

    constexpr auto node_name = "cpp_client_1";

    // Start PubSub
    PubSub pub_sub;
    //pub_sub.connect("http://localhost:4000");
    constexpr auto channel = "crdt";
    constexpr auto topic = "store_items";

    CRDT<StoreItemId, StoreItem> crdt;

    const auto coder = JsonCoder<StoreItem>();


    //const auto state = State<Key, Value>{now, "store_item_1", encoded_store_item};
    //const auto state = State<Key, Value>{now, store_item.id, store_item};

    pub_sub.add_handler(channel, topic, [&](string payload) {
        const auto coder = JsonCoder<StoreItem>();

        auto result = coder.decode(payload);
        if (!result) {
            std::ostringstream oss;
            oss << "[error] failed to decode payload received via PubSub";

            const auto error = result.error();
            if (!error.empty()) {
               oss << "; error: " << error;
            }

            oss << '\n';

            std::cerr << oss.str() << std::flush;
            return;
        }

        StoreItem& item = result.get();

        // TODO: Timestamps should be generated on send/message creation, not on receive.
        Timestamp now = Lamport::now(node_name);
        const auto id = item.id;
        crdt.on_receive(std::move(now), id, std::move(item));
    });

    const StoreItem store_item = sample_data<StoreItem>().front(); // make a copy

    {
        const auto encoded_store_item = coder.encode(store_item);

        pub_sub.broadcast<std::string>(channel, topic, encoded_store_item);

        {
            const auto maybe_item = crdt.read(store_item.id);
            if (maybe_item.has_value()) {
                const auto& item = maybe_item.value();

                using std::to_string;
                //JsonCoder<std::remove_cv<decltype(item)>> coder;
                JsonCoder<StoreItem> coder;
                const auto encoded_item = coder.encode(item);
                std::ostringstream oss;
                oss << "[info] current value of '" << item.id << "' is: `" << encoded_item << "`";

                cout << oss.str() << endl;
            }
        }
    }

    {
        StoreItem store_item_v2 = StoreItem(
                store_item.id,
                store_item.name,
                store_item.category,
                store_item.price * 2,
                store_item.currency);

        const auto encoded_store_item = coder.encode(store_item_v2);

        pub_sub.broadcast<std::string>(channel, topic, encoded_store_item);

        {
            const auto maybe_item = crdt.read(store_item.id);
            if (maybe_item.has_value()) {
                const auto& item = maybe_item.value();

                using std::to_string;
                //JsonCoder<std::remove_cv<decltype(item)>> coder;
                JsonCoder<StoreItem> coder;
                const auto encoded_item = coder.encode(item);
                std::ostringstream oss;
                oss << "[info] current value of '" << item.id << "' is: `" << encoded_item << "`";

                cout << oss.str() << endl;
            }
        }
    }

}
