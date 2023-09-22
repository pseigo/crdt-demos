//
// Created by Peyton Seigo on 2023-09-21.
//

#ifndef CRDTDEMOS_DATE_TIMES_H
#define CRDTDEMOS_DATE_TIMES_H

#include <chrono>

using SystemTimePoint = std::chrono::time_point<std::chrono::system_clock>;

std::string to_iso_8601(const SystemTimePoint& tp)
{
    // TODO: Impl `to_iso_8601/1`
    return "stub";
}

SystemTimePoint system_now()
{
    return std::chrono::system_clock::now();
}

#endif //CRDTDEMOS_DATE_TIMES_H
