//
// Created by Peyton Seigo on 2023-09-15.
//

#ifndef CRDTDEMOS_OVERLOADED_H
#define CRDTDEMOS_OVERLOADED_H

template <typename... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// “The `overloaded` template takes a list of function objects and creates a
// new function object that presents the call operators of all provided
// function objects as its own (the `using Ts::operator()...` part).”
//
// – Čukić, I. (2018), Functional Programming in C++  (s. 9.3; p. 271)

#endif //CRDTDEMOS_OVERLOADED_H
