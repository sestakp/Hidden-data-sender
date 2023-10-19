#define EXIT_FAILURE 1

#if defined(DEBUG) && DEBUG > 2
 #define DEBUG_PRINT(fmt, ...) fprintf(stderr, "DEBUG: %s:%d:%s():" fmt, \
    __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    fprintf(stderr, "\n");
#else
 #define DEBUG_PRINT(fmt, ...)
#endif

#if defined(DEBUG) && DEBUG > 1
 #define WARNING_PRINT(fmt, ...) fprintf(stderr, "WARNING: %s:%d:%s():" fmt, \
    __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    fprintf(stderr, "\n");
#else
 #define WARNING_PRINT(fmt, ...)
#endif

 #define ERROR_PRINT(fmt, ...) fprintf(stderr, "ERROR: %s:%d:%s():" fmt, \
    __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    fprintf(stderr, "\n"); \
    exit(EXIT_FAILURE);

#include <stdio.h>