//
// Created by Masahiko Tsujita on 2019-08-17.
//

#ifndef EVENTKIT_RECOVERCORE_H
#define EVENTKIT_RECOVERCORE_H

#include <eventkit/Result.h>
#include <eventkit/Promise.h>

namespace ek {
namespace detail {

template <typename T, typename E, typename F, typename Handler>
class RecoverCore : public ResultHandler<T, E> {
public:
    template <typename Tr>
    explicit RecoverCore(const std::shared_ptr<PromiseCore<T, F>>& pCore, Tr&& transformation)
        : m_pCore(pCore)
        , m_transformation(std::forward<Tr>(transformation))
    {
    }

    ~RecoverCore() override = default;

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

#endif //EVENTKIT_RECOVERCORE_H
