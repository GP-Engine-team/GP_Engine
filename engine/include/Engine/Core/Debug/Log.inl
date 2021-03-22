#include "Engine/Core/Debug/Log.hpp"

inline const std::vector<std::string>& Log::getLogs() noexcept
{
    return logs;
}

inline void Log::closeAndTryToCreateFile() noexcept
{
    if (!(!getSettingState(ESetting::ALWAYS_PRINT_LOG_FILE) || releaseLogFile))
        fileLog.close();
}

inline void Log::logAddMsg(const std::string& msg) noexcept
{
    if (!fileLog.is_open())
        return;

    if (logCallBack)
        logCallBack(msg.c_str());

    logs.emplace_back(msg);
    fileLog << msg;
}

inline void Log::logHeading() noexcept
{
    std::string msgLog;
    msgLog += "================================\n";
    msgLog += "| LOG Green Portal Engine\n";
    msgLog += "| Date : ";
    msgLog += getDateStr() + " " + getTimeStr() + "\n";
    msgLog += "================================\n\n";

    logAddMsg(msgLog);
}

inline void Log::log(const std::string& msg) noexcept
{
    std::string msgLog;
    msgLog += "GPEngine: ";
    msgLog += getSettingState(ESetting::DISPLAY_DATE) ? getDateStr() + " " : "";
    msgLog += getSettingState(ESetting::DISPLAY_HOUR) ? getTimeStr() + " " : "";
    msgLog += msg;
    msgLog += "\n";

    logAddMsg(msgLog);
}

inline void Log::logError(const std::string& msg) noexcept
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

inline void Log::logWarning(const std::string& msg) noexcept
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

    msgLog += "\n";

    if (getSettingState(ESetting::PRINT_LOG_FILE_WARNING))
        releaseLogFile = false;

    logAddMsg(msgLog);
}

inline void Log::logTips(const std::string& msg) noexcept
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

inline void Log::logInitializationStart(const std::string& elem) noexcept
{
    log((std::string(elem) + " being initialized...").c_str());
}

inline void Log::logInitializationStep(const std::string& elem, unsigned int count) noexcept
{
    log((std::string(elem) + " initialization [" + std::to_string(count) + "%]").c_str());
}

inline void Log::logInitializationEnd(const std::string& elem) noexcept
{
    log((std::string(elem) + " initialization completed").c_str());
}

inline void Log::addSetting(uint8_t flag) noexcept
{
    settings |= flag;
}

inline void Log::removeSetting(uint8_t flag) noexcept
{
    settings &= ~flag;
}

inline bool Log::getSettingState(Log::ESetting setting) noexcept
{
    return settings & setting;
}

inline std::string Log::getDateAndTimeStrFileFormat() noexcept
{
    return getDateStr('_') + std::string(1, '_') + getTimeStr('_');
}

inline std::string Log::getDateAndTimeStr() noexcept
{
    return getDateStr() + std::string(1, ' ') + getTimeStr();
}

inline std::string Log::getTimeStr(char delimitator) noexcept
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

inline std::string Log::getDateStr(char delimitator) noexcept
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
