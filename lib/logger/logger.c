#include "logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LOG_TO_CONSOLE 1
#define BUFFER_TIME    30
#define FILEMODE       "a"

static const char *CHANNELS_MAP[] = {
    [DEV] = "dev.log",
    [PROD] = "prod.log",
};

Logger get_logger(enum CHANNELS channel)
{
	Logger logger = {.channel = channel, .filename = NULL};

	if (channel < 0 || channel >= sizeof(CHANNELS_MAP) / sizeof(CHANNELS_MAP[0]))
	{
		fprintf(stderr, "Error: invalid channel: %d\n", channel);
		abort();
	}

	if (CHANNELS_MAP[channel] == NULL)
	{
		fprintf(stderr, "Error: no filename for channel %d\n", channel);
		abort();
	}

	logger.filename = CHANNELS_MAP[channel];
	return logger;
}

static void write_log(const char *filename, const char *level, const char *message)
{
	char time_buffer[BUFFER_TIME];
	__get_time(time_buffer, sizeof(time_buffer));

	char full_message[2048];
	snprintf(full_message, sizeof(full_message), "[%s] [%s] %s", time_buffer, level, message);

#if LOG_TO_CONSOLE
	printf("%s\n", full_message);
#endif

	if (filename != NULL)
	{
		FILE *file = fopen(filename, FILEMODE);
		if (file != NULL)
		{
			fprintf(file, "%s\n", full_message);
			fclose(file);
		}
	}
}

static void log_format(const Logger *logger, const char *level, const char *format, va_list args)
{
	if (logger == NULL || logger->filename == NULL)
	{
		return;
	}

	char message[1024];
	vsnprintf(message, sizeof(message), format, args);
	write_log(logger->filename, level, message);
}

void debug(const Logger *logger, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	log_format(logger, "DEBUG", format, args);
	va_end(args);
}

void info(const Logger *logger, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	log_format(logger, "INFO", format, args);
	va_end(args);
}

void warn(const Logger *logger, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	log_format(logger, "WARN", format, args);
	va_end(args);
}

void error(const Logger *logger, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	log_format(logger, "ERROR", format, args);
	va_end(args);
}

void __get_time(char *buff, size_t size)
{
	const time_t now = time(NULL);
	const struct tm *cur = localtime(&now);

	snprintf(buff, size, "%02d.%02d.%04d | %02d:%02d:%02d", cur->tm_mday, cur->tm_mon + 1, cur->tm_year + 1900,
	         cur->tm_hour, cur->tm_min, cur->tm_sec);
}
