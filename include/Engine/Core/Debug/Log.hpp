/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <ctime> 
#include <cstdio>

#ifdef _WIN32
#define LOG_FOLDER ".\\log\\"
#elif  __linux__
#define LOG_FOLDER "./log/"
#endif

/**
 * @brief Macro can by use to modify text properties.
 * @example :   cout << FBLU("I'm blue.") << endl;
 *              cout << BOLD(FBLU("I'm blue-bold.")) << endl;
 */
 /* TEXT PROPERTIES */
#define RST         "\033[0m"
#define BOLD(x)     "\033[1m" x RST
#define ITLC(x)     "\033[3m" x RST
#define UNDL(x)     "\033[4m" x RST
#define INVS(x)     "\033[7m" x RST //inverses foreground & background colors
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

#define functError(msg)                         \
                Engine::Core::Debug::Log::logError (        \
                (std::string(__FILE__) +                \
                ':' + std::to_string(__LINE__) +        \
                ": function \"" + __FUNCTION__ + "()" + \
                "\" : " + msg).c_str());

#define functWarning(msg)                       \
                Engine::Core::Debug::Log::logWarning (        \
                (std::string(__FILE__) +                \
                ':' + std::to_string(__LINE__) +        \
                ": function \"" + __FUNCTION__ + "()" + \
                "\" :"  + msg).c_str());

namespace Engine::Core::Debug
{    
    enum class ELogSetting
    {
        DISPLAY_DATE,             //display date befor message (2)
        DISPLAY_HOUR,             //display hour befor message (3)
        DISPLAY_WITH_COLOR,       //display log with color for each even
        ALWAY_PRINT_LOG_FILE,     //never destroy log file
        PRINT_LOG_FILE_WRNG,      //print log file if warning happend
        PRINT_LOG_FILE_ERR       //print log file if error happend
    };

    class Log
    {
        protected:

        #pragma region static attribut

        static bool displayDate;        // default false
        static bool displayHour;        // default true
        static bool displayWithColor;   // default true
        static bool printAlwayLogFile;              // default false
        static bool printLogFileIfWarningHappend;   // default false
        static bool printLogFileIfErrorHappend;     // default true

        static bool             releaseLogFile;     //true if log file in'st keep
        static std::string      fileLogPath;
        static std::ofstream    fileLog;

        #pragma endregion //! static attribut

        public:

        #pragma region constructor/destructor

        constexpr inline
        Log () noexcept 					    = delete;

        constexpr inline
        Log (const Log& other) noexcept			= delete;

        constexpr inline
        Log (Log&& other) noexcept				= delete;

        inline
        virtual ~Log () noexcept				= delete;

        constexpr inline
        Log& operator=(Log const& other) noexcept		= delete;

        constexpr inline
        Log& operator=(Log && other) noexcept			= delete;

        /**
         * @brief Create log file
         */
        static inline
        void init() noexcept;

        /**
         * @brief Save log file if error happen else destroy log file
         */
        static inline 
        void release() noexcept;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        /**
         * @brief print message in log event
         * 
         * @param os 
         */
        static inline
        void logAddMsg (const std::string& msg) noexcept;

        /**
         * @brief display log's heading
         */
        static inline 
        void logHeading () noexcept;

        /**
         * @brief display message in current stream
         * 
         * @param msg 
         */
        static inline 
        void log (const std::string& msg) noexcept;

        /**
         * @brief display message with prefix "Error : " in current stream
         * 
         * @param msg 
         */
        static inline 
        void logError (const std::string& msg) noexcept;

        /**
         * @brief display message with prefix "Warning : " in current stream
         * 
         * @param msg 
         */
        static inline 
        void logWarning (const std::string& msg) noexcept;

        /**
         * @brief display message with prefix "Help : " in current stream
         * 
         * @param msg 
         */
        static inline 
        void logHelp (const std::string& msg) noexcept;

        /**
         * @brief display message with elem initialized in current stream
         * 
         * @param elem initialized 
         */
        static inline 
        void logInitializationStart (const std::string& elem) noexcept;

        /**
         * @brief display message with count of elem initialize in current stream
         * 
         * @param elem 
         * @param count in %
         */
        static inline 
        void logInitializationStep   (const std::string& elem, unsigned int count) noexcept;

        /**
         * @brief display message with elem initialized in current stream
         * 
         * @param elem initialized 
         */
        static inline 
        void logInitializationEnd   (const std::string& elem) noexcept;

        /**
         * @brief Set the Setting object
         * 
         * @param setting
         *         
         * @param data 
         */
        static inline
        void setSetting	(ELogSetting setting, bool data) noexcept;

        /**
         * @brief Get the Setting object
         * 
         * @param setting 
         * @return true 
         * @return false 
         */
        [[nodiscard]] static inline 
        bool getSetting	(ELogSetting setting) noexcept;

        /**
         * @brief Get the Date And Time Str
         * 
         * @return std::string 
         */        
        [[nodiscard]] static inline 
        std::string getTimeStr(char delimitator = ':') noexcept;

        [[nodiscard]] static inline 
        std::string getDateStr(char delimitator = '-') noexcept;

        [[nodiscard]] static inline
        std::string getDateAndTimeStrFileFormat() noexcept;

        [[nodiscard]] static inline 
        std::string getDateAndTimeStr() noexcept;

        #pragma endregion //!static methods
    };
} // namespace Engine::Core::Debug

#include "Log.inl"