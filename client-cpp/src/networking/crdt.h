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

#include "pub_sub.h"
#include "data/timestamp.h"

using std::cout, std::endl;
using std::string;

namespace {
    template <typename Key, typename Value>
    struct State {
        Timestamp timestamp;
        Key key;
        Value value;

        State() = default;
        State(const State& other) = default;
        State& operator=(const State& other) = default;
    };
}

template <typename Key, typename Value>
class CRDT
{
public:
    using _State = State<Key, Value>;

    std::optional<Value> read(Key key) const
    {
        using std::cbegin, std::cend;

        //if (const auto value = std::find(cbegin(_states), cend(_states), key) != cend(_states)) {
        const auto state = std::find_if(begin(_states), end(_states),
                                        [&](const auto& state){ return state.key == key; });
        if (state != end(_states)) {
            return state->value;
        }

        return std::nullopt;
    }

    /*
    void set(Key key, Value value, PubSub& pub_sub) {
        Timestamp timestamp = Timestamp::now("cpp_client_1");
        pub_sub.broadcast()
    }
     */

    void on_receive(Timestamp timestamp, Key key, Value value) {
        decltype(_states) previous;

        for (const auto& state : _states) {
            if (state.key == key) {
                previous.push_back(state);
            }
        }

        using std::cbegin, std::cend;
        if (previous.empty() || std::all_of(cbegin(previous), cend(previous),
                                            [&](const auto &state) { return state.timestamp < timestamp; })) {
            decltype(_states) new_states;
            std::swap(_states, new_states);

            // Remove all previous states for `key`
            using std::begin, std::end;
            new_states.erase(std::remove_if(begin(new_states), end(new_states),
                                            [&](const _State &state) { return state.key == key; }),
                             end(new_states));

            // Put new state
            new_states.push_back({timestamp, key, value});

            std::swap(_states, new_states);
        }
    }

private:
    std::vector<_State> _states;
};

#endif //CRDTDEMOS_CRDT_H

/*
```
on initialization do
    values := {}
end on

on request to read value for key k do
    if \exists t, v. (t, k, v) \in values then return v else return null
end on

on request to set key k to value v do
    t := newTimestamp()  |> globally unique, e.g. Lamport timestamp
    broadcast (set, t, k, v) by reliable broadcast (including to self)
end on

on delivering (set, t, k, v) by reliable broadcast do
    previous := {(t', k', v') \in values | k' = k}
    if previous = {} \or \forall (t', k', v') \in previous. t' < t then
        values := (values \ previous) \intersection {(t, k, v)}
    end if
end on
```
*/
