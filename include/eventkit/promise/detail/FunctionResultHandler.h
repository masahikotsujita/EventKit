//
// Created by Tsujita Masahiko on 2020/09/04.
//

#ifndef EVENTKIT_FUNCTIONRESULTHANDLER_H
#define EVENTKIT_FUNCTIONRESULTHANDLER_H

#include <eventkit/common/Allocator.h>
#include <eventkit/common/IntrusiveObject.h>
#include <eventkit/promise/ResultHandler.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E, typename Function>
class FunctionResultHandler : public ResultHandler<T, E> {
public:
    template <typename F>
    explicit FunctionResultHandler(ek::common::Allocator* pA, F&& function);

    virtual ~FunctionResultHandler() override = default;

    virtual void onResult(const Result<T, E>& result) override;

    virtual void ref() override;

    virtual void unref() override;

private:
    Function m_function;
    ek::common::Allocator* m_pA;
    ek::common::IntrusiveObjectMixin m_intrusiveObjectMixin;

};

}
}
}

#include <eventkit/promise/detail/FunctionResultHandler-inl.h>

#endif //EVENTKIT_FUNCTIONRESULTHANDLER_H
