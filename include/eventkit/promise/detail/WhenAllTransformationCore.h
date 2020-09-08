//
// Created by Masahiko Tsujita on 2019/09/04.
//

#ifndef EVENTKIT_WHENALLTRANSFORMATIONCORE_H
#define EVENTKIT_WHENALLTRANSFORMATIONCORE_H

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
class WhenAllTransformationCore : public PromiseCore<values_of_prmises_t < Prs...>, error_of_prmises_t<Prs...>> {
public:
    using Results = std::tuple<result_of_promise_t<Prs>...>;
    using Values = values_of_prmises_t<Prs...>;
    using Error = error_of_prmises_t<Prs...>;
    using Super = PromiseCore<Values, Error>;
    using Result = ek::promise::Result<Values, Error>;

    explicit WhenAllTransformationCore(ek::common::Allocator* pA)
        : m_values()
        , m_fulfilledPromiseFlags()
        , m_pA(pA)
        , m_intrusiveMixin(deleteCallback, this)  {
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
        m_intrusiveMixin.ref();
    }

    virtual void unref() override {
        m_intrusiveMixin.unref();
    }

private:

    static void deleteCallback(ek::common::IntrusiveMixin*, void* pContext) {
        auto* pThis = static_cast<WhenAllTransformationCore<Values, Error>*>(pContext);
        pThis->m_pA->destroy(pThis);
    }

private:
    Values m_values;
    std::bitset<std::tuple_size<Values>::value> m_fulfilledPromiseFlags;
    ek::common::Allocator* m_pA;
    ek::common::IntrusiveMixin m_intrusiveMixin;

};

template <typename T, typename E>
class DynamicWhenAllTransformationCore : public PromiseCore<std::vector<T>, E> {
public:
    using Values = std::vector<T>;
    using Error = E;
    using Super = PromiseCore<Values, Error>;
    using Results = Result<std::vector<T>, E>;

    explicit DynamicWhenAllTransformationCore(ek::common::Allocator* pA, size_t size)
        : m_values(size)
        , m_fulfilledFlags(size, false)
        , m_pA(pA)
        , m_intrusiveMixin(deleteCallback, this) {
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
        m_intrusiveMixin.ref();
    }

    virtual void unref() override {
        m_intrusiveMixin.unref();
    }

private:

    static void deleteCallback(ek::common::IntrusiveMixin*, void* pContext) {
        auto* pThis = static_cast<DynamicWhenAllTransformationCore<T, E>*>(pContext);
        pThis->m_pA->destroy(pThis);
    }

private:
    std::mutex m_mutex;
    Values m_values;
    std::vector<bool> m_fulfilledFlags;
    ek::common::Allocator* m_pA;
    ek::common::IntrusiveMixin m_intrusiveMixin;

};

}
}
}

#endif //EVENTKIT_WHENALLTRANSFORMATIONCORE_H
