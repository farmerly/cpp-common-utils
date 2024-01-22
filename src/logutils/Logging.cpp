#include "Logging.h"

bool init_log_manager(std::string filename, int keepDays, int level)
{
    if (!g_logManager->setLogFilepath(filename)) {
        return false;
    }
    g_logManager->setLogKeepDays(keepDays);
    g_logManager->setLoggingLevel(level);
    return true;
}