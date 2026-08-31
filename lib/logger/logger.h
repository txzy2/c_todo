#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

#include <stddef.h>

enum CHANNELS
{
	CHANNEL_UNKNOWN = 0,
	DEV = 1,
	PROD = 2,
	CHANNEL_MAX
};

typedef struct
{
	enum CHANNELS channel;
	const char *filename;
} Logger;

extern Logger logger;

Logger get_logger(enum CHANNELS channel);

void __get_time(char *buff, size_t size);

void debug(const Logger *logger, const char *format, ...);
void info(const Logger *logger, const char *format, ...);
void warn(const Logger *logger, const char *format, ...);
void error(const Logger *logger, const char *format, ...);

#endif // LOGGER_LOGGER_H
