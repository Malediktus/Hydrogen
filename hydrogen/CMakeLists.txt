set(HEADER_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Hydrogen.hpp"
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
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Assets/Asset.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Assets/AssetManager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Assets/ShaderAsset.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Assets/SpriteAsset.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Event/EventManager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Event/KeyCodes.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Scene/Scene.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Scene/Components.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Math/Math.hpp"
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
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/RenderCommand.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Renderer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/ShaderCompiler.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/SwapChain.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanBuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanContext.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanFramebuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanShader.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanSwapChain.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanTexture.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanVertexArray.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/MacOS/MacOSWindow.hpp")

set(SOURCE_LIST
    src/Core/Platform.cpp
    src/Core/Memory.cpp
    src/Core/Logger.cpp
    src/Core/Application.cpp
    src/Core/Task.cpp
    src/Core/Cache.cpp
    src/Core/Window.cpp
    src/Assets/AssetManager.cpp
    src/Event/EventManager.cpp
    src/Scene/Scene.cpp
    src/Renderer/Buffer.cpp
    src/Renderer/Context.cpp
    src/Renderer/Framebuffer.cpp
    src/Renderer/RenderDevice.cpp
    src/Renderer/RendererAPI.cpp
    src/Renderer/RenderWindow.cpp
    src/Renderer/Shader.cpp
    src/Renderer/Texture.cpp
    src/Renderer/VertexArray.cpp
    src/Renderer/RenderCommand.cpp
    src/Renderer/Renderer.cpp
    src/Renderer/ShaderCompiler.cpp
    src/Renderer/SwapChain.cpp
    src/Platform/Vulkan/VulkanBuffer.cpp
    src/Platform/Vulkan/VulkanContext.cpp
    src/Platform/Vulkan/VulkanFramebuffer.cpp
    src/Platform/Vulkan/VulkanRenderDevice.cpp
    src/Platform/Vulkan/VulkanRendererAPI.cpp
    src/Platform/Vulkan/VulkanShader.cpp
    src/Platform/Vulkan/VulkanSwapChain.cpp
    src/Platform/Vulkan/VulkanTexture.cpp
    src/Platform/Vulkan/VulkanVertexArray.cpp
    src/Platform/MacOS/MacOSWindow.cpp)

add_library(Hydrogen ${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(Hydrogen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(Hydrogen PRIVATE HY_EXPORT_API)
target_compile_definitions(Hydrogen PRIVATE $<$<CONFIG:Debug>:HY_DEBUG>)
target_compile_definitions(Hydrogen PRIVATE $<$<CONFIG:Release>:HY_RELEASE>)
target_compile_features(Hydrogen PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(Hydrogen PRIVATE /W4 /WX)
else()
    target_compile_options(Hydrogen PRIVATE -Wall -Wextra -Wpedantic)
endif()

set(GLSLANGLIBS
    glslang
    HLSL
    OSDependent
    OGLCompiler
    SPIRV)

target_link_libraries(Hydrogen PRIVATE glfw assimp Vulkan::Vulkan)
target_link_libraries(Hydrogen PUBLIC spdlog::spdlog stb_image imgui glm EnTT::EnTT TracyClient ${GLSLANGLIBS})
