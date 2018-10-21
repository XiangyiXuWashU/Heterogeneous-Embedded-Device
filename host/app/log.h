#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <stdlib.h>

#define LOG_INFO(format, ...)      \
    do {                           \
        printf("[%s|%s@%s,%d] " format "\n", "INFO ", __func__, __FILE__, __LINE__, ##__VA_ARGS__ ); \
    } while (0)

#define LOG_ERROR(format, ...)     \
    do {                           \
        printf("[%s|%s@%s,%d] " format "\n", "ERROR", __func__, __FILE__, __LINE__, ##__VA_ARGS__ ); \
    } while (0)

#define LOG_DEBUG(format, ...)     \
    do {                           \
        printf("[%s|%s@%s,%d] " format "\n", "DEBUG", __func__, __FILE__, __LINE__, ##__VA_ARGS__ ); \
    } while (0)

#define LOG_WARN(format, ...)      \
    do {                           \
        printf("[%s|%s@%s,%d] " format "\n", "WRAN ", __func__, __FILE__, __LINE__, ##__VA_ARGS__ ); \
    } while (0)

#define LOG_TRACE(format, ...)     \
    do {                           \
        printf("[%s|%s@%s,%d] " format "\n", "TRACE", __func__, __FILE__, __LINE__, ##__VA_ARGS__ ); \
    } while (0)

#define LOG_FATAL(format, ...)     \
    do {                           \
        printf("[%s|%s@%s,%d] " format "\n", "FATAL", __func__, __FILE__, __LINE__, ##__VA_ARGS__ ); \
        exit(1);                   \
    } while (0)

#define ASSERT(EXP)                \
    do{                            \
        if (!(EXP)) {                 \
            printf("[%s@%s,%d] ASSERT: " #EXP "\n", __func__, __FILE__, __LINE__ );                  \
            exit(1);               \
        }                          \
    } while(0)

#endif

