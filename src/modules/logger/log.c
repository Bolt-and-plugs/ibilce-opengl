#include "log.h"
#include "../../defines.h"
#include "../../app.h"
#include "../utils/utils.h"

extern App app;

log_level get_min_log_level(void) {
  if (app.debug)
    return INFO;
  return WARN;
}

void c_log(log_level level, status_code status, const char *str, ...) {
  if (level < app.min_log_level)
    return;

  time_t clk = time(NULL);

  printf("%-10s", sanitize_str(ctime(&clk) + 11));
  switch (level) {
    case ERROR:
      printf("\033[31;1;4mERROR\033[0m ");
      break;
    case WARN:
      printf("\033[93;1;2mWARN\033[0m ");
      break;
    case INFO:
      printf("\033[1;94;1mINFO\033[0m ");
      break;
    case DEBUG:
      printf("\033[1;95;4mDEBUG\033[0m ");
      break;
    case CRIT_ERROR:
      printf("\033[31;1;4mCRIT_ERROR\033[0m ");
      break;
    default:
      break;
  }

  if ((status / 100) != -1) {
    printf("| STATUS CODE: ");
  }
  switch (status / 100) {
    default:
      printf("%d - Unknown Status | ", status);
  }

  char buffer[DEFAULT_LOGGER_BUFFER_SIZE];
  va_list arg_list;
  va_start(arg_list, str);
  vsnprintf(buffer, 255, str, arg_list);
  va_end(arg_list);
  printf("%s\n", sanitize_str(buffer));
}
