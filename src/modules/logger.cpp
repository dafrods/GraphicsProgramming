#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

static const char* LOG_FILENAME = "output.log";

#include <format>

void loggerInit()
{
    auto logger = spdlog::basic_logger_mt(
    "default",
    LOG_FILENAME
    );
    
    logger->flush_on(spdlog::level::info);

    spdlog::set_default_logger(logger);
}

void loggerInit(const char* path)
{
    auto logger = spdlog::basic_logger_mt(
        "default",
        std::format("{}/{}",path, LOG_FILENAME)
    );
    
    logger->flush_on(spdlog::level::info);

    spdlog::set_default_logger(logger);
}