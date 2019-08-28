//
// Created by Masahiko Tsujita on 2019-08-16.
//

#ifndef EVENTKIT_THENTRANSFORMATION_H
#define EVENTKIT_THENTRANSFORMATION_H

#include <eventkit/promise/Result.h>
#include <eventkit/promise/Promise.h>
#include <eventkit/promise/detail/ResultHandlerWrapper.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E, typename U, typename Handler>
class ThenTransformation :
    public std::enable_shared_from_this<ThenTransformation<T, E, U, Handler>>,
    public PromiseCore<U, E>,
    public ResultHandlerWrapper<T, E> {
public:
    template <typename Tr>
    explicit ThenTransformation(Tr&& transformation)
        : m_transformation(std::forward<Tr>(transformation))
    {
    }

    ~ThenTransformation() override = default;

    virtual void onResult(const Result<T, E>& result) override {
        if (result.getType() == ResultType::succeeded) {
            m_transformation(result.getValue()).addHandler(asCore());
        } else {
            PromiseCore<U, E>::handleResult(Result<U, E>::failed(result.getError()));
        }
    }
    
    std::shared_ptr<PromiseCore<U, E>> asCore() {
        return std::static_pointer_cast<PromiseCore<U, E>>(this->shared_from_this());
    }
    
    std::shared_ptr<ResultHandler<T, E>> asHandler() {
        return std::static_pointer_cast<ResultHandlerWrapper<T, E>>(this->shared_from_this());
    }

private:
    Handler m_transformation;

};

}
}
}

#endif //EVENTKIT_THENTRANSFORMATION_H
