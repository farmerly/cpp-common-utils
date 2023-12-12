#include "Logger.h"

bool init_log_manager(std::string filename, int keepDays)
{
    if (!g_logManager->setLogFilepath(filename)) {
        return false;
    }
    g_logManager->setLogKeepDays(keepDays);
    return true;
}