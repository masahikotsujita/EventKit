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
    
    ek::common::intrusive_ptr<PromiseCore<T, F>> asCore() {
        return ek::common::intrusive_ptr<PromiseCore<T, F>>(static_cast<PromiseCore<T, F>*>(this));
    }
    
    ek::common::intrusive_ptr<ResultObserver<T, E>> asHandler() {
        return ek::common::intrusive_ptr<ResultObserver<T, E>>(static_cast<ResultObserverMultipleInheritanceHelper<T, E>*>(this));
    }

private:
    Handler m_transformation;

};

}
}
}

#endif //EVENTKIT_RECOVERTRANSFORMATIONCORE_H
