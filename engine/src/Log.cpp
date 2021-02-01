#include "Engine/Core/Debug/Log.hpp"

using namespace Engine::Core::Debug;

uint8_t Log::settings(ESetting::DISPLAY_HOUR | ESetting::DISPLAY_WITH_COLOR | ESetting::PRINT_LOG_FILE_ERROR);

bool Log::releaseLogFile(true);
std::string Log::fileLogPath(
    (std::string(LOG_FOLDER) + std::string("log_") + getDateAndTimeStrFileFormat() + std::string(".txt")).c_str());
std::ofstream Log::fileLog(fileLogPath.c_str());