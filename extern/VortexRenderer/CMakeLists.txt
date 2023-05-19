cmake_minimum_required(VERSION 3.15...3.26.2)

option(VORTEX_BUILD_DOCS ON)
option(VORTEX_BUILD_GLAD ON)
option(VORTEX_BUILD_GLM ON)
option(VORTEX_BUILD_SPDLOG ON)
option(VORTEX_BUILD_TRACY ON)
option(VORTEX_USE_TRACY ON)

project(Vortex VERSION 0.1.0
    DESCRIPTION "A cross platform renderer supported multiple rendering APIs"
    LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(VORTEX_BUILD_DOCS ON)
    set(VORTEX_BUILD_GLAD ON)
    set(VORTEX_BUILD_GLM ON)
    set(VORTEX_BUILD_SPDLOG ON)
    set(VORTEX_BUILD_TRACY ON)
    set(VORTEX_USE_TRACY OFF)

    set(CMAKE_CXX_STANDARD, C++17)
    set(CMAKE_CXX_STANDARD_REQUIRED, ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

if(VORTEX_BUILD_DOCS)
    find_package(Doxygen)

    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()
endif()

if(VORTEX_BUILD_SPDLOG)
    set(SPDLOG_BUILD_EXAMPLES OFF)
    set(SPDLOG_BUILD_TESTS OFF)
endif()

if(VORTEX_BUILD_TRACY)
    set(TRACY_BUILD_EXAMPLES OFF)
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug" AND VORTEX_USE_TRACY)
    set(TRACY_ENABLE ON)
    set(TRACY_ON_DEMAND ON)
else()
    set(TRACY_ENABLE OFF)
    set(TRACY_ON_DEMAND OFF)
endif()

if(VORTEX_BUILD_GLM)
    add_subdirectory(extern/glm)
endif()

if(VORTEX_BUILD_TRACY)
    add_subdirectory(extern/tracy)
endif()

if(VORTEX_BUILD_SPDLOG)
    add_subdirectory(extern/spdlog)
endif()

if(VORTEX_BUILD_GLAD)
    add_subdirectory(extern/glad)
endif()

add_subdirectory(src)
