/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

#include <vector>
#include <functional>
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
#define RESET "\033[0m"
#define BOLD(x) "\033[1m" x RESET
#define ITALIC(x) "\033[3m" x RESET
#define UNDERLINE(x) "\033[4m" x RESET
#define INVERSE(x) "\033[7m" x RESET // inverses foreground & background colors
#define STRIKETHROUGH(x) "\033[9m" x RESET ""

/* FOREGROUND COLOR TEXT */
#define F_BLACK(x) "\033[30m" x RESET
#define F_RED(x) "\033[31m" x RESET
#define F_GREEN(x) "\033[32m" x RESET
#define F_YELLOW(x) "\033[33m" x RESET
#define F_BLUE(x) "\033[34m" x RESET
#define F_MAGENTA(x) "\033[35m" x RESET
#define F_CYAN(x) "\033[36m" x RESET
#define F_WHITE(x) "\033[37m" x RESET

/* BACKGROUND COLOR TEXT */
#define B_BLACK(x) "\033[40m" x RESET
#define B_RED(x) "\033[41m" x RESET
#define B_GREEN(x) "\033[42m" x RESET
#define B_YELLOW(x) "\033[43m" x RESET
#define B_BLUE(x) "\033[44m" x RESET
#define B_MAGENTA(x) "\033[45m" x RESET
#define B_CYAN(x) "\033[46m" x RESET
#define B_WHITE(x) "\033[47m" x RESET

#define FUNCT_ERROR(msg)                                                                                               \
    GPE::Log::getInstance()->logError(                                                                                 \
        GPE::stringFormat("%s : %d : function \"%s\" : %s", __FILE__, __LINE__, __FUNCTION__, msg));

#define FUNCT_WARNING(msg)                                                                                             \
    GPE::Log::getInstance()->logWarning(                                                                               \
        GPE::stringFormat("%s : %d : function \"%s\" : %s", __FILE__, __LINE__, __FUNCTION__, msg));

namespace GPE
{

class Log
{
public:
    enum ESetting : uint8_t
    {
        DISPLAY_DATE           = (1u << 1), // display date befor message (2)
        DISPLAY_HOUR           = (1u << 2), // display hour befor message (3)
        DISPLAY_WITH_COLOR     = (1u << 3), // display log with color for each even
        ALWAYS_PRINT_LOG_FILE  = (1u << 4), // never destroy log file
        PRINT_LOG_FILE_WARNING = (1u << 5), // print log file if warning happend
        PRINT_LOG_FILE_ERROR   = (1u << 6)  // print log file if error happend
    };

protected:
    static Log* m_instance;

protected:
    // Bitfiled of the setting. By default is set to : DISPLAY_HOUR | DISPLAY_WITH_COLOR | PRINT_LOG_FILE_ERROR
    std::string              fileLogPath;
    std::vector<std::string> logs;
    uint8_t                  settings;
    bool                     releaseLogFile; // true if log file in'st keep

protected:
    Log();

public:
    std::function<void(const char* msg)> logCallBack;

public:
    ~Log();

    /**
     * Singletons should not be cloneable.
     */
    Log(Log& other) = delete;

    /**
     * Singletons should not be assignable.
     */
    void operator=(const Log&) = delete;

    /**
     * This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */

    static Log* getInstance()
    {
        if (m_instance == nullptr)
        {
            m_instance = new Log();
        }

        return m_instance;
    }

    static void setInstance(Log& log)
    {
        m_instance = &log;
    }

    inline const std::vector<std::string>& getLogs() noexcept;

    /**
     * @brief Save log file if error happen else destroy log file
     */
    void closeAndTryToCreateFile() noexcept;

    /**
     * @brief print message in log event
     *
     * @param os
     */
    void logAddMsg(const std::string& msg) noexcept;

    /**
     * @brief display log's heading
     */
    void logHeading() noexcept;

    /**
     * @brief display message in current stream
     *
     * @param msg
     */
    void log(const std::string& msg) noexcept;

    /**
     * @brief display message with prefix "Error : " in current stream
     *
     * @param msg
     */
    void logError(const std::string& msg) noexcept;

    /**
     * @brief display message with prefix "Warning : " in current stream
     *
     * @param msg
     */
    void logWarning(const std::string& msg) noexcept;

    /**
     * @brief display message with prefix "Tips : " in current stream
     *
     * @param msg
     */
    void logTips(const std::string& msg) noexcept;

    /**
     * @brief display message with elem initialized in current stream
     *
     * @param elem initialized
     */
    inline void logInitializationStart(const std::string& elem) noexcept;

    /**
     * @brief display message with count of elem initialize in current stream
     *
     * @param elem
     * @param count in %
     */
    inline void logInitializationStep(const std::string& elem, unsigned int count) noexcept;

    /**
     * @brief display message with elem initialized in current stream
     *
     * @param elem initialized
     */
    inline void logInitializationEnd(const std::string& elem) noexcept;

    /**
     * @brief Add the Setting object
     *
     * @param flag : use ESetting. For multiple Setting use ESetting::DISPLAY_DATE | ESetting::DISPLAY_HOUR
     */
    inline void addSetting(uint8_t flag) noexcept;

    /**
     * @brief Remove the Setting object
     *
     * @param flag : use ESetting. For multiple Setting use ESetting::DISPLAY_DATE | ESetting::DISPLAY_HOUR
     */
    inline void removeSetting(uint8_t flag) noexcept;

    /**
     * @brief Retrun flag of setting in parameter
     * @param setting
     * @return bool
     */
    [[nodiscard]] inline bool getSettingState(ESetting setting) noexcept;

    /**
     * @brief Get the Date And Time Str
     *
     * @return std::string
     */
    [[nodiscard]] std::string getTimeStr(char delimitator = ':') noexcept;

    [[nodiscard]] std::string getDateStr(char delimitator = '-') noexcept;

    [[nodiscard]] inline std::string getDateAndTimeStrFileFormat() noexcept;

    [[nodiscard]] inline std::string getDateAndTimeStr() noexcept;
};

#include "Log.inl"

} // namespace GPE
