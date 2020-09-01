//
// Created by Tsujita Masahiko on 2020/08/18.
//

#ifndef EVENTKIT_WHENALL_INL_H
#define EVENTKIT_WHENALL_INL_H

namespace ek {
namespace promise {
namespace global_functions {
namespace detail {

template <size_t Idx, typename Cr, typename LastPr>
void addCoreAsHandlerToPromisesAt(const ek::common::IntrusivePtr<Cr>& pCore, LastPr&& lastPr) {
    lastPr.done(ek::promise::detail::make_function_observer<typename LastPr::Value, typename LastPr::Error>([pCore](const auto& result){
        pCore->template onResultAt<Idx>(result);
    }));
}

template <size_t Idx, typename Cr, typename PrAtIndex, typename ...RestPrs>
void addCoreAsHandlerToPromisesAt(const ek::common::IntrusivePtr<Cr>& pCore, PrAtIndex&& promiseAtIndex, RestPrs&& ...restPromises) {
    promiseAtIndex.done(ek::promise::detail::make_function_observer<typename PrAtIndex::Value, typename PrAtIndex::Error>([pCore](const auto& result){
        pCore->template onResultAt<Idx>(result);
    }));
    addCoreAsHandlerToPromisesAt<Idx + 1>(pCore, std::forward<RestPrs>(restPromises)...);
}

template <typename Cr, typename ...Prs>
void addCoreAsHandlerToPromises(const ek::common::IntrusivePtr<Cr>& pCore, Prs&& ...promises) {
    addCoreAsHandlerToPromisesAt<0>(pCore, std::forward<Prs>(promises)...);
}

}
}
}
}

#endif //EVENTKIT_WHENALL_INL_H
