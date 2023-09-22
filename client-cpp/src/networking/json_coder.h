//
// Created by Peyton Seigo on 2023-09-15.
//

#ifndef CRDTDEMOS_JSON_CODER_H
#define CRDTDEMOS_JSON_CODER_H

template <typename T>
class JsonCoder
{
public:
    std::string encode(const T& data) const;
    T decode(const std::string& encoded) const;
};

#endif //CRDTDEMOS_JSON_CODER_H
