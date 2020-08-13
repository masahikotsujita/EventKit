//
// Created by Masahiko Tsujita on 2019/09/04.
//

#ifndef EVENTKIT_ALL_H
#define EVENTKIT_ALL_H

#include <eventkit/promise/detail/type_traits.h>
#include <eventkit/promise/detail/AllTransformationCore.h>
#include <type_traits>
#include <memory>

namespace ek {
namespace promise {
namespace operators {
namespace detail {

template <size_t Idx, typename Cr, typename LastPr>
void addCoreAsHandlerToPromisesAt(const ek::common::intrusive_ptr<Cr>& pCore, LastPr&& lastPr) {
    lastPr.pipe(ek::promise::detail::make_function_observer<typename LastPr::Value, typename LastPr::Error>([pCore](const auto& result){
        pCore->template onResultAt<Idx>(result);
    }));
}

template <size_t Idx, typename Cr, typename PrAtIndex, typename ...RestPrs>
void addCoreAsHandlerToPromisesAt(const ek::common::intrusive_ptr<Cr>& pCore, PrAtIndex&& promiseAtIndex, RestPrs&& ...restPromises) {
    promiseAtIndex.pipe(ek::promise::detail::make_function_observer<typename PrAtIndex::Value, typename PrAtIndex::Error>([pCore](const auto& result){
        pCore->template onResultAt<Idx>(result);
    }));
    addCoreAsHandlerToPromisesAt<Idx + 1>(pCore, std::forward<RestPrs>(restPromises)...);
}

template <typename Cr, typename ...Prs>
void addCoreAsHandlerToPromises(const ek::common::intrusive_ptr<Cr>& pCore, Prs&& ...promises) {
    addCoreAsHandlerToPromisesAt<0>(pCore, std::forward<Prs>(promises)...);
}

}

template <typename ...Prs>
auto whenAll(Prs&& ...promises) {
    using Values = ek::promise::detail::values_of_prmises_t<std::decay_t<Prs>...>;
    using Error = ek::promise::detail::error_of_prmises_t<Prs...>;
    auto pCore = ek::common::make_intrusive<ek::promise::detail::AllTransformationCore<std::decay_t<Prs>...>>();
    detail::addCoreAsHandlerToPromises(pCore, std::forward<Prs>(promises)...);
    return ek::promise::detail::make_promise<Values, Error>(pCore);
}

}
}
}

#endif //EVENTKIT_ALL_H
