#include "Logger.h"

bool init_log_manager(std::string filename, int level, int keepDays)
{
    if (!g_logManager->setLogFilepath(filename)) {
        return false;
    }
    g_logManager->setLogKeepDays(keepDays);
    g_logManager->setConsoleLevel(level);
    return true;
}