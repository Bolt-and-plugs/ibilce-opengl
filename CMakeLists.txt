cmake_minimum_required(VERSION 3.15)

set(project_name comp_grafica)
project(${project_name} VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

file(GLOB SRC_FILES src/*.c)
file(GLOB PROJECT_MODULES src/modules/*/*.c)
file(GLOB VENDOR vendor/*/*.c)

add_executable(${project_name}
  ${SRC_FILES}
  ${PROJECT_MODULES}
  ${VENDOR}
)

target_link_libraries(${project_name} PRIVATE
  OpenGL::GL
)

target_include_directories(${project_name} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/vendor"
)

target_compile_definitions(${project_name} PRIVATE
  BUILD_TYPE="${CMAKE_BUILD_TYPE}"
)

target_compile_options(${project_name} PRIVATE
  $<$<CONFIG:DEBUG>:-g3 -O0 -Wall -Wextra -Wpedantic -lm>
  $<$<CONFIG:RELEAE>:-lm>
)

if(NOT WIN32)
  target_compile_options(${project_name} PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address>)
  target_link_options(${project_name} PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address>)
endif()


message(STATUS "Project Name: ${project_name}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
