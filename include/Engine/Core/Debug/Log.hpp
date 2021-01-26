/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "Engine/Core/Tools/Format.hpp"

#ifdef _WIN32
#define LOG_FOLDER ".\\log\\"
#elif __linux__
#define LOG_FOLDER "./log/"
#endif

/**
 * @brief Macro can by use to modify text properties.
 * @example :   cout << FBLU("I'm blue.") << endl;
 *              cout << BOLD(FBLU("I'm blue-bold.")) << endl;
 */
/* TEXT PROPERTIES */
#define RST "\033[0m"
#define BOLD(x) "\033[1m" x RST
#define ITLC(x) "\033[3m" x RST
#define UNDL(x) "\033[4m" x RST
#define INVS(x) "\033[7m" x RST // inverses foreground & background colors
#define STKTHGH(x) "\033[9m" x RST

/* FOREGROUND COLOR TEXT */
#define FBLK(x) "\033[30m" x RST
#define FRED(x) "\033[31m" x RST
#define FGRN(x) "\033[32m" x RST
#define FYEL(x) "\033[33m" x RST
#define FBLU(x) "\033[34m" x RST
#define FMAG(x) "\033[35m" x RST
#define FCYN(x) "\033[36m" x RST
#define FWHT(x) "\033[37m" x RST

/* BACKGROUND COLOR TEXT */
#define BBLK(x) "\033[40m" x RST
#define BRED(x) "\033[41m" x RST
#define BGRN(x) "\033[42m" x RST
#define BYEL(x) "\033[43m" x RST
#define BBLU(x) "\033[44m" x RST
#define BMAG(x) "\033[45m" x RST
#define BCYN(x) "\033[46m" x RST
#define BWHT(x) "\033[47m" x RST

#define FUNCT_ERROR(msg)                                                                                               \
    Engine::Core::Debug::Log::logError(                                                                                \
        Engine::Core::Tools::stringformat("%s : %d : function \"%s\" : %s", __FILE__, __LINE__, __FUNCSIG__, msg));

#define FUNCT_WARNING(msg)                                                                                             \
    Engine::Core::Debug::Log::logWarning(                                                                              \
        Engine::Core::Tools::stringformat("%s : %d : function \"%s\" : %s", __FILE__, __LINE__, __FUNCSIG__, msg));

namespace Engine::Core::Debug
{
enum ESetting : uint8_t
{
    DISPLAY_DATE = (1u << 1),           // display date befor message (2)
    DISPLAY_HOUR = (1u << 2),           // display hour befor message (3)
    DISPLAY_WITH_COLOR = (1u << 3),     // display log with color for each even
    ALWAYS_PRINT_LOG_FILE = (1u << 4),  // never destroy log file
    PRINT_LOG_FILE_WARNING = (1u << 5), // print log file if warning happend
    PRINT_LOG_FILE_ERROR = (1u << 6),   // print log file if error happend
};

class Log
{
  protected:
#pragma region static attribut

    // Bitfiled of the setting. By default is set to : DISPLAY_HOUR | DISPLAY_WITH_COLOR | PRINT_LOG_FILE_ERROR
    static uint8_t settings;

    static bool releaseLogFile; // true if log file in'st keep
    static std::string fileLogPath;
    static std::ofstream fileLog;

#pragma endregion //! static attribut

  public:
#pragma region constructor / destructor

    constexpr inline Log() noexcept = delete;

    inline ~Log() noexcept = delete;

    constexpr inline Log(const Log &other) noexcept = delete;

    constexpr inline Log(Log &&other) noexcept = delete;

    constexpr inline Log &operator=(Log const &other) noexcept = delete;

    constexpr inline Log &operator=(Log &&other) noexcept = delete;

    /**
     * @brief Create log file
     */
    static inline void logFileHeader() noexcept;

    /**
     * @brief Save log file if error happen else destroy log file
     */
    static inline void closeAndTryToCreateFile() noexcept;

#pragma endregion //! constructor/destructor

#pragma region static methods

    /**
     * @brief print message in log event
     *
     * @param os
     */
    static inline void logAddMsg(const std::string &msg) noexcept;

    /**
     * @brief display log's heading
     */
    static inline void logHeading() noexcept;

    /**
     * @brief display message in current stream
     *
     * @param msg
     */
    static inline void log(const std::string &msg) noexcept;

    /**
     * @brief display message with prefix "Error : " in current stream
     *
     * @param msg
     */
    static inline void logError(const std::string &msg) noexcept;

    /**
     * @brief display message with prefix "Warning : " in current stream
     *
     * @param msg
     */
    static inline void logWarning(const std::string &msg) noexcept;

    /**
     * @brief display message with prefix "Tips : " in current stream
     *
     * @param msg
     */
    static inline void logTips(const std::string &msg) noexcept;

    /**
     * @brief display message with elem initialized in current stream
     *
     * @param elem initialized
     */
    static inline void logInitializationStart(const std::string &elem) noexcept;

    /**
     * @brief display message with count of elem initialize in current stream
     *
     * @param elem
     * @param count in %
     */
    static inline void logInitializationStep(const std::string &elem, unsigned int count) noexcept;

    /**
     * @brief display message with elem initialized in current stream
     *
     * @param elem initialized
     */
    static inline void logInitializationEnd(const std::string &elem) noexcept;

    /**
     * @brief Add the Setting object
     *
     * @param flag : use ESetting. For multiple Setting use ESetting::DISPLAY_DATE | ESetting::DISPLAY_HOUR
     */
    static inline void addSetting(uint8_t flag) noexcept;

    /**
     * @brief Remove the Setting object
     *
     * @param flag : use ESetting. For multiple Setting use ESetting::DISPLAY_DATE | ESetting::DISPLAY_HOUR
     */
    static inline void removeSetting(uint8_t flag) noexcept;

    /**
     * @brief Retrun flag of setting in parameter
     * @param setting
     * @return bool
     */
    [[nodiscard]] static inline bool getSettingState(ESetting setting) noexcept;

    /**
     * @brief Get the Date And Time Str
     *
     * @return std::string
     */
    [[nodiscard]] static inline std::string getTimeStr(char delimitator = ':') noexcept;

    [[nodiscard]] static inline std::string getDateStr(char delimitator = '-') noexcept;

    [[nodiscard]] static inline std::string getDateAndTimeStrFileFormat() noexcept;

    [[nodiscard]] static inline std::string getDateAndTimeStr() noexcept;

#pragma endregion //! static methods
};
} // namespace Engine::Core::Debug

#include "Log.inl"