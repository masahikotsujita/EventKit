//
// Created by Masahiko Tsujita on 2019/08/31.
//

#ifndef EVENTKIT_RESULTOBSERVER_INL_H
#define EVENTKIT_RESULTOBSERVER_INL_H

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E, typename Function>
class FunctionResultObserver : public ResultObserver<T, E> {
public:
    template <typename F>
    explicit FunctionResultObserver(F&& function)
        : m_function(std::forward<F>(function)) {
    }
    
    virtual ~FunctionResultObserver() override = default;
    
    virtual void onResult(const Result <T, E>& result) override {
        m_function(result);
    }

private:
    Function m_function;
    
};

template <typename T, typename E, typename Function>
auto make_function_observer(Function&& function) -> std::shared_ptr<ResultObserver<T, E>> {
    return std::make_shared<FunctionResultObserver<T, E, std::decay_t<Function>>>(std::forward<Function>(function));
}

}
}
}

#endif //EVENTKIT_RESULTOBSERVER_INL_H
