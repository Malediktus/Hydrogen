project(stb_image VERSION 0.1.0
    DESCRIPTION "The vortex stb_image build"
    LANGUAGES C)

add_library(stb_image stb_image.c include/stb_image/stb_image.h)
target_include_directories(stb_image PUBLIC include)
