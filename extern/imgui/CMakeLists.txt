project(imgui VERSION 0.1.0
    DESCRIPTION "The hydrogen imgui build"
    LANGUAGES CXX)

add_library(imgui
    imconfig.h
    imgui_draw.cpp
    imgui_internal.h
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui.cpp
    imgui.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_glfw.h
    backends/imgui_impl_opengl3_loader.h
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_opengl3.h
    backends/imgui_impl_vulkan.cpp
    backends/imgui_impl_vulkan.h
)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(imgui PUBLIC cxx_std_17)
target_link_libraries(imgui PRIVATE glfw Vulkan::Vulkan)
