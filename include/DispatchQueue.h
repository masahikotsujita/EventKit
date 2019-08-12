//
// Created by Masahiko Tsujita on 2019-08-12.
//

#ifndef EVENTKIT_DISPATCHQUEUE_H
#define EVENTKIT_DISPATCHQUEUE_H

#include <mutex>
#include <list>
#include <queue>
#include "RunLoop.h"

namespace ek {

class DispatchItem {
public:
    virtual ~DispatchItem() = default;

    virtual void run() = 0;

};

template <typename Function>
class DispatchFunctionItem : public DispatchItem {
public:

    template <typename F>
    explicit DispatchFunctionItem(F&& function)
        : m_function(std::forward<F>(function)) {
    }

    ~DispatchFunctionItem() override = default;

    void run() override {
        m_function();
    }

private:
    Function m_function;

};

class DispatchQueue {
public:

    friend class RunLoop;

    template <typename F>
    void dispatchAsync(F&& function) {
        dispatchItemAsync(std::make_shared<DispatchFunctionItem<std::decay_t<F>>>(std::forward<F>(function)));
    }

private:

    void dispatchItemAsync(const std::shared_ptr<DispatchItem>& pTask) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(pTask);
        m_pRunLoop->signal();
    }

    void fire() {
        bool isEmpty = false;
        do {
            std::shared_ptr<DispatchItem> item = nullptr;
            {
                std::lock_guard<std::mutex> lock(m_mutex);

                if (!m_queue.empty()) {
                    item = m_queue.front();
                    m_queue.pop();
                }
                isEmpty = m_queue.empty();
            }
            if (item != nullptr) {
                item->run();
            }
        } while (!isEmpty);
    }

    void setRunLoop(RunLoop* pRunLoop) {
        m_pRunLoop = pRunLoop;
    }

private:
    std::mutex m_mutex;
    std::queue<std::shared_ptr<DispatchItem>, std::list<std::shared_ptr<DispatchItem>>> m_queue;
    RunLoop* m_pRunLoop;

};

}

#endif //EVENTKIT_DISPATCHQUEUE_H
