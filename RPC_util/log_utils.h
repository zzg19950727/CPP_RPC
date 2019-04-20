#ifndef LOG_UTILS_H
#define	LOG_UTILS_H

#include <syslog.h>
#include <errno.h>
#include <stdio.h>

#define log(priority, ...) syslog(priority, __VA_ARGS__)
#define log2(priority, ...) printf(__VA_ARGS__),printf("\n")
void openlog(const char *ident, const char *dir, int priority);
void closelog();

#endif	//LOG_UTILS_H

