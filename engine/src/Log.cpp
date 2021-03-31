#include "Engine/Core/Debug/Log.hpp"
#include <ctime>
#include <iostream>
#include <cstdio>
#include <fstream>

using namespace GPE;

Log* Log::m_instance = nullptr;

Log::Log()
    : fileLogPath((std::string(LOG_FOLDER) + std::string("log_") + getDateAndTimeStrFileFormat() + std::string(".txt"))
                      .c_str()),
      logCallBack{},
      settings(ESetting::DISPLAY_HOUR | ESetting::DISPLAY_WITH_COLOR | ESetting::PRINT_LOG_FILE_ERROR),
      releaseLogFile(true)
{
    logHeading();
}

Log::~Log()
{
    closeAndTryToCreateFile();
}


void Log::closeAndTryToCreateFile() noexcept
{
    if (!(!getSettingState(ESetting::ALWAYS_PRINT_LOG_FILE) || releaseLogFile))
    {
        std::ofstream fileLog (fileLogPath.c_str());
        if (fileLog.fail())
            return;

        for (std::string& log : logs)
        {
            fileLog << log;
        }

        fileLog.close();
    }
}

void Log::logAddMsg(const std::string& msg) noexcept
{
    if (logCallBack)
        logCallBack(msg.c_str());

    logs.emplace_back(msg);
}

void Log::logHeading() noexcept
{
    std::string msgLog;
    msgLog += "================================\n"
              "| LOG Green Portal Engine\n"
              "| Date : ";
    msgLog += getDateStr() + " " + getTimeStr() + "\n";
    msgLog += "================================\n\n";

    logAddMsg(msgLog);
}

void Log::log(const std::string& msg) noexcept
{
    std::string msgLog;
    msgLog += "GPEngine: ";
    msgLog += getSettingState(ESetting::DISPLAY_DATE) ? getDateStr() + " " : "";
    msgLog += getSettingState(ESetting::DISPLAY_HOUR) ? getTimeStr() + " " : "";
    msgLog += msg;
    msgLog += "\n";

    logAddMsg(msgLog);
}

void Log::logError(const std::string& msg) noexcept
{
    std::string msgLog;
    msgLog += "GPEngine: ";
    msgLog += getSettingState(ESetting::DISPLAY_DATE) ? getDateStr() + " " : "";
    msgLog += getSettingState(ESetting::DISPLAY_HOUR) ? getTimeStr() + " " : "";

    if (getSettingState(ESetting::DISPLAY_WITH_COLOR))
    {
        logAddMsg(msgLog);
        msgLog.clear();

        msgLog += F_RED(BOLD("ERROR: "));
        msgLog.clear();

        msgLog += "ERROR: ";
        logAddMsg(msgLog);
        msgLog.clear();
    }
    else
    {
        msgLog += "ERROR: ";
    }

    msgLog += msg;
    msgLog += "\n";

    if (getSettingState(ESetting::PRINT_LOG_FILE_ERROR))
        releaseLogFile = false;

    logAddMsg(msgLog);
}

void Log::logWarning(const std::string& msg) noexcept
{
    std::string msgLog;
    msgLog += "GPEngine: ";
    msgLog += getSettingState(ESetting::DISPLAY_DATE) ? getDateStr() + " " : "";
    msgLog += getSettingState(ESetting::DISPLAY_HOUR) ? getTimeStr() + " " : "";

    if (getSettingState(ESetting::DISPLAY_WITH_COLOR))
    {
        logAddMsg(msgLog);
        msgLog.clear();

        msgLog += F_YELLOW(BOLD("WARNING: "));
        msgLog.clear();

        msgLog += "WARNING: ";
        logAddMsg(msgLog);
        msgLog.clear();
    }
    else
    {
        msgLog += "WARNING: ";
    }

    msgLog += msg;
    msgLog += "\n";

    if (getSettingState(ESetting::PRINT_LOG_FILE_WARNING))
        releaseLogFile = false;

    logAddMsg(msgLog);
}

void Log::logTips(const std::string& msg) noexcept
{
    std::string msgLog;
    msgLog += "GPEngine: ";
    msgLog += getSettingState(ESetting::DISPLAY_DATE) ? getDateStr() + " " : "";
    msgLog += getSettingState(ESetting::DISPLAY_HOUR) ? getTimeStr() + " " : "";

    if (getSettingState(ESetting::DISPLAY_WITH_COLOR))
    {
        logAddMsg(msgLog);
        msgLog.clear();

        msgLog += F_GREEN(BOLD("TIPS: "));
        msgLog.clear();

        msgLog += "TIPS: ";
        logAddMsg(msgLog);
        msgLog.clear();
    }
    else
    {
        msgLog += "TIPS: ";
    }

    msgLog += msg;
    msgLog += "\n";

    if (getSettingState(ESetting::ALWAYS_PRINT_LOG_FILE))
        releaseLogFile = false;

    logAddMsg(msgLog);
}

std::string Log::getTimeStr(char delimitator) noexcept
{
    struct tm newtime;
    time_t    now = time(0);
    localtime_s(&newtime, &now);

    std::string rst;
    rst += std::to_string(newtime.tm_hour) + std::string(1, delimitator);
    rst += std::to_string(newtime.tm_min) + std::string(1, delimitator);
    rst += std::to_string(newtime.tm_sec);
    return rst;
}

std::string Log::getDateStr(char delimitator) noexcept
{
    struct tm newtime;
    time_t    now = time(0);
    localtime_s(&newtime, &now);

    std::string rst;
    rst += std::to_string(newtime.tm_mday) + std::string(1, delimitator);
    rst += std::to_string(newtime.tm_mon + 1) + std::string(1, delimitator);
    rst += std::to_string(newtime.tm_year + 1900);
    return rst;
}
