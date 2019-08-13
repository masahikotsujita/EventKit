//
// Created by Masahiko Tsujita on 2019-08-13.
//

#ifndef EVENTKIT_DISPATCHQUEUE_INL_H
#define EVENTKIT_DISPATCHQUEUE_INL_H

namespace ek {

template <typename Function>
template <typename F>
DispatchFunctionItem<Function>::DispatchFunctionItem(F&& function)
    : m_function(std::forward<F>(function)) {
}

template <typename Function>
void DispatchFunctionItem<Function>::run() {
    m_function();
}

template <typename F>
void DispatchQueue::dispatchAsync(F&& function) {
    dispatchItemAsync(std::make_shared<DispatchFunctionItem<std::decay_t<F>>>(std::forward<F>(function)));
}

}

#endif //EVENTKIT_DISPATCHQUEUE_INL_H
