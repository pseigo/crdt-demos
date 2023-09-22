//
// Created by Peyton Seigo on 2023-09-15.
//

#ifndef CRDTDEMOS_PUB_SUB_H
#define CRDTDEMOS_PUB_SUB_H

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

//#include "crdt.h"

// Inspiration: https://hexdocs.pm/phoenix_pubsub/Phoenix.PubSub.html
class PubSub {
private:
    using string = std::string;

public:
    using Callback = std::function<void(string)>;

    void add_handler(string channel, string topic, Callback callback)
    {
        _handlers.push_back({ channel, topic, std::move(callback) });
    }

    template <typename T>
    void broadcast(string channel, string topic, T message)
    {
        using std::to_string;

        std::ostringstream out;
        out << "[info] Broadcasting to '" << channel << ":" << topic << "': ";

        if constexpr (std::is_same<T, string>()) {
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

    void handle_in(string channel, string topic, string payload)
    {
        for (auto& handler: _handlers) {
            if (handler.channel == channel && (handler.topic == topic || handler.topic == "*")) {
                if (handler.callback) {
                    handler.callback(payload);
                }
            }
        }
    }

    void connect(PubSub& other)
    {
        if (&other == nullptr) {
            throw std::invalid_argument("given PubSub is null");
        }
        _others.push_back(&other);
    }

private:
    struct _Handler {
        string channel;
        string topic;
        Callback callback;
    };

    std::vector<_Handler> _handlers;
    std::vector<PubSub*> _others;
};

#endif //CRDTDEMOS_PUB_SUB_H
