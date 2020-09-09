//
// Created by Tsujita Masahiko on 2020/09/08.
//

#ifndef EVENTKIT_RESULTHANDLERMIXIN_H
#define EVENTKIT_RESULTHANDLERMIXIN_H

#include <eventkit/promise/ResultHandler.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E>
class ResultHandlerMixin final : public ek::promise::ResultHandler<T, E> {
public:
    using OnResultCallback = void (*)(const Result<T, E>& result, void* pContext);
    using RefCallback = void (*)(void* pContext);
    using UnrefCallback = void (*)(void* pContext);

    explicit ResultHandlerMixin(OnResultCallback cbOnResult, RefCallback cbRef, UnrefCallback cbUnref, void* pContext);

    virtual ~ResultHandlerMixin() override = default;

    virtual void onResult(const Result <T, E>& result) override;

    virtual void ref() override;

    virtual void unref() override;

private:
    OnResultCallback m_cbOnResult;
    RefCallback m_cbRef;
    UnrefCallback m_cbUnref;
    void* m_pContext;

};

template <typename T, typename E>
ResultHandlerMixin<T, E>::ResultHandlerMixin(OnResultCallback cbOnResult, RefCallback cbRef, UnrefCallback cbUnref, void* pContext)
    : m_cbOnResult(cbOnResult)
    , m_cbRef(cbRef)
    , m_cbUnref(cbUnref)
    , m_pContext(pContext) {
}

template <typename T, typename E>
void ResultHandlerMixin<T, E>::onResult(const Result<T, E>& result) {
    m_cbOnResult(result, m_pContext);
}

template <typename T, typename E>
void ResultHandlerMixin<T, E>::ref() {
    m_cbRef(m_pContext);
}

template <typename T, typename E>
void ResultHandlerMixin<T, E>::unref() {
    m_cbUnref(m_pContext);
}

}
}
}

#endif //EVENTKIT_RESULTHANDLERMIXIN_H
