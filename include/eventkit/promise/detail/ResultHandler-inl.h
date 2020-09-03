//
// Created by Masahiko Tsujita on 2019/08/31.
//

#ifndef EVENTKIT_RESULTHANDLER_INL_H
#define EVENTKIT_RESULTHANDLER_INL_H

namespace ek {
namespace promise {

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

#endif //EVENTKIT_RESULTHANDLER_INL_H
