//
// Created by Masahiko Tsujita on 2019/09/04.
//

#ifndef EVENTKIT_WHENALLTRANSFORMATIONPROMISECORE_H
#define EVENTKIT_WHENALLTRANSFORMATIONPROMISECORE_H

#include <tuple>
#include <eventkit/promise/Result.h>
#include <bitset>
#include <vector>
#include <mutex>
#include <algorithm>
#include <eventkit/promise/detail/PromiseCore.h>

namespace ek {
namespace promise {
namespace detail {

template <typename ...Prs>
class WhenAllTransformationPromiseCore : public PromiseCore<values_of_prmises_t < Prs...>, error_of_prmises_t<Prs...>> {
public:
    using Results = std::tuple<result_of_promise_t<Prs>...>;
    using Values = values_of_prmises_t<Prs...>;
    using Error = error_of_prmises_t<Prs...>;
    using Super = PromiseCore<Values, Error>;
    using Result = ek::promise::Result<Values, Error>;

    explicit WhenAllTransformationPromiseCore(ek::common::Allocator* pA)
        : m_values()
        , m_fulfilledPromiseFlags()
        , m_pA(pA)
        , m_intrusiveObjectMixin(deleteCallback, this)  {
    }
    
    template <size_t Index>
    void onResultAt(const std::tuple_element_t<Index, Results>& result) {
        if (result.getType() == ResultType::succeeded) {
            std::get<Index>(m_values) = result.getValue();
            m_fulfilledPromiseFlags[Index] = true;
            if (m_fulfilledPromiseFlags.all()) {
                Super::onResult(Result::succeeded(m_values));
            }
        } else {
            Super::onResult(Result::failed(result.getError()));
        }
    }

    virtual void ref() override {
        m_intrusiveObjectMixin.ref();
    }

    virtual void unref() override {
        m_intrusiveObjectMixin.unref();
    }

private:

    static void deleteCallback(ek::common::IntrusiveObjectMixin*, void* pContext) {
        auto* pThis = static_cast<WhenAllTransformationPromiseCore<Values, Error>*>(pContext);
        pThis->m_pA->destroy(pThis);
    }

private:
    Values m_values;
    std::bitset<std::tuple_size<Values>::value> m_fulfilledPromiseFlags;
    ek::common::Allocator* m_pA;
    ek::common::IntrusiveObjectMixin m_intrusiveObjectMixin;

};

template <typename T, typename E>
class DynamicWhenAllTransformationPromiseCore : public PromiseCore<std::vector<T>, E> {
public:
    using Values = std::vector<T>;
    using Error = E;
    using Super = PromiseCore<Values, Error>;
    using Results = Result<std::vector<T>, E>;

    explicit DynamicWhenAllTransformationPromiseCore(ek::common::Allocator* pA, size_t size)
        : PromiseCore<std::vector<T>, E>(pA)
        , m_values(size)
        , m_fulfilledFlags(size, false)
        , m_pA(pA)
        , m_intrusiveObjectMixin(deleteCallback, this) {
    }

    void onResultAt(const ek::promise::Result<T, E>& result, size_t index) {
        if (result.getType() == ResultType::succeeded) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_fulfilledFlags[index]) {
                return;
            }
            m_values[index] = result.getValue();
            m_fulfilledFlags[index] = true;
            if (std::find(m_fulfilledFlags.begin(), m_fulfilledFlags.end(), false) == m_fulfilledFlags.end()) {
                Super::resolve(Results::succeeded(m_values));
            }
        } else {
            std::lock_guard<std::mutex> lock(m_mutex);
            Super::resolve(Results::failed(result.getError()));
        }
    }

    virtual void ref() override {
        m_intrusiveObjectMixin.ref();
    }

    virtual void unref() override {
        m_intrusiveObjectMixin.unref();
    }

private:

    static void deleteCallback(ek::common::IntrusiveObjectMixin*, void* pContext) {
        auto* pThis = static_cast<DynamicWhenAllTransformationPromiseCore<T, E>*>(pContext);
        pThis->m_pA->destroy(pThis);
    }

private:
    std::mutex m_mutex;
    Values m_values;
    std::vector<bool> m_fulfilledFlags;
    ek::common::Allocator* m_pA;
    ek::common::IntrusiveObjectMixin m_intrusiveObjectMixin;

};

}
}
}

#endif //EVENTKIT_WHENALLTRANSFORMATIONPROMISECORE_H
