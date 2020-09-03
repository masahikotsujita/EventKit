//
// Created by Masahiko Tsujita on 2019-08-17.
//

#ifndef EVENTKIT_RECOVERTRANSFORMATIONCORE_H
#define EVENTKIT_RECOVERTRANSFORMATIONCORE_H

#include <eventkit/promise/Result.h>
#include <eventkit/promise/Promise.h>
#include <eventkit/promise/detail/ResultHandlerMultipleInheritanceHelper.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E, typename F, typename Handler>
class RecoverTransformationCore :
    public PromiseCore<T, F>,
    public ResultHandlerMultipleInheritanceHelper<T, E> {
public:
    template <typename Tr>
    explicit RecoverTransformationCore(ek::common::Allocator* pA, Tr&& transformation)
        : PromiseCore<T, F>(pA)
        , m_transformation(std::forward<Tr>(transformation))
    {
    }

    ~RecoverTransformationCore() override = default;

    virtual void onResult (result_observer_multiple_inheritance_helper_tag_t, const Result<T, E>& result) override {
        if (result.getType() == ResultType::failed) {
            m_transformation(result.getError()).done(asCore());
        } else {
            PromiseCore<T, F>::onResult(Result<T, F>::succeeded(result.getValue()));
        }
    }
    
    ek::common::IntrusivePtr<PromiseCore < T, F>> asCore() {
        return ek::common::IntrusivePtr<PromiseCore<T, F>>(static_cast<PromiseCore<T, F>*>(this));
    }
    
    ek::common::IntrusivePtr<ResultHandler< T, E>> asHandler() {
        return ek::common::IntrusivePtr<ResultHandler<T, E>>(static_cast<ResultHandlerMultipleInheritanceHelper<T, E>*>(this));
    }
    
    virtual void ref(result_observer_multiple_inheritance_helper_tag_t) override {
        PromiseCore<T, F>::ref();
    }
    
    virtual void unref(result_observer_multiple_inheritance_helper_tag_t) override {
        PromiseCore<T, F>::unref();
    }

private:
    Handler m_transformation;

};

}
}
}

#endif //EVENTKIT_RECOVERTRANSFORMATIONCORE_H
