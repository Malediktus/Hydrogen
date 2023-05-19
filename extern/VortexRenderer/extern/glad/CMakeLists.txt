project(glad VERSION 0.1.0
    DESCRIPTION "The vortex glad build"
    LANGUAGES C)

add_library(glad src/glad.c include/KHR/khrplatform.h include/glad/glad.h)
target_include_directories(glad PUBLIC include)
