#include "Logging.h"

bool init_log_manager(std::string filename, int keepDays, int level)
{
    logutils::LogManager *logManager = logutils::LogManager::getInstance();
    if (!logManager->setLogFilepath(filename)) {
        return false;
    }
    logManager->setLogKeepDays(keepDays);
    logManager->setLoggingLevel(level);
    return true;
}