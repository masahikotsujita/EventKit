//
// Created by Masahiko Tsujita on 2019-08-16.
//

#ifndef EVENTKIT_THENTRANSFORMATION_H
#define EVENTKIT_THENTRANSFORMATION_H

#include <eventkit/Result.h>
#include <eventkit/Promise.h>

namespace ek {
namespace detail {

template <typename T, typename E, typename U, typename Handler>
class ThenTransformation : public ResultHandler<T, E> {
public:
    template <typename Tr>
    explicit ThenTransformation(const std::shared_ptr<PromiseCore < U, E>>& pCore, Tr&& transformation)
        : m_pCore(pCore)
        , m_transformation(std::forward<Tr>(transformation))
    {
    }

    ~ThenTransformation() override = default;

    void handleResult (const Result<T, E>& result) override {
        if (result.getType() == ResultType::succeeded) {
            m_transformation(result.getValue()).addHandler(m_pCore);
        } else {
            m_pCore->handleResult(Result<U, E>::failed(result.getError()));
        }
    }

private:
    std::shared_ptr<PromiseCore<U, E>> m_pCore;
    Handler m_transformation;

};

}
}

#endif //EVENTKIT_THENTRANSFORMATION_H
