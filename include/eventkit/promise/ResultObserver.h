//
// Created by Masahiko Tsujita on 2019-08-15.
//

#ifndef EVENTKIT_RESULTOBSERVER_H
#define EVENTKIT_RESULTOBSERVER_H

#include <eventkit/promise/Result.h>
#include <eventkit/promise/Promise.h>
#include <eventkit/common/IntrusiveObject.h>

namespace ek {
namespace promise {

template <typename T, typename E>
class ResultObserver {
public:
    virtual ~ResultObserver() = default;

    virtual void onResult(const Result <T, E>& result) = 0;
    
    virtual void ref() = 0;
    
    virtual void unref() = 0;

};

template <typename T, typename E>
void intrusive_ptr_ref(ResultObserver<T, E>* p) {
    p->ref();
}

template <typename T, typename E>
void intrusive_ptr_unref(ResultObserver<T, E>* p) {
    p->unref();
}

}
}

#include <eventkit/promise/detail/ResultObserver-inl.h>

#endif //EVENTKIT_RESULTOBSERVER_H
