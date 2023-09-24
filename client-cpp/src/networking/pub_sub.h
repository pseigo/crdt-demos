//
// Created by Peyton Seigo on 2023-09-15.
//

#ifndef CRDTDEMOS_PUB_SUB_H
#define CRDTDEMOS_PUB_SUB_H

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>


// TODO: Implement incoming message buffer
//   inspo: "After broadcast algorithm receives message from network, it may buffer/queue it before delivering to the application" (D.S. (2021), pg. 79/169)
// TODO: Implement backpressure

// TODO: Implement outgoing message buffer?

// Inspiration: https://hexdocs.pm/phoenix_pubsub/Phoenix.PubSub.html
class PubSub {
private:
public:
    using Callback = std::function<void(std::string)>;

    void add_handler(std::string channel, std::string topic, Callback callback)
    {
        _handlers.push_back({
            std::move(channel),
            std::move(topic),
            std::move(callback)
        });
    }

    template <typename T>
    void broadcast(const std::string_view channel, const std::string_view topic, T message)
    {
        using std::to_string;

        std::ostringstream out;
        out << "[info] Broadcasting to '" << channel << ":" << topic << "': ";

        if constexpr (std::is_same<T, std::string>()) {
            out << message;
        } else {
            out << to_string(message);
        }

        std::cout << out.str() << std::endl;

        // Send to self
        handle_in(channel, topic, message);

        // Send to others
        for (auto& pub_sub : _others) {
            pub_sub->handle_in(channel, topic, message);
        }
    }

    void handle_in(const std::string_view channel, const std::string_view topic, std::string raw_message)
    {
        for (auto& handler: _handlers) {
            if (handler.channel == channel && (handler.topic == topic || handler.topic == "*")) {
                if (handler.callback) {
                    handler.callback(std::move(raw_message));
                }
            }
        }
    }

    void connect(PubSub& other)
    {
        _others.push_back(&other);
    }

private:
    struct _Handler {
        std::string channel;
        std::string topic;
        Callback callback;
    };

    std::vector<_Handler> _handlers;
    std::vector<PubSub*> _others;
};

#endif //CRDTDEMOS_PUB_SUB_H
