//
// Created by Tsujita Masahiko on 2020/09/05.
//

#ifndef EVENTKIT_BASICPROMISECORE_H
#define EVENTKIT_BASICPROMISECORE_H

#include <eventkit/promise/detail/PromiseCore.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E>
class BasicPromiseCore final : public PromiseCore<T, E> {
public:
    using Handler = ResultHandler<T, E>;

    explicit BasicPromiseCore()
        : PromiseCore<T, E>()
        , m_intrusiveObjectMixin(deleteCallback, this) {
    }

    virtual ~BasicPromiseCore() override = default;

    virtual void ref() override {
        m_intrusiveObjectMixin.ref();
    }

    virtual void unref() override {
        m_intrusiveObjectMixin.unref();
    }

private:

    static void deleteCallback(ek::common::IntrusiveObjectMixin*, void* pContext) {
        auto* pThis = static_cast<BasicPromiseCore<T, E>*>(pContext);
        delete pThis;
    }

private:
    ek::common::IntrusiveObjectMixin m_intrusiveObjectMixin;

};

}
}
}

#endif //EVENTKIT_BASICPROMISECORE_H
