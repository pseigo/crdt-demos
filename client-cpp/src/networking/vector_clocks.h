//
// Created by Peyton Seigo on 2023-09-17.
//

#ifndef CRDTDEMOS_VECTOR_CLOCKS_H
#define CRDTDEMOS_VECTOR_CLOCKS_H

#include <cstddef>
#include <map>
#include <optional>
#include <vector>

#include "types.h"

namespace vector_clocks {
    using event_count_t = size_t;

    struct Timestamp {
        std::map<NodeId, event_count_t> counts;
    };

    /**
     * Initializes event count to 0 for each `node_id` that is not already in `timestamp`.
     */
    void init_new_nodes(Timestamp& timestamp, const std::vector<NodeId>& node_ids)
    {
        for (const auto& node_id : node_ids) {
            timestamp.counts.insert(std::make_pair(node_id, 0));
        }
    }

    void before_send(const NodeId& node_id, Timestamp& timestamp)
    {
        timestamp.counts.at(node_id)++;

        // TODO: send `(timestmap, message)` via network (don't do it here; not a concern of `vector_clocks`)
    }

    /**
     * Merges `message_timestamp` into `local_timestamp` by element-wise maximum.
     */
    void merge(Timestamp& local_timestamp, const Timestamp& message_timestamp)
    {
        using std::make_pair;

        // TODO: find optimal way to loop through messages and init counts for new nodes.

        for (auto& pair : message_timestamp.counts) {
            const NodeId& node_id = pair.first;
            const event_count_t reported_event_count = pair.second;

            // Initialize if node is not in timestamp.
            local_timestamp.counts.insert(make_pair(node_id, 0));

            auto& local_event_count = local_timestamp.counts.at(node_id);
            local_event_count = std::max(local_event_count, reported_event_count);
        }
    }

    void on_receive(const NodeId& local_node_id, Timestamp& local_timestamp, const Timestamp& message_timestamp)
    {
        merge(local_timestamp, message_timestamp);
        local_timestamp.counts.at(local_node_id)++;
    }

    /**
     * Increments event count for `node_id`.
     * @throws std::out_of_range If `node_id` is not in `timestamp`.
     */
    void increment(Timestamp& timestamp, const NodeId& node_id)
    {
        timestamp.counts.at(node_id)++;
    }

    // TODO: measure performance of vector timestamp comparison and make them faster if needed
    /**
     * @return true iff the timestamps are comparable.
     */
    bool are_comparable(const Timestamp& left, const Timestamp& right)
    {
        if (left.counts.size() != right.counts.size()) {
            return false;
        }

        bool left_has_a_greater_value = false;
        bool right_has_a_greater_value = false;

        for (const auto& pair : left.counts) {
            const NodeId& node_id = pair.first;
            const event_count_t left_count = pair.second;

            try {
                const event_count_t right_count = right.counts.at(node_id);

                if (left_count > right_count) {
                    left_has_a_greater_value = true;
                } else if (right_count > left_count) {
                    right_has_a_greater_value = true;
                }
            } catch (std::out_of_range) {
                return false;
            }
        }

        return !(left_has_a_greater_value && right_has_a_greater_value);
    }
}

/**
 * Where `V(e)` is the timestamp of event `e`, `(V(a) == V(b)) <==> (a == b)`.
 * @throws std::logic_error if the timestamps are not comparable.
 * @throws std::out_of_bounds if an error occurs due to some node not being common to both timestamps.
 */
bool operator==(const vector_clocks::Timestamp& left, const vector_clocks::Timestamp& right)
{
    if (!vector_clocks::are_comparable(left, right)) {
        throw std::logic_error("timestamps are not comparable");
    }

    for (const auto& pair: left.counts) {
        const NodeId& node_id = pair.first;
        const vector_clocks::event_count_t left_count = pair.second;
        const vector_clocks::event_count_t right_count = right.counts.at(node_id);

        if (left_count != right_count) {
            return false;
        }
    }

    return true;
}

/**
 * @throws std::logic_error if the timestamps are not comparable.
 * @throws std::out_of_bounds if an error occurs due to some node not being common to both timestamps.
 */
bool operator<=(const vector_clocks::Timestamp& left, const vector_clocks::Timestamp& right)
{
    if (!are_comparable(left, right)) {
        throw std::logic_error("timestamps are not comparable");
    }

    for (const auto& pair: left.counts) {
        const NodeId& node_id = pair.first;
        const vector_clocks::event_count_t left_count = pair.second;
        const vector_clocks::event_count_t right_count = right.counts.at(node_id);

        if (left_count > right_count) {
            return false;
        }
    }

    return true;
}

