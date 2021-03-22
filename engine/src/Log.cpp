#include "Engine/Core/Debug/Log.hpp"

using namespace GPE;

Log* Log::m_instance = nullptr;

Log::Log()
    : fileLogPath((std::string(LOG_FOLDER) + std::string("log_") + getDateAndTimeStrFileFormat() + std::string(".txt"))
                      .c_str()),
      fileLog(fileLogPath.c_str()), logCallBack{},
      settings(ESetting::DISPLAY_HOUR | ESetting::DISPLAY_WITH_COLOR | ESetting::PRINT_LOG_FILE_ERROR),
      releaseLogFile(true)
{
    logHeading();
}

Log::~Log()
{
    closeAndTryToCreateFile();
}