#include "Engine/Core/Debug/Log.hpp"

void Engine::Core::Debug::Log::init() noexcept
{
    logHeading();
}

inline 
void Engine::Core::Debug::Log::release() noexcept
{
    fileLog.close();

    if (!getSettingState(ALWAYS_PRINT_LOG_FILE) || releaseLogFile)
        std::remove(fileLogPath.c_str());
}

inline
void Engine::Core::Debug::Log::logAddMsg (const std::string& msg) noexcept
{
    if (!fileLog.is_open())
        return;

    std::streambuf* coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(fileLog.rdbuf()); //redirect std::cout to log.txt

    std::cout << msg;

    std::cout.rdbuf(coutbuf); //reset to standard output again
}


inline
void Engine::Core::Debug::Log::logHeading () noexcept
{
    std::string msgLog;
    msgLog += "================================\n";
    msgLog += "| LOG Green Portal Engine\n";
    msgLog += "| Date : ";
    msgLog += getDateStr() + " " + getTimeStr() + "\n";
    msgLog += "================================\n\n";

    std::cout << msgLog;
    logAddMsg(msgLog);
}

inline 
void Engine::Core::Debug::Log::log (const std::string& msg) noexcept
{
    std::string msgLog;
    msgLog += "GPEngine: "; 
    msgLog += getSettingState(DISPLAY_DATE) ? getDateStr() + " " : "";
    msgLog += getSettingState(DISPLAY_HOUR) ? getTimeStr() + " " : "";
    msgLog += msg;
    msgLog += "\n";

    std::cout << msgLog;
    logAddMsg (msgLog);
}

inline 
void Engine::Core::Debug::Log::logError (const std::string& msg) noexcept
{
    std::string msgLog;
    msgLog += "GPEngine: "; 
    msgLog += getSettingState(DISPLAY_DATE) ? getDateStr() + " " : "";
    msgLog += getSettingState(DISPLAY_HOUR) ? getTimeStr() + " " : "";

    if(getSettingState(DISPLAY_WITH_COLOR))
    {
        logAddMsg (msgLog);
        std::cerr << msgLog;
        msgLog.clear();

        msgLog += FRED(BOLD("ERROR: "));
        std::cerr << msgLog;
        msgLog.clear();

        msgLog += "ERROR: ";
        logAddMsg (msgLog);
        msgLog.clear();
    }
    else
    {
        msgLog += "ERROR: ";
    }
    
    msgLog += msg;
    msgLog += "\n";

    if (getSettingState(PRINT_LOG_FILE_ERROR))
        releaseLogFile = false;

    std::cerr << msgLog;
    logAddMsg (msgLog);
}

inline 
void Engine::Core::Debug::Log::logWarning (const std::string& msg) noexcept
{
    std::string msgLog;
    msgLog += "GPEngine: "; 
    msgLog += getSettingState(DISPLAY_DATE) ? getDateStr() + " " : "";
    msgLog += getSettingState(DISPLAY_HOUR) ? getTimeStr() + " " : "";

    if(getSettingState(DISPLAY_WITH_COLOR))
    {
        logAddMsg (msgLog);
        std::cerr << msgLog;
        msgLog.clear();

        msgLog += FYEL(BOLD("WARNING: "));
        std::cerr << msgLog;
        msgLog.clear();
        
        msgLog += "WARNING: ";
        logAddMsg (msgLog);
        msgLog.clear();
    }
    else
    {
        msgLog += "WARNING: ";
    }

    msgLog += msg;
    msgLog += "\n";

    if (getSettingState(PRINT_LOG_FILE_WARNING))
        releaseLogFile = false;

    std::cerr << msgLog;
    logAddMsg (msgLog);
}

inline 
void Engine::Core::Debug::Log::logTips (const std::string& msg) noexcept
{
    std::string msgLog;
    msgLog += "GPEngine: "; 
    msgLog += getSettingState(DISPLAY_DATE) ? getDateStr() + " " : "";
    msgLog += getSettingState(DISPLAY_HOUR) ? getTimeStr() + " " : "";

    if(getSettingState(DISPLAY_WITH_COLOR))
    {
        logAddMsg (msgLog);
        std::cout << msgLog;
        msgLog.clear();

        msgLog += FGRN(BOLD("TIPS: "));
        std::cout << msgLog;
        msgLog.clear();
        
        msgLog += "TIPS: ";
        logAddMsg (msgLog);
        msgLog.clear();
    }
    else
    {
        msgLog += "TIPS: ";
    }

    msgLog += msg;
    msgLog += "\n";

    if (getSettingState(ALWAYS_PRINT_LOG_FILE))
        releaseLogFile = false;

    std::cout << msgLog;
    logAddMsg (msgLog);
}

inline 
void Engine::Core::Debug::Log::logInitializationStart (const std::string& elem) noexcept
{
    log((std::string(elem) + " being initialized...").c_str());
}

inline 
void Engine::Core::Debug::Log::logInitializationStep   (const std::string& elem, unsigned int count) noexcept
{
    log((std::string(elem) + " initialization [" + std::to_string(count) + "%]").c_str());
}

inline 
void Engine::Core::Debug::Log::logInitializationEnd   (const std::string& elem) noexcept
{
    log((std::string(elem) + " initialization completed").c_str());
}

inline
void Engine::Core::Debug::Log::addSetting(uint8_t flag) noexcept
{
    settings |= flag;
}

inline
void Engine::Core::Debug::Log::removeSetting(uint8_t flag) noexcept
{
    settings &= ~flag;
}

inline
bool Engine::Core::Debug::Log::getSettingState(ESetting setting) noexcept
{
    return settings & setting;
}

inline
std::string Engine::Core::Debug::Log::getDateAndTimeStrFileFormat() noexcept
{
    return getDateStr('_') + std::string(1, '_') + getTimeStr('_');
}

inline
std::string Engine::Core::Debug::Log::getDateAndTimeStr() noexcept
{
    return getDateStr() + std::string(1, ' ') + getTimeStr();
}

inline
std::string Engine::Core::Debug::Log::getTimeStr(char delimitator) noexcept
{
    struct tm newtime;
    time_t now = time(0);
    localtime_s(&newtime, &now);

    std::string rst;
    rst += std::to_string(newtime.tm_hour) + std::string(1, delimitator);
    rst += std::to_string(newtime.tm_min) + std::string(1, delimitator);
    rst += std::to_string(newtime.tm_sec);
    return rst;
}

inline
std::string Engine::Core::Debug::Log::getDateStr(char delimitator) noexcept
{
    struct tm newtime;
    time_t now = time(0);
    localtime_s(&newtime, &now);

    std::string rst;
    rst += std::to_string(newtime.tm_mday) + std::string(1, delimitator);
    rst += std::to_string(newtime.tm_mon + 1) + std::string(1, delimitator);
    rst += std::to_string(newtime.tm_year + 1900);
    return rst;
}