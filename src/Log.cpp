#include "Engine/Core/Debug/Log.hpp"

using namespace Engine::Core::Debug;
 
bool Log::displayDate         (false);
bool Log::displayHour         (true);
bool Log::displayWithColor    (true);

bool Log::printAlwaysLogFile                 (false);           
bool Log::printLogFileOnWarning             (false);
bool Log::printLogFileOnError               (true); 

bool Log::releaseLogFile                    (true);
std::string  Log::fileLogPath             ((   std::string(LOG_FOLDER) + 
                                                std::string("log_") + 
                                                getDateAndTimeStrFileFormat() +
                                                std::string(".txt")).c_str());
std::ofstream  Log::fileLog   (fileLogPath.c_str());