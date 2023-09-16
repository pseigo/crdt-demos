//
// Created by Peyton Seigo on 2023-09-15.
//

#ifndef CRDTDEMOS_STORE_CATEGORIES_H
#define CRDTDEMOS_STORE_CATEGORIES_H

#include <cassert>
#include <string>

#include "data/to_enum.h"

enum class StoreCategory {
    CLEANING,
    COMPUTER_PERIPHERALS
};

std::string to_string(StoreCategory category)
{
    switch (category) {
        case StoreCategory::CLEANING: return "cleaning";
        case StoreCategory::COMPUTER_PERIPHERALS: return "computer_peripherals";
        default: assert(false); // bug: non-exhaustive switch
    }
}

template <>
StoreCategory to_enum<StoreCategory>(std::string category)
{
    if (category == "cleaning") { return StoreCategory::CLEANING; }
    else if (category == "computer_peripherals") { return StoreCategory::COMPUTER_PERIPHERALS; }

    throw std::invalid_argument("invalid store category");
}

#endif //CRDTDEMOS_STORE_CATEGORIES_H
