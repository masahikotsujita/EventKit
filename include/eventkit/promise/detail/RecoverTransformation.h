//
// Created by Masahiko Tsujita on 2019-08-17.
//

#ifndef EVENTKIT_RECOVERTRANSFORMATION_H
#define EVENTKIT_RECOVERTRANSFORMATION_H

#include <eventkit/promise/Result.h>
#include <eventkit/promise/Promise.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E, typename F, typename Handler>
class RecoverTransformation : public ResultHandler<T, E> {
public:
    template <typename Tr>
    explicit RecoverTransformation(const std::shared_ptr<PromiseCore < T, F>>& pCore, Tr&& transformation)
        : m_pCore(pCore)
        , m_transformation(std::forward<Tr>(transformation))
    {
    }

    ~RecoverTransformation() override = default;

    void handleResult (const Result<T, E>& result) override {
        if (result.getType() == ResultType::failed) {
            m_transformation(result.getError()).addHandler(m_pCore);
        } else {
            m_pCore->handleResult(Result<T, F>::succeeded(result.getValue()));
        }
    }

private:
    std::shared_ptr<PromiseCore<T, F>> m_pCore;
    Handler m_transformation;

};

}
}
}

#endif //EVENTKIT_RECOVERTRANSFORMATION_H
