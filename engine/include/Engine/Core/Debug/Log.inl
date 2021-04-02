#include "Engine/Core/Debug/Log.hpp"

inline const std::vector<std::string>& Log::getLogs() noexcept
{
    return logs;
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
