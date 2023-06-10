project(stb_image VERSION 0.1.0
    DESCRIPTION "The hydrogen stb_image build"
    LANGUAGES C)

add_library(stb_image stb_image.c stb_image.h)
target_include_directories(stb_image PUBLIC .)
