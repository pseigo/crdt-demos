//
// Created by Peyton Seigo on 2023-09-15.
//

#ifndef CRDTDEMOS_CONNECTION_H
#define CRDTDEMOS_CONNECTION_H

#include <chrono>
#include <ostream>
#include <string>
#include <variant>

#include "../functional/overloaded.h"

class Connection
{
public:
    struct Configuration
    {
        std::string host_url;
        size_t heartbeat_timeout;
    };

public:
    struct disconnected {};
    struct connected
    {
        connected() :
                connected_at{std::chrono::system_clock::now()},
                _monotonic_connected_at{std::chrono::steady_clock::now()}
        {}

        std::chrono::time_point<std::chrono::system_clock> connected_at;

        auto duration() const
        {
            return std::chrono::steady_clock::now() - _monotonic_connected_at;
        }

    private:
        std::chrono::time_point<std::chrono::steady_clock> _monotonic_connected_at;
    };

private:
    std::variant<disconnected, connected> _state;
    Configuration _config;

public:
    Connection(Configuration config) :
            _state{disconnected{}},
            _config{std::move(config)}
    {}

    // TODO: what are the memory semantics here?
    std::variant<disconnected, connected> state() const { return _state; }

    void connect()
    {
        _state = connected();
    }

    void disconnect()
    {
        _state = disconnected();
    }
};

std::ostream& operator<<(std::ostream& out, const Connection& connection)
{
    std::visit(
            overloaded {
                    [&](const Connection::disconnected& /* state */) {
                        out << "disconnected";
                    },
                    [&](const Connection::connected& state) {
                        using std::chrono::duration_cast, std::chrono::seconds;
                        const auto duration = state.duration();
                        out << "connected for " << duration_cast<seconds>(duration).count() << " seconds";
                    }
            },
            connection.state());
    return out;
}

#endif //CRDTDEMOS_CONNECTION_H
