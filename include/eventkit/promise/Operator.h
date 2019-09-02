//
// Created by Masahiko Tsujita on 2019/09/02.
//

#ifndef EVENTKIT_OPERATOR_H
#define EVENTKIT_OPERATOR_H

#include <utility>

namespace ek {
namespace promise {

template <typename Pr, typename Op>
decltype(auto) operator | (Pr&& pr, Op&& op) {
    return std::forward<Op>(op)(std::forward<Pr>(pr));
}

}
}

#endif //EVENTKIT_OPERATOR_H
