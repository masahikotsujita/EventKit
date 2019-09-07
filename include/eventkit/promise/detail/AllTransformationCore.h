//
// Created by Masahiko Tsujita on 2019/09/04.
//

#ifndef EVENTKIT_ALLTRANSFORMATIONCORE_H
#define EVENTKIT_ALLTRANSFORMATIONCORE_H

#include <tuple>
#include <eventkit/promise/Result.h>
#include <bitset>

namespace ek {
namespace promise {
namespace detail {

template <typename ...Prs>
class AllTransformationCore : public PromiseCore<values_of_prmises_t<Prs...>, error_of_prmises_t<Prs...>> {
public:
    using Results = std::tuple<result_of_promise_t<Prs>...>;
    using Values = values_of_prmises_t<Prs...>;
    using Error = error_of_prmises_t<Prs...>;
    using Super = PromiseCore<Values, Error>;
    using Result = ek::promise::Result<Values, Error>;
    
    template <size_t Index>
    void onResultAt(const std::tuple_element_t<Index, Results>& result) {
        if (result.getType() == ResultType::succeeded) {
            std::get<Index>(m_values) = result.getValue();
            m_fulfilledPromiseFlags[Index] = true;
            if (m_fulfilledPromiseFlags.all()) {
                Super::onResult(Result::succeeded(m_values));
            }
        } else {
            Super::onResult(Result::failed(result.getError()));
        }
    }

private:
    Values m_values;
    std::bitset<std::tuple_size<Values>::value> m_fulfilledPromiseFlags;

};

}
}
}

#endif //EVENTKIT_ALLTRANSFORMATIONCORE_H
