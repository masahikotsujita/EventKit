//
// Created by Masahiko Tsujita on 2019-08-17.
//

#ifndef EVENTKIT_RECOVERTRANSFORMATIONCORE_H
#define EVENTKIT_RECOVERTRANSFORMATIONCORE_H

#include <eventkit/promise/Result.h>
#include <eventkit/promise/Promise.h>
#include <eventkit/promise/detail/ResultObserverMultipleInheritanceHelper.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E, typename F, typename Handler>
class RecoverTransformationCore :
    public std::enable_shared_from_this<RecoverTransformationCore<T, E, F, Handler>>,
    public PromiseCore<T, F>,
    public ResultObserverMultipleInheritanceHelper<T, E> {
public:
    template <typename Tr>
    explicit RecoverTransformationCore(Tr&& transformation)
        : m_transformation(std::forward<Tr>(transformation))
    {
    }

    ~RecoverTransformationCore() override = default;

    virtual void onResult (result_observer_multiple_inheritance_helper_tag_t, const Result<T, E>& result) override {
        if (result.getType() == ResultType::failed) {
            m_transformation(result.getError()).pipe(asCore());
        } else {
            PromiseCore<T, F>::onResult(Result<T, F>::succeeded(result.getValue()));
        }
    }
    
    std::shared_ptr<PromiseCore<T, F>> asCore() {
        return std::static_pointer_cast<PromiseCore<T, F>>(this->shared_from_this());
    }
    
    std::shared_ptr<ResultObserver<T, E>> asHandler() {
        return std::static_pointer_cast<ResultObserverMultipleInheritanceHelper<T, E>>(this->shared_from_this());
    }

private:
    Handler m_transformation;

};

}
}
}

#endif //EVENTKIT_RECOVERTRANSFORMATIONCORE_H