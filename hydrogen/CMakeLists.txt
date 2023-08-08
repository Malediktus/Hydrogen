set(BASE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Hydrogen.hpp"
)
set(CORE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Core/Platform.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Core/Memory.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Core/Logger.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Core/Application.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Core/Entry.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Core/Task.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Core/UUID.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Core/Base.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Core/Cache.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Core/Window.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Core/Assert.hpp"
)
set(ASSET_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Assets/Asset.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Assets/AssetManager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Assets/ShaderAsset.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Assets/SpriteAsset.hpp"
)
set(EVENT_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Event/EventManager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Event/KeyCodes.hpp"
)
set(SCENE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Scene/Scene.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Scene/Components.hpp"
)
set(MATH_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Math/Math.hpp"
)
set(RENDERER_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Buffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Camera.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Context.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Framebuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/RenderDevice.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/RendererAPI.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/RenderWindow.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Shader.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Texture.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/VertexArray.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Renderer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/ShaderCompiler.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/SwapChain.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/CommandBuffer.hpp"
)
set(VULKAN_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanBuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanContext.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanFramebuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanShader.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanSwapChain.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanTexture.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanVertexArray.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanCommandBuffer.hpp"
)
set(MACOS_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/MacOS/MacOSWindow.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/MacOS/MacOSEntry.hpp"
)
set(WINDOWS_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Windows/WindowsWindow.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Windows/WindowsEntry.hpp"
)
set(HEADER_LIST
    ${BASE_HEADERS}
    ${CORE_HEADERS}
    ${ASSET_HEADERS}
    ${EVENT_HEADERS}
    ${SCENE_HEADERS}
    ${MATH_HEADERS}
    ${RENDERER_HEADERS}
    ${VULKAN_HEADERS}
    ${MACOS_HEADERS}
    ${WINDOWS_HEADERS}
)

set(CORE_SOURCES
    src/Core/Platform.cpp
    src/Core/Memory.cpp
    src/Core/Logger.cpp
    src/Core/Application.cpp
    src/Core/Entry.cpp
    src/Core/Task.cpp
    src/Core/Cache.cpp
    src/Core/Window.cpp
)
set(ASSET_SOURCES
    src/Assets/AssetManager.cpp
)
set(EVENT_SOURCES
    src/Event/EventManager.cpp
)
set(SCENE_SOURCES
    src/Scene/Scene.cpp
)
set(RENDERER_SOURCES
    src/Renderer/Buffer.cpp
    src/Renderer/Context.cpp
    src/Renderer/Framebuffer.cpp
    src/Renderer/RenderDevice.cpp
    src/Renderer/RendererAPI.cpp
    src/Renderer/RenderWindow.cpp
    src/Renderer/Shader.cpp
    src/Renderer/Texture.cpp
    src/Renderer/VertexArray.cpp
    src/Renderer/Renderer.cpp
    src/Renderer/ShaderCompiler.cpp
    src/Renderer/SwapChain.cpp
    src/Renderer/CommandBuffer.cpp
)
set(VULKAN_SOURCES
    src/Platform/Vulkan/VulkanBuffer.cpp
    src/Platform/Vulkan/VulkanContext.cpp
    src/Platform/Vulkan/VulkanFramebuffer.cpp
    src/Platform/Vulkan/VulkanRenderDevice.cpp
    src/Platform/Vulkan/VulkanRendererAPI.cpp
    src/Platform/Vulkan/VulkanShader.cpp
    src/Platform/Vulkan/VulkanSwapChain.cpp
    src/Platform/Vulkan/VulkanTexture.cpp
    src/Platform/Vulkan/VulkanVertexArray.cpp
    src/Platform/Vulkan/VulkanCommandBuffer.cpp
)
set(MACOS_SOURCES
    src/Platform/MacOS/MacOSWindow.cpp
    src/Platform/MacOS/MacOSEntry.cpp
)
set(WINDOWS_SOURCES
    src/Platform/Windows/WindowsWindow.cpp
    src/Platform/Windows/WindowsEntry.cpp
)
set(SOURCE_LIST
    ${CORE_SOURCES}
    ${ASSET_SOURCES}
    ${EVENT_SOURCES}
    ${SCENE_SOURCES}
    ${RENDERER_SOURCES}
    ${VULKAN_SOURCES}
    ${MACOS_SOURCES}
    ${WINDOWS_SOURCES}
)

add_library(Hydrogen ${SOURCE_LIST} ${HEADER_LIST})

source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen"
    PREFIX "Header Files"
    FILES ${BASE_HEADERS}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen"
    PREFIX "Header Files"
    FILES ${CORE_HEADERS}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen"
    PREFIX "Header Files"
    FILES ${ASSET_HEADERS}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen"
    PREFIX "Header Files"
    FILES ${EVENT_HEADERS}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen"
    PREFIX "Header Files"
    FILES ${SCENE_HEADERS}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen"
    PREFIX "Header Files"
    FILES ${MATH_HEADERS}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen"
    PREFIX "Header Files"
    FILES ${RENDERER_HEADERS}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen"
    PREFIX "Header Files"
    FILES ${VULKAN_HEADERS}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen"
    PREFIX "Header Files"
    FILES ${MACOS_HEADERS}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen"
    PREFIX "Header Files"
    FILES ${WINDOWS_HEADERS}
)

source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PREFIX "Source Files"
    FILES ${CORE_SOURCES}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PREFIX "Source Files"
    FILES ${ASSET_SOURCES}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PREFIX "Source Files"
    FILES ${EVENT_SOURCES}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PREFIX "Source Files"
    FILES ${SCENE_SOURCES}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PREFIX "Source Files"
    FILES ${RENDERER_SOURCES}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PREFIX "Source Files"
    FILES ${VULKAN_SOURCES}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PREFIX "Source Files"
    FILES ${MACOS_SOURCES}
)
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PREFIX "Source Files"
    FILES ${WINDOWS_SOURCES}
)

target_include_directories(Hydrogen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(Hydrogen PRIVATE HY_EXPORT_API)
target_compile_definitions(Hydrogen PRIVATE $<$<CONFIG:Debug>:HY_DEBUG>)
target_compile_definitions(Hydrogen PRIVATE $<$<CONFIG:Release>:HY_RELEASE>)
target_compile_features(Hydrogen PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(Hydrogen PRIVATE /W4 /Qpar)
else()
    target_compile_options(Hydrogen PRIVATE -Wall -Wextra -Wpedantic)
endif()

set(GLSLANGLIBS
    glslang
    HLSL
    OSDependent
    OGLCompiler
    SPIRV)

target_link_libraries(Hydrogen PRIVATE glfw Vulkan::Vulkan)
target_link_libraries(Hydrogen PUBLIC spdlog::spdlog stb_image imgui glm EnTT::EnTT TracyClient ${GLSLANGLIBS})
