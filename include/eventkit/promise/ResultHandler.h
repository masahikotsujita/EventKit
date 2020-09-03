//
// Created by Masahiko Tsujita on 2019-08-15.
//

#ifndef EVENTKIT_RESULTHANDLER_H
#define EVENTKIT_RESULTHANDLER_H

#include <eventkit/promise/Result.h>
#include <eventkit/common/IntrusiveObject.h>

namespace ek {
namespace promise {

template <typename T, typename E>
class ResultHandler {
public:
    virtual ~ResultHandler() = default;

    virtual void onResult(const Result <T, E>& result) = 0;
    
    virtual void ref() = 0;
    
    virtual void unref() = 0;

};

template <typename T, typename E>
void intrusive_ptr_ref(ResultHandler<T, E>* p) {
    p->ref();
}

template <typename T, typename E>
void intrusive_ptr_unref(ResultHandler<T, E>* p) {
    p->unref();
}

}
}

#include <eventkit/promise/detail/ResultHandler-inl.h>

#endif //EVENTKIT_RESULTHANDLER_H
