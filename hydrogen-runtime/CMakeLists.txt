if(MSVC)
    add_executable(HydrogenDemo WIN32 demo.cpp)
else()
    add_executable(HydrogenDemo demo.cpp)
endif()

target_compile_features(HydrogenDemo PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(HydrogenDemo PRIVATE /W4 /WX)
else()
    target_compile_options(HydrogenDemo PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_link_libraries(HydrogenDemo PRIVATE Hydrogen glfw)
target_compile_definitions(HydrogenDemo PRIVATE WORKING_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
