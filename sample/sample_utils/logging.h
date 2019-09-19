//
// Created by Masahiko Tsujita on 2019/09/19.
//

#ifndef EVENTKIT_LOGGING_H
#define EVENTKIT_LOGGING_H

#include <mutex>
#include <iostream>

namespace sample_utils {

template <typename T>
struct StringWriter {
    template <typename S>
    static void write(S& stream, const T& value) {
        stream << value;
    }
};

template <typename S, typename T>
void writeInternal(S& stream, const T& value) {
    StringWriter<T>::write(stream, value);
}

template <typename S, typename T, typename ...Args>
void writeInternal(S& stream, const T& value, const Args& ...args) {
    StringWriter<T>::write(stream, value);
    writeInternal(stream, args...);
}

template <typename S, typename ...Args>
void write(S& stream, const Args& ...args) {
    writeInternal(stream, args...);
}

class Logger {
public:
    template <typename ...Args>
    void log(Args&& ...args) {
        std::lock_guard<std::mutex> lock(m_mutex);
        write(std::cout, std::forward<Args>(args)...);
        std::cout << std::endl;
    }

private:
    std::mutex m_mutex;
    
};

extern Logger g_sharedLogger;

}

#define LOG(...) ( sample_utils::g_sharedLogger.log(__VA_ARGS__) )

#endif //EVENTKIT_LOGGING_H
