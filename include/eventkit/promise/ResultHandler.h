//
// Created by Masahiko Tsujita on 2019-08-15.
//

#ifndef EVENTKIT_RESULTHANDLER_H
#define EVENTKIT_RESULTHANDLER_H

#include <eventkit/promise/Result.h>
#include <eventkit/promise/Promise.h>

namespace ek {
namespace promise {

template <typename T, typename E>
class ResultHandler {
public:
    virtual ~ResultHandler() = default;

    virtual void handleResult(const Result <T, E>& result) = 0;

};

}
}

#include <eventkit/promise/detail/ResultHandler-inl.h>

#endif //EVENTKIT_RESULTHANDLER_H
