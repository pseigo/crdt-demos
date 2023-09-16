//
// Created by Peyton Seigo on 2023-09-15.
//

#ifndef CRDTDEMOS_STORE_ITEM_H
#define CRDTDEMOS_STORE_ITEM_H

#include "../third_party/nlohmann/json.hpp"

#include "data/sample_data.h"
#include "../error.h"

#include "store_category.h"

// TODO: Choose a UUID version that will be suitable for my use cases
// TODO: Generate UUIDs
using StoreItemId = string;

struct StoreItem {
    StoreItemId id;

    std::string name;
    StoreCategory category;

    long double price;
    Currency currency;

    StoreItem() = default;

    StoreItem(StoreItemId id,
              std::string name,
              StoreCategory category,
              decltype(price) price,
              Currency currency) :
            id{id},
            name{std::move(name)},
            category{category},
            price{price},
            currency{currency}
    {}

    StoreItem(const StoreItem& other) = default;
    StoreItem& operator=(const StoreItem& other) = default;
    StoreItem(StoreItem&& other) = default;
    StoreItem& operator=( StoreItem&& other) = default;

    void swap(StoreItem& other) noexcept {
        std::swap(this->id, other.id);
        std::swap(this->name, other.name);
        std::swap(this->category, other.category);
        std::swap(this->price, other.price);
        std::swap(this->currency, other.currency);
    }

    bool operator<(const StoreItem& other) const {
        // INVARIANT: IDs are unique
        return id < other.id;
    }
};

template<>
std::vector<StoreItem> sample_data<StoreItem>() {
    return {
            {"987C30DB-A335-40F0-8694-0921EAB9406F", "Microfiber Cloth", StoreCategory::CLEANING,             7.99,  Currency::CAD},
            {"D91A2AF2-7957-4B64-9B7E-2B1FD21FF519", "Bluetooth Mouse",  StoreCategory::COMPUTER_PERIPHERALS, 34.99, Currency::CAD}
    };
}

template <>
class JsonCoder<StoreItem>
{
public:
    std::string encode(StoreItem item) const
    {
        using std::to_string;
        std::ostringstream os;
        os << "{"
           << "\"id\": \"" << item.id << "\", "
           << "\"name\": \"" << item.name << "\", "
           << "\"category\": \"" << to_string(item.category) << "\", "
           << "\"price\": " << to_string(item.price) << ", "
           << "\"currency\": \"" << to_string(item.currency) << "\""
           << "}";
        return os.str();
    }

    //StoreItem decode(std::string encoded) const
    //Expected<StoreItem, std::string> decode(std::string encoded) const
    template <typename Result = Expected<StoreItem, std::string>>
    Result decode(std::string encoded) const
    {
        using json = nlohmann::json;

        // TODO: catch or document `json::parse` exceptions
        json data = json::parse(encoded);

        std::optional<StoreItemId> id{std::nullopt};
        std::optional<std::string> name{std::nullopt};
        std::optional<StoreCategory> category{std::nullopt};
        std::optional<long double> price{std::nullopt};
        std::optional<Currency> currency{std::nullopt};

        try {
            id = data.at("id");
            name = data.at("name");
            category = to_enum<StoreCategory>(data.at("category"));
            price = data.at("price");
            currency = to_enum<Currency>(data.at("currency"));
        } catch (json::type_error error) {
            using std::to_string;
            return Result::error("type error (nlohmann::json::type_error::id = " + to_string(error.id));
        } catch (json::out_of_range error) {
            using std::to_string;
            return Result::error("type error (nlohmann::json::out_of_range::id = " + to_string(error.id));
        } catch (std::invalid_argument error) {
            return Result::error(std::string(error.what()) + " - string does not map to any enum value");
        }

        if (!(id && name && category && price && currency)) {
            return Result::error("missing expected value");
        }

        return Result::success(id.value(), name.value(), category.value(), price.value(), currency.value());

        /*
        try {
            id = data.at("id");
        } catch (json::type_error error) {
            if (error.id == 304) { // Not an object

            } else {

            }
        } catch (json::out_of_range error) {
            if (error.id == 403) { // key is not stored in object

            } else {

            }
        }
         */
    }
};

#endif //CRDTDEMOS_STORE_ITEM_H
