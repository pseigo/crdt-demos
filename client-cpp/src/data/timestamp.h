//
// Created by Peyton Seigo on 2023-09-15.
//

#ifndef CRDTDEMOS_TIMESTAMP_H
#define CRDTDEMOS_TIMESTAMP_H

#include <string>

struct Timestamp {
    Timestamp() = default;
    Timestamp(const Timestamp& other) = default;
    Timestamp& operator=(const Timestamp& other) = default;

    // TODO: Lamport timestamps - compare
    bool operator<(const Timestamp& right) const {
        // TODO: stub
        //return true;
        return _tp < right._tp;
    }

    static Timestamp now(std::string node_name)
    {
        // TODO: Lamport timestamps - new
        Timestamp ts;
        ts._tp = std::chrono::steady_clock::now();
        return ts;
    }

private:
    std::chrono::steady_clock::time_point _tp;
};

namespace Lamport {
    Timestamp now(std::string node_name)
    {
        // TODO: Lamport timestamps - new
        //return {};
        return Timestamp::now(node_name);
    }
}

#endif //CRDTDEMOS_TIMESTAMP_H
