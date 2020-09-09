//
// Created by Masahiko Tsujita on 2019-08-17.
//

#ifndef EVENTKIT_RECOVERTRANSFORMATIONCORE_H
#define EVENTKIT_RECOVERTRANSFORMATIONCORE_H

#include <eventkit/promise/Result.h>
#include <eventkit/promise/Promise.h>
#include <eventkit/promise/detail/ResultHandlerMixin.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E, typename F, typename Handler>
class RecoverTransformationCore : public PromiseCore<T, F> {
public:
    template <typename Tr>
    explicit RecoverTransformationCore(ek::common::Allocator* pA, Tr&& transformation);

    ~RecoverTransformationCore() override = default;

    inline ek::common::IntrusivePtr<ResultHandler<T, F>> asDstResultHandler();

    inline ek::common::IntrusivePtr<ResultHandler<T, E>> asSrcResultHandler();

    inline virtual void ref() override;

    inline virtual void unref() override;

private:

    static void deleteCallback(ek::common::IntrusiveObjectMixin*, void* pContext);

    static void onSrcResultCallback(const ek::promise::Result<T, E>& result, void* pContext);

    static void onDstResultCallback(const ek::promise::Result<T, F>& result, void* pContext);

    static void refCallback(void* pContext);

    static void unrefCallback(void* pContext);

private:
    Handler m_transformation;
    ek::common::Allocator* m_pA;
    ek::common::IntrusiveObjectMixin m_intrusiveMixin;
    ek::promise::detail::ResultHandlerMixin<T, E> m_srcResultHandlerMixin;
    ek::promise::detail::ResultHandlerMixin<T, F> m_dstResultHandlerMixin;

};

template <typename T, typename E, typename F, typename Handler>
template <typename Tr>
RecoverTransformationCore<T, E, F, Handler>::RecoverTransformationCore(ek::common::Allocator* pA, Tr&& transformation)
    : PromiseCore<T, F>()
    , m_transformation(std::forward<Tr>(transformation))
    , m_pA(pA)
    , m_intrusiveMixin(deleteCallback, this)
    , m_srcResultHandlerMixin(onSrcResultCallback, refCallback, unrefCallback, this)
    , m_dstResultHandlerMixin(onDstResultCallback, refCallback, unrefCallback, this)
{
}

template <typename T, typename E, typename F, typename Handler>
common::IntrusivePtr<ResultHandler<T, F>> RecoverTransformationCore<T, E, F, Handler>::asDstResultHandler() {
    return ek::common::IntrusivePtr<ResultHandler<T, F>>(&m_dstResultHandlerMixin);
}

template <typename T, typename E, typename F, typename Handler>
common::IntrusivePtr<ResultHandler<T, E>> RecoverTransformationCore<T, E, F, Handler>::asSrcResultHandler() {
    return ek::common::IntrusivePtr<ResultHandler<T, E>>(&m_srcResultHandlerMixin);
}

template <typename T, typename E, typename F, typename Handler>
void RecoverTransformationCore<T, E, F, Handler>::ref() {
    m_intrusiveMixin.ref();
}

template <typename T, typename E, typename F, typename Handler>
void RecoverTransformationCore<T, E, F, Handler>::unref() {
    m_intrusiveMixin.unref();
}

template <typename T, typename E, typename F, typename Handler>
void RecoverTransformationCore<T, E, F, Handler>::deleteCallback(ek::common::IntrusiveObjectMixin*, void* pContext) {
    auto* pThis = static_cast<RecoverTransformationCore*>(pContext);
    pThis->m_pA->destroy(pThis);
}

template <typename T, typename E, typename F, typename Handler>
void RecoverTransformationCore<T, E, F, Handler>::onSrcResultCallback(const Result<T, E>& result, void* pContext) {
    auto* pThis = static_cast<RecoverTransformationCore*>(pContext);
    if (result.getType() == ResultType::failed) {
        pThis->m_transformation(result.getError()).done(pThis->asDstResultHandler());
    } else {
        pThis->resolve(Result<T, F>::succeeded(result.getValue()));
    }
}

template <typename T, typename E, typename F, typename Handler>
void
RecoverTransformationCore<T, E, F, Handler>::onDstResultCallback(const Result<T, F>& result, void* pContext) {
    auto* pThis = static_cast<RecoverTransformationCore*>(pContext);
    pThis->resolve(result);
}

template <typename T, typename E, typename F, typename Handler>
void RecoverTransformationCore<T, E, F, Handler>::refCallback(void* pContext) {
    auto* pThis = static_cast<RecoverTransformationCore*>(pContext);
    pThis->ref();
}

template <typename T, typename E, typename F, typename Handler>
void RecoverTransformationCore<T, E, F, Handler>::unrefCallback(void* pContext) {
    auto* pThis = static_cast<RecoverTransformationCore*>(pContext);
    pThis->unref();
}

}
}
}

#endif //EVENTKIT_RECOVERTRANSFORMATIONCORE_H
