//
// Created by Tsujita Masahiko on 2020/09/04.
//

#ifndef EVENTKIT_FUNCTIONRESULTHANDLER_INL_H
#define EVENTKIT_FUNCTIONRESULTHANDLER_INL_H

#include <eventkit/promise/detail/FunctionResultHandler.h>

namespace ek {
namespace promise {
namespace detail{

template <typename T, typename E, typename Function>
template <typename F>
FunctionResultHandler<T, E, Function>::FunctionResultHandler(ek::common::Allocator* pA, F&& function)
    : m_function(std::forward<F>(function))
    , m_pA(pA)
    , m_intrusiveMixin([](ek::common::IntrusiveMixin*, void* pContext){
        auto* pThis = static_cast<FunctionResultHandler*>(pContext);
        pThis->m_pA->destroy(pThis);
    }, this) {
}

template <typename T, typename E, typename Function>
void FunctionResultHandler<T, E, Function>::onResult(const Result <T, E>& result) {
    m_function(result);
}

template <typename T, typename E, typename Function>
void FunctionResultHandler<T, E, Function>::ref() {
    m_intrusiveMixin.ref();
}

template <typename T, typename E, typename Function>
void FunctionResultHandler<T, E, Function>::unref() {
    m_intrusiveMixin.unref();
}

template <typename T, typename E, typename Function>
auto make_function_observer(ek::common::Allocator* pA, Function&& function) -> ek::common::IntrusivePtr<ResultHandler<T, E>> {
    return ek::common::make_intrusive<FunctionResultHandler<T, E, std::decay_t<Function>>>(pA, pA, std::forward<Function>(function));
}

}
}
}

#endif //EVENTKIT_FUNCTIONRESULTHANDLER_INL_H
