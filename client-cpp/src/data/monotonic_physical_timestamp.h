//
// Created by Peyton Seigo on 2023-09-15.
//

#ifndef CRDTDEMOS_MONOTONIC_PHYSICAL_TIMESTAMP_H
#define CRDTDEMOS_MONOTONIC_PHYSICAL_TIMESTAMP_H

#include <chrono>

struct MonotonicPhysicalTimestamp {
private:
    MonotonicPhysicalTimestamp() = default;

public:
    MonotonicPhysicalTimestamp(const MonotonicPhysicalTimestamp& other) = default;
    MonotonicPhysicalTimestamp& operator=(const MonotonicPhysicalTimestamp& other) = default;

    bool operator<(const MonotonicPhysicalTimestamp& right) const {
        return _tp < right._tp;
    }

    static MonotonicPhysicalTimestamp now()
    {
        MonotonicPhysicalTimestamp ts;
        ts._tp = std::chrono::steady_clock::now();
        return ts;
    }

private:
    std::chrono::steady_clock::time_point _tp;
};

#endif //CRDTDEMOS_MONOTONIC_PHYSICAL_TIMESTAMP_H
