//
// Created by Peyton Seigo on 2023-09-21.
//

#ifndef CRDTDEMOS_MESSAGE_H
#define CRDTDEMOS_MESSAGE_H

#include <ostream>

#include "networking/types.h"
#include "networking/json_coder.h"
#include "networking/vector_clocks.h"

struct Message
{
    NodeId source;
    vector_clocks::Timestamp timestamp;
    std::string data;
};

template <>
class JsonCoder<Message>
{
public:
    std::string encode(const Message& message) const
    {
        using std::to_string;

        const auto encoded_timestamp = JsonCoder<vector_clocks::Timestamp>{}.encode(message.timestamp);
        std::ostringstream oss;
        oss << "{\"source\": \"" << message.source << "\", \"timestamp\": " << encoded_timestamp << ", \"data\": " << message.data << "}";
        return oss.str();
    }

    template <typename Result = Expected<Message, std::string>>
    Result decode(const std::string& encoded) const
    {
        using json = nlohmann::json;

        // TODO: catch or document `json::parse` exceptions
        json container = json::parse(encoded);

        std::optional<NodeId> source{std::nullopt};
        std::optional<std::string> timestamp_str{std::nullopt};
        //std::optional<json> timestamp_container{std::nullopt};
        std::optional<std::string> data{std::nullopt};

        try {
            source = container.at("source");
            timestamp_str = container.at("timestamp").dump();
            //timestamp_container = container.at("timestamp");
            data = container.at("data").dump();
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

        if (!(source && timestamp_str && data)) {
            return Result::error("missing expected value");
        }

        auto expected_timestamp = JsonCoder<vector_clocks::Timestamp>{}.decode(timestamp_str.value());
        if (!expected_timestamp) {
            std::ostringstream oss;
            const auto error = expected_timestamp.error();
            oss << "failed to parse timestamp; error: " << error;
            return Result::error(oss.str());
        }
        const auto timestamp = expected_timestamp.get();

        Message message;
        message.source = source.value();
        message.timestamp = timestamp;
        message.data = data.value();

        return Result::success(message);
    }
};

#endif //CRDTDEMOS_MESSAGE_H
