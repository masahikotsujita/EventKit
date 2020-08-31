//
// Created by Masahiko Tsujita on 2019/09/04.
//

#ifndef EVENTKIT_WHENALL_H
#define EVENTKIT_WHENALL_H

#include <eventkit/promise/global_functions/detail/whenAll-inl.h>
#include <eventkit/promise/detail/type_traits.h>
#include <eventkit/promise/detail/WhenAllTransformationCore.h>
#include <type_traits>
#include <memory>
#include <vector>

namespace ek {
namespace promise {
namespace global_functions {

struct pack_as_tuple_t { };
constexpr pack_as_tuple_t pack_as_tuple = pack_as_tuple_t();

template <typename ...Prs>
auto whenAll(ek::common::Allocator* pA, pack_as_tuple_t, Prs&& ...promises) {
    using Values = ek::promise::detail::values_of_prmises_t<std::decay_t<Prs>...>;
    using Error = ek::promise::detail::error_of_prmises_t<Prs...>;
    auto pCore = ek::common::make_intrusive<ek::promise::detail::WhenAllTransformationCore<std::decay_t<Prs>...>>(pA);
    detail::addCoreAsHandlerToPromises(pCore, std::forward<Prs>(promises)...);
    return ek::promise::detail::make_promise<Values, Error>(pCore);
}

template <typename InputIt>
auto whenAll(ek::common::Allocator* pA, InputIt begin, InputIt end) {
    using P = ek::promise::detail::value_type_of_t<InputIt>;
    using T = typename P::Value;
    using E = typename P::Error;
    auto pCore = ek::common::make_intrusive<ek::promise::detail::DynamicWhenAllTransformationCore<T, E>>(pA, pA, std::distance(begin, end));
    size_t idx = 0;
    auto itr = begin;
    for (; itr != end; ++itr, ++idx) {
        const auto& promise = *itr;
        promise.done(ek::promise::detail::make_function_observer<T, E>(pA, [pCore, idx](const auto& result){
            pCore->onResultAt(result, idx);
        }));
    }
    return ek::promise::detail::make_promise<std::vector<T>, E>(pCore);
}

template <typename Prs>
auto whenAll(ek::common::Allocator* pA, const Prs& promises) {
    return whenAll(pA, promises.begin(), promises.end());
}

template <typename Pr>
auto whenAll(ek::common::Allocator* pA, std::initializer_list<Pr> promises) {
    return whenAll(pA, promises.begin(), promises.end());
}

}
}
}

#endif //EVENTKIT_WHENALL_H