/**
  * @throws std::logic_error if the timestamps are not comparable.
  * @throws std::out_of_bounds if an error occurs due to some node not being common to both timestamps.
  */
bool operator!=(const vector_clocks::Timestamp& left, const vector_clocks::Timestamp& right)
{
    return !(left == right);
}

/**
 * Where `V(e)` is the timestamp of event `e`, `(V(a) < V(b)) <==> (a -> b)` (`a` happens before `b`).
 * @throws std::logic_error if the timestamps are not comparable.
 * @throws std::out_of_bounds if an error occurs due to some node not being common to both timestamps.
 */
bool operator<(const vector_clocks::Timestamp& left, const vector_clocks::Timestamp& right)
{
    return left <= right && left != right;
}

/**
 * @throws std::logic_error if the timestamps are not comparable.
 * @throws std::out_of_bounds if an error occurs due to some node not being common to both timestamps.
 */
bool operator>(const vector_clocks::Timestamp& left, const vector_clocks::Timestamp& right)
{
    return !(left <= right);
}

/**
  * @throws std::logic_error if the timestamps are not comparable.
  * @throws std::out_of_bounds if an error occurs due to some node not being common to both timestamps.
  */
bool operator>=(const vector_clocks::Timestamp& left, const vector_clocks::Timestamp& right)
{
    return (left > right) || (left == right);
}

/**
 * Where `V(e)` is the timestamp of event `e`, `(V(a) || V(b)) <==> (a || b)` (`a` is concurrent with `b`).
 * @return true if timestamps are concurrent with each other.
 * @throws std::logic_error if the timestamps are not comparable.
 * @throws std::out_of_bounds if an error occurs due to some node not being common to both timestamps.
 */
 /*
bool operator||(const vector_clocks::Timestamp& left, const vector_clocks::Timestamp& right)
{
    return !(left <= right) && (left != right);
}
  */
// Woops, `||` actually means "incomparable" for timestamps.
 /*
bool operator||(const vector_clocks::Timestamp& left, const vector_clocks::Timestamp& right)
{
    return !vector_clocks::are_comparable(left, right);
}
  */


#include "../../third_party/nlohmann/json.hpp"

#include "../functional/expected.h"

template<>
class JsonCoder<vector_clocks::Timestamp>
{
public:
    std::string encode(const vector_clocks::Timestamp& timestamp) const
    {
        std::ostringstream oss;
        oss << '[';

        const auto num_nodes = timestamp.counts.size();
        size_t i = 0;

        for (const auto& pair : timestamp.counts) {
            oss << "{\"node_id\": \"" << pair.first << "\", \"count\": " << pair.second << "}";
            if (i < num_nodes - 1) {
                oss << ", ";
            }
            i++;
        }

        oss << ']';
        return oss.str();
    }

    template <typename Result = Expected<vector_clocks::Timestamp, std::string>>
    Result decode(std::string encoded) const
    {
        // TODO: can `nlohmann::json` provide detailed parse error messages?
        using json = nlohmann::json;

        decltype(vector_clocks::Timestamp::counts) counts;

        // TODO: catch or document `json::parse` exceptions
        json nodes_container = json::parse(encoded);

        for (const auto& node_container : nodes_container) {
            std::optional<std::string> node_id{std::nullopt};
            std::optional<vector_clocks::event_count_t> count{std::nullopt};

            try {
                node_id = node_container.at("node_id");
                count = node_container.at("count");
            } catch (json::type_error error) {
                using std::to_string;
                std::ostringstream oss;
                oss << "type error (nlohmann::json::type_error::id = " << to_string(error.id) << ")";
                return Result::error(oss.str());
            } catch (json::out_of_range error) {
                using std::to_string;
                std::ostringstream oss;
                oss << "type error (nlohmann::json::out_of_range::id = " << to_string(error.id) << ")";
                return Result::error(oss.str());
            } catch (std::invalid_argument error) {
                return Result::error(std::string(error.what()) + " - string does not map to any enum value");
            }

            if (!(node_id && count)) {
                return Result::error("missing expected value");
            }

            counts.insert(std::make_pair(node_id.value(), count.value()));
        }

        vector_clocks::Timestamp timestamp;
        std::swap(counts, timestamp.counts);

        return Result::success(timestamp);
    }
};

#endif //CRDTDEMOS_VECTOR_CLOCKS_H
