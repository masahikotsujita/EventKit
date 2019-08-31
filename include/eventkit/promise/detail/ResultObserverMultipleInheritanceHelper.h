//
// Created by Masahiko Tsujita on 2019-08-25.
//

#ifndef EVENTKIT_RESULTOBSERVERMULTIPLEINHERITANCEHELPER_H
#define EVENTKIT_RESULTOBSERVERMULTIPLEINHERITANCEHELPER_H

#include <eventkit/promise/ResultObserver.h>

namespace ek {
namespace promise {
namespace detail {

/**
 * @class       ResultObserverMultipleInheritanceHelper
 * @brief       A helper class to inherit multiple `ResultObserver`s without conflict.
 */
template <typename T, typename E>
class ResultObserverMultipleInheritanceHelper : public ResultObserver<T, E> {
public:
    virtual ~ResultObserverMultipleInheritanceHelper() override = default;
    
    virtual void handleResult (const Result<T, E>& result) override final {
        onResult(result);
    }
    
    virtual void onResult(const Result<T, E>& result) = 0;
    
};

}
}
}

#endif //EVENTKIT_RESULTOBSERVERMULTIPLEINHERITANCEHELPER_H
