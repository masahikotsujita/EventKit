//
// Created by Masahiko Tsujita on 2019-08-14.
//

#ifndef EVENTKIT_RESULT_H
#define EVENTKIT_RESULT_H

#include <type_traits>
#include <cassert>

namespace ek {
namespace promise {

enum class ResultType {
    succeeded = 1,
    failed
};

template <typename T, typename E>
class Result final {
public:
    using Value = T;
    using Error = E;

    Result()
        : m_type(static_cast<ResultType>(0)) {
    }

    Result(const Result& other)
        : m_type(other.m_type) {
        switch (other.m_type) {
        case ResultType::succeeded:
            constructValue(other.m_value);
            break;
        case ResultType::failed:
            constructError(other.m_error);
            break;
        default:
            break;
        }
    }

    Result(Result&& other) noexcept
        : m_type(other.m_type) {
        switch (other.m_type) {
        case ResultType::succeeded:
            constructValue(std::move(other).m_value);
            break;
        case ResultType::failed:
            constructError(std::move(other).m_error);
            break;
        default:
            break;
        }
    }

    Result& operator=(const Result& other) {
        if (&other == this) {
            return *this;
        }
        if (m_type != other.m_type) {
            clear();
        }
        switch (other.m_type) {
        case ResultType::succeeded:
            if (m_type == ResultType::succeeded) {
                assignValue(other.m_value);
            } else {
                constructValue(other.m_value);
            }
            break;
        case ResultType::failed:
            if (m_type == ResultType::failed) {
                assignError(other.m_error);
            } else {
                constructError(other.m_error);
            }
            break;
        default:
            break;
        }
        m_type = other.m_type;
        return *this;
    }

    Result& operator=(Result&& other) noexcept {
        if (&other == this) {
            return *this;
        }
        if (m_type != other.m_type) {
            clear();
        }
        switch (other.m_type) {
        case ResultType::succeeded:
            if (m_type == ResultType::succeeded) {
                assignValue(std::move(other).m_value);
            } else {
                constructValue(std::move(other).m_value);
            }
            break;
        case ResultType::failed:
            if (m_type == ResultType::failed) {
                assignError(std::move(other).m_error);
            } else {
                constructError(std::move(other).m_error);
            }
            break;
        default:
            break;
        }
        m_type = other.m_type;
        return *this;
    }

    ~Result() {
        clear();
    }

private:
    struct construct_with_value_t { explicit construct_with_value_t() = default; };
    struct construct_with_error_t { explicit construct_with_error_t() = default; };
    static constexpr construct_with_value_t construct_with_value = construct_with_value_t();
    static constexpr construct_with_error_t construct_with_error = construct_with_error_t();

    template <typename ...Args>
    explicit Result(construct_with_value_t, Args&& ...args)
        : m_type(ResultType::succeeded) {
        constructValue(std::forward<Args>(args)...);
    }

    template <typename ...Args>
    explicit Result(construct_with_error_t, Args&& ...args)
        : m_type(ResultType::failed) {
        constructError(std::forward<Args>(args)...);
    }

    template <typename ...Args>
    void constructValue(Args&& ...args) {
        new(&m_value) Value(std::forward<Args>(args)...);
    }

    template <typename _T>
    void assignValue(_T&& value) {
        m_value = std::forward<_T>(value);
    }

    template <typename ...Args>
    void constructError(Args&& ...args) {
        new(&m_error) Error(std::forward<Args>(args)...);
    }

    template <typename _E>
    void assignError(_E&& error) {
        m_error = std::forward<_E>(error);
    }

    void clear() {
        switch (m_type) {
        case ResultType::succeeded:
            m_value.~Value();
            break;
        case ResultType::failed:
            m_error.~Error();
            break;
        default:
            break;
        }
    }

public:
    template <typename ...Args>
    static Result succeeded(Args&& ...args) {
        return Result(construct_with_value, std::forward<Args>(args)...);
    }

    template <typename ...Args>
    static Result failed(Args&& ...args) {
        return Result(construct_with_error, std::forward<Args>(args)...);
    }

    ResultType getType() const {
        return m_type;
    }

    Value& getValue() noexcept {
        assert(m_type == ResultType::succeeded);
        return m_value;
    }

    const Value& getValue() const noexcept {
        assert(m_type == ResultType::succeeded);
        return m_value;
    }

    Error& getError() noexcept {
        assert(m_type == ResultType::failed);
        return m_error;
    }

    const Error& getError() const noexcept {
        assert(m_type == ResultType::failed);
        return m_error;
    }

private:
    ResultType m_type;
    union {
        Value m_value;
        Error m_error;
    };

};

}
}

#endif //EVENTKIT_RESULT_H
