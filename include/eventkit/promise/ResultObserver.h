//
// Created by Masahiko Tsujita on 2019-08-15.
//

#ifndef EVENTKIT_RESULTOBSERVER_H
#define EVENTKIT_RESULTOBSERVER_H

#include <eventkit/promise/Result.h>
#include <eventkit/promise/Promise.h>

namespace ek {
namespace promise {

template <typename T, typename E>
class ResultObserver {
public:
    virtual ~ResultObserver() = default;

    virtual void handleResult(const Result <T, E>& result) = 0;

};

}
}

#include <eventkit/promise/detail/ResultObserver-inl.h>

#endif //EVENTKIT_RESULTOBSERVER_H
