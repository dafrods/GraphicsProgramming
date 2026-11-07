#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

void loggerInit()
{
    auto logger = spdlog::basic_logger_mt(
        "default",
        "application.log"
    );
    
    logger->flush_on(spdlog::level::info);

    spdlog::set_default_logger(logger);
}