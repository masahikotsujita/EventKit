//
// Created by Masahiko Tsujita on 2019-08-17.
//

#ifndef EVENTKIT_RECOVERTRANSFORMATIONPROMISECORE_H
#define EVENTKIT_RECOVERTRANSFORMATIONPROMISECORE_H

#include <eventkit/promise/Result.h>
#include <eventkit/promise/Promise.h>
#include <eventkit/promise/detail/ResultHandlerMixin.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E, typename F, typename Handler>
class RecoverTransformationPromiseCore : public PromiseCore<T, F> {
public:
    template <typename Tr>
    explicit RecoverTransformationPromiseCore(ek::common::Allocator* pA, Tr&& transformation);

    ~RecoverTransformationPromiseCore() override = default;

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
RecoverTransformationPromiseCore<T, E, F, Handler>::RecoverTransformationPromiseCore(ek::common::Allocator* pA, Tr&& transformation)
    : PromiseCore<T, F>()
    , m_transformation(std::forward<Tr>(transformation))
    , m_pA(pA)
    , m_intrusiveMixin(deleteCallback, this)
    , m_srcResultHandlerMixin(onSrcResultCallback, refCallback, unrefCallback, this)
    , m_dstResultHandlerMixin(onDstResultCallback, refCallback, unrefCallback, this)
{
}

template <typename T, typename E, typename F, typename Handler>
common::IntrusivePtr<ResultHandler<T, F>> RecoverTransformationPromiseCore<T, E, F, Handler>::asDstResultHandler() {
    return ek::common::IntrusivePtr<ResultHandler<T, F>>(&m_dstResultHandlerMixin);
}

template <typename T, typename E, typename F, typename Handler>
common::IntrusivePtr<ResultHandler<T, E>> RecoverTransformationPromiseCore<T, E, F, Handler>::asSrcResultHandler() {
    return ek::common::IntrusivePtr<ResultHandler<T, E>>(&m_srcResultHandlerMixin);
}

template <typename T, typename E, typename F, typename Handler>
void RecoverTransformationPromiseCore<T, E, F, Handler>::ref() {
    m_intrusiveMixin.ref();
}

template <typename T, typename E, typename F, typename Handler>
void RecoverTransformationPromiseCore<T, E, F, Handler>::unref() {
    m_intrusiveMixin.unref();
}

template <typename T, typename E, typename F, typename Handler>
void RecoverTransformationPromiseCore<T, E, F, Handler>::deleteCallback(ek::common::IntrusiveObjectMixin*, void* pContext) {
    auto* pThis = static_cast<RecoverTransformationPromiseCore*>(pContext);
    pThis->m_pA->destroy(pThis);
}

template <typename T, typename E, typename F, typename Handler>
void RecoverTransformationPromiseCore<T, E, F, Handler>::onSrcResultCallback(const Result<T, E>& result, void* pContext) {
    auto* pThis = static_cast<RecoverTransformationPromiseCore*>(pContext);
    if (result.getType() == ResultType::failed) {
        pThis->m_transformation(result.getError()).done(pThis->asDstResultHandler());
    } else {
        pThis->resolve(Result<T, F>::succeeded(result.getValue()));
    }
}

template <typename T, typename E, typename F, typename Handler>
void
RecoverTransformationPromiseCore<T, E, F, Handler>::onDstResultCallback(const Result<T, F>& result, void* pContext) {
    auto* pThis = static_cast<RecoverTransformationPromiseCore*>(pContext);
    pThis->resolve(result);
}

template <typename T, typename E, typename F, typename Handler>
void RecoverTransformationPromiseCore<T, E, F, Handler>::refCallback(void* pContext) {
    auto* pThis = static_cast<RecoverTransformationPromiseCore*>(pContext);
    pThis->ref();
}

template <typename T, typename E, typename F, typename Handler>
void RecoverTransformationPromiseCore<T, E, F, Handler>::unrefCallback(void* pContext) {
    auto* pThis = static_cast<RecoverTransformationPromiseCore*>(pContext);
    pThis->unref();
}

}
}
}

#endif //EVENTKIT_RECOVERTRANSFORMATIONPROMISECORE_H
