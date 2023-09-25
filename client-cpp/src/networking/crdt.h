//
// Created by Peyton Seigo on 2023-09-14.
//

#ifndef CRDTDEMOS_CRDT_H
#define CRDTDEMOS_CRDT_H

#include <algorithm>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "pub_sub.h"
#include "vector_clocks.h"

namespace {
    template <typename Key, typename Value>
    struct State {
        vector_clocks::Timestamp timestamp;
        Key key;
        Value value;

        State(vector_clocks::Timestamp timestamp, Key key, Value value) :
            timestamp{std::move(timestamp)},
            key{std::move(key)},
            value{std::move(value)}
        {}

        State(const State& other) = default;
        State& operator=(const State& other) = default;
    };
}

template <typename Key, typename Value>
class CRDT
{
    using _State = State<Key, Value>;

public:
    std::optional<Value> read(Key key) const
    {
        using std::cbegin, std::cend;

        const auto state = std::find_if(begin(_states), end(_states),
                                        [&](const auto& state){ return state.key == key; });
        if (state != end(_states)) {
            return state->value;
        }

        return std::nullopt;
    }

    void add(vector_clocks::Timestamp timestamp, Key key, Value value) {
        decltype(_states) previous;

        for (const auto& state : _states) {
            if (state.key == key) {
                previous.push_back(state);
            }
        }

        using std::cbegin, std::cend;
        if (previous.empty() || std::all_of(cbegin(previous), cend(previous),
                                            [&](const auto& state)
                                            {
                                                // TODO: Should it be an invariant that `timestamp` and all previous timestamps are comparable?
                                                // Discard this `add` call if there exists an incomparable timestamp.
                                                if (!vector_clocks::are_comparable(state.timestamp, timestamp)) {
                                                    return false;
                                                }
                                                const auto result = state.timestamp < timestamp;
                                                return result;
                                            })) {
            // Remove all previous states for `key`
            using std::begin, std::end;
            _states.erase(std::remove_if(begin(_states), end(_states),
                                         [&](const _State& state) { return state.key == key; }),
                          end(_states));

            // Put new state
            _states.push_back({timestamp, key, value});
        }
    }

private:
    std::vector<_State> _states;
};

#endif //CRDTDEMOS_CRDT_H
