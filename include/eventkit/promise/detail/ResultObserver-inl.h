//
// Created by Masahiko Tsujita on 2019/08/31.
//

#ifndef EVENTKIT_RESULTOBSERVER_INL_H
#define EVENTKIT_RESULTOBSERVER_INL_H

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E, typename Function>
class FunctionResultObserver : public ek::common::IntrusiveObject, public ResultObserver<T, E> {
public:
    template <typename F>
    explicit FunctionResultObserver(ek::common::Allocator* pA, F&& function)
        : ek::common::IntrusiveObject(pA)
        , m_function(std::forward<F>(function)) {
    }
    
    virtual ~FunctionResultObserver() override = default;
    
    virtual void onResult(const Result <T, E>& result) override {
        m_function(result);
    }
    
    virtual void ref() override {
        ek::common::IntrusiveObject::ref();
    }
    
    virtual void unref() override {
        ek::common::IntrusiveObject::unref();
    }

private:
    Function m_function;
    
};

template <typename T, typename E, typename Function>
auto make_function_observer(ek::common::Allocator* pA, Function&& function) -> ek::common::IntrusivePtr<ResultObserver < T, E>> {
    return ek::common::make_intrusive<FunctionResultObserver<T, E, std::decay_t<Function>>>(pA, pA, std::forward<Function>(function));
}

}
}
}

#endif //EVENTKIT_RESULTOBSERVER_INL_H
