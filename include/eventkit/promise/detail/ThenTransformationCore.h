//
// Created by Masahiko Tsujita on 2019-08-16.
//

#ifndef EVENTKIT_THENTRANSFORMATIONCORE_H
#define EVENTKIT_THENTRANSFORMATIONCORE_H

#include <eventkit/promise/Result.h>
#include <eventkit/promise/Promise.h>
#include <eventkit/promise/detail/ResultObserverMultipleInheritanceHelper.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E, typename U, typename Handler>
class ThenTransformationCore :
    public PromiseCore<U, E>,
    public ResultObserverMultipleInheritanceHelper<T, E> {
public:
    template <typename Tr>
    explicit ThenTransformationCore(Tr&& transformation)
        : m_transformation(std::forward<Tr>(transformation))
    {
    }

    ~ThenTransformationCore() override = default;

    virtual void onResult(result_observer_multiple_inheritance_helper_tag_t, const Result<T, E>& result) override {
        if (result.getType() == ResultType::succeeded) {
            m_transformation(result.getValue()).pipe(asCore());
        } else {
            PromiseCore<U, E>::onResult(Result<U, E>::failed(result.getError()));
        }
    }
    
    ek::common::IntrusivePtr<PromiseCore<U, E>> asCore() {
        return ek::common::IntrusivePtr<PromiseCore<U, E>>(static_cast<PromiseCore<U, E>*>(this));
    }
    
    ek::common::IntrusivePtr<ResultObserver<T, E>> asHandler() {
        return ek::common::IntrusivePtr<ResultObserver<T, E>>(static_cast<ResultObserverMultipleInheritanceHelper<T, E>*>(this));
    }
    
    virtual void ref(result_observer_multiple_inheritance_helper_tag_t) const override {
        PromiseCore<U, E>::ref();
    }
    
    virtual void unref(result_observer_multiple_inheritance_helper_tag_t) const override {
        PromiseCore<U, E>::unref();
    }

private:
    Handler m_transformation;

};

}
}
}

#endif //EVENTKIT_THENTRANSFORMATIONCORE_H
