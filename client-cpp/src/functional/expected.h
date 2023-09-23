//
// Created by Peyton Seigo on 2023-09-15.
//

#ifndef CRDTDEMOS_EXPECTED_H
#define CRDTDEMOS_EXPECTED_H

#include <iostream>
#include <optional>
#include <string>
#include <stdexcept>
#include <utility>
#include <variant>

template <typename T, typename E>
class Expected
{
private:
    union {
        T _value;
        E _error;
    };

    bool _valid;

public:
    Expected() : _valid{false} {}

    Expected(const Expected& other) : _valid{other._valid} {
        if (_valid) {
            new (&_value) T(other._value);
        } else {
            new (&_error) E(other._error);
        }
    }

    Expected(Expected&& other) : _valid{other._valid} {
        if (_valid) {
            new (&_value) T(std::move(other._value));
        } else {
            new (&_error) E(std::move(other._error));
        }
    }

    ~Expected() {
        if (_valid) {
            _value.~T();
        } else {
            _error.~E();
        }
    }

    void swap(Expected& other) {
        using std::swap;
        if (_valid) {
            if (other._valid) {
                swap(_value, other._value);
            } else {
                // Grab `other`'s error and reset its union.
                auto other_error = std::move(other._error);
                other._error.~E();

                // Move `_value` into `other`.
                new (&other._value) T(std::move(_value));
                _value.~T();

                // Load `other`'s error.
                new (&_error) E(std::move(other_error));

                // Swap flags.
                swap(_valid, other._valid);
            }
        } else {
            if (other._valid) {
                other.swap(*this);
            } else {
                swap(_error, other._error);
            }
        }
    }

    Expected& operator=(Expected other)
    {
        swap(other);
        return *this;
    }

    //T& get() const {
    T& get() {
        if (!_valid) {
            throw std::logic_error("no value");
        }
        return _value;
    }

    // TODO: how to make these `const`? I get errors with `Expected<StoreItem, std::string>`
    //E& error() const {
    E& error() {
        if (_valid) {
            throw std::logic_error("no error");
        }
        return _error;
    }

    operator bool() const {
        return _valid;
    }

    operator std::optional<T>() const {
        if (_valid) {
            return _value;
        } else {
            return std::optional<T>();
        }
    }

    template <typename... Args>
    static Expected success(Args&&... params)
    {
        Expected result;
        result._valid = true;
        new (&result._value) T(std::forward<Args>(params)...);
        return result;
    }

    template <typename... Args>
    static Expected error(Args&&... params)
    {
        Expected result;
        result._valid = false;
        new (&result._error) E(std::forward<Args>(params)...);
        return result;
    }
};

// is below a typo? `template`?
//template <typename T, template Variant, template E = Expected<T, std::string>>
/*
template <typename T, typename Variant, typename E = Expected<T, std::string>>
E get_if(const Variant& variant)
{
    const T* const value_ptr = std::get_if<T>(variant);

    if (value_ptr) {
        return E::success(*value_ptr);
    } else {
        return E::error("type does not match the currently active alternative");
    }
}
 */
//template <typename T, template <typename...> typename Variant, typename E = Expected<T, std::string>>

template <typename T, typename Variant>
auto get_if(const Variant& variant) -> Expected<T, std::string>
{
    const T* value_ptr = std::get_if<T>(&variant);

    using E = Expected<T, std::string>;

    if (value_ptr) {
        return E::success(*value_ptr);
    } else {
        return E::error("type does not match the currently active alternative");
    }
}

void get_if_demo()
{
    std::variant<std::string, std::nullopt_t> v = "hi";
    auto maybe_str = get_if<std::string, decltype(v)>(v);
    if (maybe_str) {
        const std::optional<std::string> os = maybe_str;
        if (os.has_value()) {
            const auto s = os.value();
            std::cout << "[info] value from `get_if`: " << s << std::endl;
        } else {
            std::cerr << "[error] optional from `get_if` is empty" << std::endl;
        }
    } else {
        maybe_str.get();
        std::cerr << "[error] Expected from `get_if` is invalid" << std::endl;
    }

//    const auto maybe_str = get_if<std::string>(v);
}

#endif //CRDTDEMOS_EXPECTED_H
