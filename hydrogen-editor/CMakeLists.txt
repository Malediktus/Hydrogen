add_executable(HydrogenEditor
    include/Panels/AssetNavigatorPanel.hpp
    include/Panel.hpp
    src/EditorApp.cpp
)

target_compile_features(HydrogenEditor PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(HydrogenEditor PRIVATE /W4 /WX)
else()
    target_compile_options(HydrogenEditor PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(HydrogenEditor PRIVATE Hydrogen glfw)
target_include_directories(HydrogenEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(HydrogenEditor PRIVATE WORKING_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
