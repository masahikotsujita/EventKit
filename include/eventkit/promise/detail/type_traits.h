//
// Created by Masahiko Tsujita on 2019/09/06.
//

#ifndef EVENTKIT_TYPE_TRAITS_H
#define EVENTKIT_TYPE_TRAITS_H

#include <tuple>

namespace ek {
namespace promise {

template <typename T, typename E>
class Result;

namespace detail {

template <typename Pr>
using result_of_promise_t = ek::promise::Result<typename Pr::Value, typename Pr::Error>;

template <typename ...Prs>
using values_of_prmises_t = std::tuple<typename Prs::Value...>;

template <typename ...Prs>
using error_of_prmises_t = typename std::tuple_element_t<0, std::tuple<Prs...>>::Error;

}
}
}

#endif //EVENTKIT_TYPE_TRAITS_H
