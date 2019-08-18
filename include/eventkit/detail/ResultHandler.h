//
// Created by Masahiko Tsujita on 2019-08-15.
//

#ifndef EVENTKIT_RESULTHANDLER_H
#define EVENTKIT_RESULTHANDLER_H

#include <eventkit/Result.h>
#include <eventkit/Promise.h>

namespace ek {
namespace detail {

template <typename T, typename E>
class ResultHandler {
public:
    virtual ~ResultHandler() = default;

    virtual void handleResult(const Result <T, E>& result) = 0;

};

template <typename T, typename E, typename Function>
class FunctionResultHandler : public ResultHandler<T, E> {
public:
    template <typename F>
    explicit FunctionResultHandler(F&& function)
        : m_function(std::forward<F>(function)) {
    }

    virtual ~FunctionResultHandler() override = default;

    virtual void handleResult(const Result <T, E>& result) override {
        m_function(result);
    }

private:
    Function m_function;

};

template <typename T, typename E, typename Function>
auto make_function_handler(Function&& function) -> std::shared_ptr<ResultHandler<T, E>> {
    return std::make_shared<FunctionResultHandler<T, E, std::decay_t<Function>>>(std::forward<Function>(function));
}

}
}

#endif //EVENTKIT_RESULTHANDLER_H
