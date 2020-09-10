//
// Created by Masahiko Tsujita on 2019-08-16.
//

#ifndef EVENTKIT_THENTRANSFORMATIONPROMISECORE_H
#define EVENTKIT_THENTRANSFORMATIONPROMISECORE_H

#include <eventkit/promise/Result.h>
#include <eventkit/promise/Promise.h>
#include <eventkit/promise/detail/ResultHandlerMixin.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E, typename U, typename Handler>
class ThenTransformationPromiseCore final : public PromiseCore<U, E> {
public:
    template <typename Tr>
    explicit ThenTransformationPromiseCore(ek::common::Allocator* pA, Tr&& transformation);

    ~ThenTransformationPromiseCore() override = default;

    inline ek::common::IntrusivePtr<ResultHandler<U, E>> asDstResultHandler();
    
    inline ek::common::IntrusivePtr<ResultHandler<T, E>> asSrcResultHandler();
    
    inline virtual void ref() override;
    
    inline virtual void unref() override;

private:

    static void deleteCallback(ek::common::IntrusiveObjectMixin*, void* pContext);

    static void onSrcResultCallback(const ek::promise::Result<T, E>& result, void* pContext);

    static void onDstResultCallback(const ek::promise::Result<U, E>& result, void* pContext);

    static void refCallback(void* pContext);

    static void unrefCallback(void* pContext);

private:
    Handler m_transformation;
    ek::common::Allocator* m_pA;
    ek::common::IntrusiveObjectMixin m_intrusiveMixin;
    ek::promise::detail::ResultHandlerMixin<T, E> m_srcResultHandlerMixin;
    ek::promise::detail::ResultHandlerMixin<U, E> m_dstResultHandlerMixin;

};

template <typename T, typename E, typename U, typename Handler>
template <typename Tr>
ThenTransformationPromiseCore<T, E, U, Handler>::ThenTransformationPromiseCore(ek::common::Allocator* pA, Tr&& transformation)
    : PromiseCore<U, E>()
    , m_transformation(std::forward<Tr>(transformation))
    , m_pA(pA)
    , m_intrusiveMixin(deleteCallback, this)
    , m_srcResultHandlerMixin(onSrcResultCallback, refCallback, unrefCallback, this)
    , m_dstResultHandlerMixin(onDstResultCallback, refCallback, unrefCallback, this)
{
}

template <typename T, typename E, typename U, typename Handler>
common::IntrusivePtr<ResultHandler<U, E>> ThenTransformationPromiseCore<T, E, U, Handler>::asDstResultHandler() {
    return ek::common::IntrusivePtr<ResultHandler<U, E>>(&m_dstResultHandlerMixin);
}

template <typename T, typename E, typename U, typename Handler>
common::IntrusivePtr<ResultHandler<T, E>> ThenTransformationPromiseCore<T, E, U, Handler>::asSrcResultHandler() {
    return ek::common::IntrusivePtr<ResultHandler<T, E>>(&m_srcResultHandlerMixin);
}

template <typename T, typename E, typename U, typename Handler>
void ThenTransformationPromiseCore<T, E, U, Handler>::ref() {
    m_intrusiveMixin.ref();
}

template <typename T, typename E, typename U, typename Handler>
void ThenTransformationPromiseCore<T, E, U, Handler>::unref() {
    m_intrusiveMixin.unref();
}

template <typename T, typename E, typename U, typename Handler>
void ThenTransformationPromiseCore<T, E, U, Handler>::deleteCallback(ek::common::IntrusiveObjectMixin*, void* pContext) {
    auto* pThis = static_cast<ThenTransformationPromiseCore*>(pContext);
    pThis->m_pA->destroy(pThis);
}

template <typename T, typename E, typename U, typename Handler>
void ThenTransformationPromiseCore<T, E, U, Handler>::onSrcResultCallback(const Result<T, E>& result, void* pContext) {
    auto* pThis = static_cast<ThenTransformationPromiseCore*>(pContext);
    if (result.getType() == ResultType::succeeded) {
        pThis->m_transformation(result.getValue()).done(pThis->asDstResultHandler());
    } else {
        pThis->resolve(Result<U, E>::failed(result.getError()));
    }
}

template <typename T, typename E, typename U, typename Handler>
void
ThenTransformationPromiseCore<T, E, U, Handler>::onDstResultCallback(const Result<U, E>& result, void* pContext) {
    auto* pThis = static_cast<ThenTransformationPromiseCore*>(pContext);
    pThis->resolve(result);
}

template <typename T, typename E, typename U, typename Handler>
void ThenTransformationPromiseCore<T, E, U, Handler>::refCallback(void* pContext) {
    auto* pThis = static_cast<ThenTransformationPromiseCore*>(pContext);
    pThis->ref();
}

template <typename T, typename E, typename U, typename Handler>
void ThenTransformationPromiseCore<T, E, U, Handler>::unrefCallback(void* pContext) {
    auto* pThis = static_cast<ThenTransformationPromiseCore*>(pContext);
    pThis->unref();
}

}
}
}

#endif //EVENTKIT_THENTRANSFORMATIONPROMISECORE_H
