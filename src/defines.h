#ifndef _DEFINES
#define _DEFINES

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "modules/utils/dynamic_array.h"
#include "modules/utils/vertex/vertex.h"
#include <time.h>
#include <unistd.h>
#include <math.h>

#ifdef __linux__
#include <sys/wait.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#elif _WIN32 || _WIN64
#include <gl/freeglut.h>
#include <gl/gl.h>
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#endif
