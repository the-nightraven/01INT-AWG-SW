#pragma once
#include <string_view>

namespace awg {

    class Logger  {
        public:
            static void info(std::string_view kTag, std::string_view message);
            static void warn(std::string_view kTag, std::string_view message);
            static void error(std::string_view kTag, std::string_view message, int retCode);
            static void debug(std::string_view kTag, std::string_view message, int retCode);
    };

}