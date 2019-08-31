//
// Created by Masahiko Tsujita on 2019-08-25.
//

#ifndef EVENTKIT_RESULTHANDLERWRAPPER_H
#define EVENTKIT_RESULTHANDLERWRAPPER_H

#include <eventkit/promise/ResultHandler.h>

namespace ek {
namespace promise {
namespace detail {

/**
 * @class       ResultHandlerWrapper
 * @brief       An wrapper class of ResultHandler to avoid multiple inheritance conflict.
 */
template <typename T, typename E>
class ResultHandlerWrapper : public ResultHandler<T, E> {
public:
    virtual ~ResultHandlerWrapper() override = default;
    
    virtual void handleResult (const Result<T, E>& result) override final {
        onResult(result);
    }
    
    virtual void onResult(const Result<T, E>& result) = 0;
    
};

}
}
}

#endif //EVENTKIT_RESULTHANDLERWRAPPER_H
