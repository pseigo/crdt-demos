//
// Created by Peyton Seigo on 2023-09-15.
//

#ifndef CRDTDEMOS_TO_ENUM_H
#define CRDTDEMOS_TO_ENUM_H

#include <string>

/**
 * Returns an `Enum` value corresponding to `str`.
 *
 * @param str Lowercase representation of an `Enum` value.
 * @throws std::invalid_argument if `str` does not map to any value in `Enum`.
 */
template <typename Enum>
Enum to_enum(std::string str);

#endif //CRDTDEMOS_TO_ENUM_H
