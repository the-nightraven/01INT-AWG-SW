#include <awg/core/logger.hpp>

#include <cstdio>
#include <mutex>

namespace awg {

    namespace {
        std::mutex gLogMutex;
    }

    void Logger::info(std::string_view kTag, std::string_view message) {
        std::lock_guard<std::mutex> lock(gLogMutex);

        std::printf("[INFO] [%.*s] %.*s\n", 
            static_cast<int>(kTag.size()), kTag.data(),
            static_cast<int>(message.size()), message.data());
    }

    void Logger::warn(std::string_view kTag, std::string_view message) {
        std::lock_guard<std::mutex> lock(gLogMutex);

        std::printf("[WARN] [%.*s] %.*s\n", 
            static_cast<int>(kTag.size()), kTag.data(),
            static_cast<int>(message.size()), message.data());
    }

    void Logger::error(std::string_view kTag, std::string_view message, int retCode) {
        std::lock_guard<std::mutex> lock(gLogMutex);

        std::printf("[ERROR] [%.*s] %.*s (%d)\n", 
            static_cast<int>(kTag.size()), kTag.data(),
            static_cast<int>(message.size()), message.data(),
            retCode);
    }

    void Logger::debug(std::string_view kTag, std::string_view message, int retCode) {
#if defined (DEBUG)
        std::lock_guard<std::mutex> lock(gLogMutex);

        std::printf("[DEBUG] [%.*s] %.*s (%d)\n", 
            static_cast<int>(kTag.size()), kTag.data(),
            static_cast<int>(message.size()), message.data(),
            retCode);
#else
        (void) kTag,
        (void) message,
        (void) retCode
#endif
    }

}