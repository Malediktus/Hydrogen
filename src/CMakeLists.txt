set(HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Hydrogen.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Core/Platform.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Core/Memory.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Core/Logger.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Core/Application.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Core/Entry.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Core/Task.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Core/UUID.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Core/Base.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Core/Window.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Core/Assert.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Assets/AssetManager.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Event/EventManager.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Event/KeyCodes.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Scene/Scene.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Scene/Components.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/MacOS/MacOSWindow.hpp")

set(SOURCE_LIST
    Core/Platform.cpp
    Core/Memory.cpp
    Core/Logger.cpp
    Core/Application.cpp
    Core/Task.cpp
    Core/Window.cpp
    Core/Assert.cpp
    Assets/AssetManager.cpp
    Event/EventManager.cpp
    Scene/Scene.cpp
    Platform/MacOS/MacOSWindow.cpp)

add_library(Hydrogen ${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(Hydrogen PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(Hydrogen PRIVATE HY_EXPORT_API)
target_compile_definitions(Hydrogen PRIVATE $<$<CONFIG:Debug>:HY_DEBUG>)
target_compile_definitions(Hydrogen PRIVATE $<$<CONFIG:Release>:HY_RELEASE>)
target_compile_features(Hydrogen PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(Hydrogen PRIVATE /W4 /WX)
else()
    target_compile_options(Hydrogen PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(Hydrogen PRIVATE glfw assimp)
target_link_libraries(Hydrogen PUBLIC spdlog::spdlog glm Vortex::Vortex EnTT::EnTT)
