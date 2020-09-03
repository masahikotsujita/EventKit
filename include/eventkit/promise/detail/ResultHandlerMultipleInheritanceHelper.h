//
// Created by Masahiko Tsujita on 2019-08-25.
//

#ifndef EVENTKIT_RESULTHANDLERMULTIPLEINHERITANCEHELPER_H
#define EVENTKIT_RESULTHANDLERMULTIPLEINHERITANCEHELPER_H

#include <eventkit/promise/ResultHandler.h>

namespace ek {
namespace promise {
namespace detail {

struct result_observer_multiple_inheritance_helper_tag_t { explicit result_observer_multiple_inheritance_helper_tag_t() = default; };

constexpr result_observer_multiple_inheritance_helper_tag_t
    result_observer_multiple_inheritance_helper_tag = result_observer_multiple_inheritance_helper_tag_t();

/**
 * @class       ResultHandlerMultipleInheritanceHelper
 * @brief       A helper class to inherit multiple `ResultHandler`s without conflict.
 */
template <typename T, typename E>
class ResultHandlerMultipleInheritanceHelper : public ResultHandler<T, E> {
public:
    virtual ~ResultHandlerMultipleInheritanceHelper() override = default;
    
    virtual void onResult(const Result<T, E>& result) override final {
        onResult(result_observer_multiple_inheritance_helper_tag, result);
    }
    
    virtual void ref() override {
        ref(result_observer_multiple_inheritance_helper_tag);
    }
    
    virtual void unref() override {
        unref(result_observer_multiple_inheritance_helper_tag);
    }
    
    virtual void onResult(result_observer_multiple_inheritance_helper_tag_t, const Result<T, E>& result) = 0;
    
    virtual void ref(result_observer_multiple_inheritance_helper_tag_t) = 0;
    
    virtual void unref(result_observer_multiple_inheritance_helper_tag_t) = 0;
    
};

}
}
}

#endif //EVENTKIT_RESULTHANDLERMULTIPLEINHERITANCEHELPER_H
