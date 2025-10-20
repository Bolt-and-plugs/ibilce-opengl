#ifndef _LOG
#define _LOG

#include <stdarg.h>

typedef enum { DEBUG, INFO, WARN, ERROR, CRIT_ERROR } log_level;

typedef enum {
  DEFAULT_STATUS = -100,
  MEMORY_ALLOC_ERROR = 100,
  WINDOW_INITIALIZATION_ERROR = 200,
  OPENGL_LOAD_ERROR = 201,
  SHADER_LOAD_ERROR = 300,
  SHADER_COMPILE_ERROR = 301,
  SHADER_PROGRAM_CREATION_ERROR = 302,
  FILE_OPEN_FAILURE = 403,
  FILE_NOT_FOUND = 404,
  MODEL_PARSING_ERROR = 500,
  INVALID_INPUT = 600,
  INVALID_ARGS = 601,
  TEXTURE_LOADING_ERROR = 701,

} status_code;

#define DEFAULT_LOGGER_BUFFER_SIZE 4096 // bytes

// defining correct macros for logging
#define c_crit_error(s, ...)                                                   \
  c_log(CRIT_ERROR, s, ##__VA_ARGS__, NULL);                                   \
  exit(1);
#define c_error(s, ...) c_log(ERROR, s, ##__VA_ARGS__, NULL)
#define c_warn(s, ...) c_log(WARN, s, ##__VA_ARGS__, NULL)
#define c_info(...) c_log(INFO, DEFAULT_STATUS, ##__VA_ARGS__, NULL)
#define c_debug(s, ...) c_log(DEBUG, s, ##__VA_ARGS__, NULL)

log_level get_min_log_level();

// do not use c_log directly unless you are sure you need to
void c_log(log_level level, status_code status_code, const char *str, ...);

#endif
