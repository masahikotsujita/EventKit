//
// Created by Masahiko Tsujita on 2019-08-17.
//

#ifndef EVENTKIT_RECOVERTRANSFORMATION_H
#define EVENTKIT_RECOVERTRANSFORMATION_H

#include <eventkit/promise/Result.h>
#include <eventkit/promise/Promise.h>
#include <eventkit/promise/detail/ResultHandlerWrapper.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E, typename F, typename Handler>
class RecoverTransformation :
    public std::enable_shared_from_this<RecoverTransformation<T, E, F, Handler>>,
    public PromiseCore<T, F>,
    public ResultHandlerWrapper<T, E> {
public:
    template <typename Tr>
    explicit RecoverTransformation(Tr&& transformation)
        : m_transformation(std::forward<Tr>(transformation))
    {
    }

    ~RecoverTransformation() override = default;

    virtual void onResult (const Result<T, E>& result) override {
        if (result.getType() == ResultType::failed) {
            m_transformation(result.getError()).addHandler(asCore());
        } else {
            PromiseCore<T, F>::handleResult(Result<T, F>::succeeded(result.getValue()));
        }
    }
    
    std::shared_ptr<PromiseCore<T, F>> asCore() {
        return std::static_pointer_cast<PromiseCore<T, F>>(this->shared_from_this());
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

#endif //EVENTKIT_RECOVERTRANSFORMATION_H
