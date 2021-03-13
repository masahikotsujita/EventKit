//
// Created by Masahiko Tsujita on 2019/09/04.
//

#ifndef EVENTKIT_WHENALL_H
#define EVENTKIT_WHENALL_H

#include <eventkit/promise/global_functions/detail/whenAll-inl.h>
#include <eventkit/promise/detail/type_traits.h>
#include <eventkit/promise/detail/WhenAllTransformationPromiseCore.h>
#include <type_traits>
#include <memory>
#include <vector>
#include <eventkit/common/AllocatorStack.h>

namespace ek {
namespace promise {
namespace global_functions {

struct pack_as_tuple_t { };
constexpr pack_as_tuple_t pack_as_tuple = pack_as_tuple_t();

template <typename ...Prs>
auto whenAll(pack_as_tuple_t, Prs&& ...promises) {
    ek::common::Allocator* pA = ek::common::getCurrentAllocator();
    using Values = ek::promise::detail::values_of_prmises_t<std::decay_t<Prs>...>;
    using Error = ek::promise::detail::error_of_prmises_t<Prs...>;
    auto pCore = ek::common::make_intrusive<ek::promise::detail::WhenAllTransformationPromiseCore<std::decay_t<Prs>...>>(pA);
    detail::addCoreAsHandlerToPromises(pCore, std::forward<Prs>(promises)...);
    return ek::promise::detail::make_promise<Values, Error>(pCore);
}

template <typename InputIt>
auto whenAll(InputIt begin, InputIt end) {
    ek::common::Allocator* pA = ek::common::getCurrentAllocator();
    using P = ek::promise::detail::value_type_of_t<InputIt>;
    using T = typename P::Value;
    using E = typename P::Error;
    auto pCore = ek::common::make_intrusive<ek::promise::detail::DynamicWhenAllTransformationPromiseCore<T, E>>(pA, pA, std::distance(begin, end));
    size_t idx = 0;
    auto itr = begin;
    for (; itr != end; ++itr, ++idx) {
        const auto& promise = *itr;
        promise.pipe(ek::promise::detail::make_function_observer<T, E>(pA, [pCore, idx](const auto& result){
            pCore->onResultAt(result, idx);
        }));
    }
    return ek::promise::detail::make_promise<std::vector<T>, E>(pCore);
}

template <typename Prs>
auto whenAll(const Prs& promises) {
    return whenAll(promises.begin(), promises.end());
}

template <typename Pr>
auto whenAll(std::initializer_list<Pr> promises) {
    return whenAll(promises.begin(), promises.end());
}

}
}
}

#endif //EVENTKIT_WHENALL_H
