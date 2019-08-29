//
// Created by Masahiko Tsujita on 2019-08-14.
//

#ifndef EVENTKIT_RESOLVER_H
#define EVENTKIT_RESOLVER_H

#include <memory>
#include <eventkit/promise/Result.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E>
class PromiseCore;

}
}
}

namespace ek {
namespace promise {

template <typename T, typename E>
class Fulfiller;

template <typename T, typename E>
class Rejecter;

template <typename T, typename E>
class Resolver final {
private:
    using Core = detail::PromiseCore<T, E>;

public:

    explicit Resolver(const std::shared_ptr<Core>& pCore)
        : m_pCore(pCore) {
    }

    void fulfill(const T& value) const {
        m_pCore->handleResult(Result<T, E>::succeeded(value));
    }

    void reject(const E& error) const {
        m_pCore->handleResult(Result<T, E>::failed(error));
    }

    Fulfiller<T, E> fulfiller() const {
        return Fulfiller<T, E>(m_pCore);
    }

    Rejecter<T, E> rejecter() const {
        return Rejecter<T, E>(m_pCore);
    }

private:
    std::shared_ptr<Core> m_pCore;

};

template <typename T, typename E>
class Fulfiller final {
private:
    using Core = detail::PromiseCore<T, E>;

public:

    explicit Fulfiller(const std::shared_ptr<Core>& pCore)
        : m_pCore(pCore) {
    }

    void operator () (const T& value) const {
        m_pCore->handleResult(Result<T, E>::succeeded(value));
    }

private:
    std::shared_ptr<Core> m_pCore;

};

template <typename T, typename E>
class Rejecter final {
private:
    using Core = detail::PromiseCore<T, E>;

public:

    explicit Rejecter(const std::shared_ptr<Core>& pCore)
        : m_pCore(pCore) {
    }

    void operator () (const E& error) const {
        m_pCore->handleResult(Result<T, E>::failed(error));
    }

private:
    std::shared_ptr<Core> m_pCore;

};

}
}

#endif //EVENTKIT_RESOLVER_H
