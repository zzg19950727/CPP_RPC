#include "log_utils.h"

void openlog(const char *ident, const char *dir, int priority) 
{
    openlog( ident, LOG_CONS | LOG_PID, priority );
}

void closelog() 
{
    ;//closelog();
}
