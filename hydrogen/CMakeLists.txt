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
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Core/Window.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Core/Assert.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Assets/AssetManager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Event/EventManager.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Event/KeyCodes.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Scene/Scene.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Scene/Components.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Math/Math.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Buffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Camera.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Context.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Framebuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Renderbuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/RendererAPI.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/RenderWindow.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Shader.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Texture.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/VertexArray.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/RenderCommand.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Renderer/Renderer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLBuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLContext.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLFramebuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLRenderbuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLRendererAPI.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLShader.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLTexture.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLVertexArray.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanBuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanContext.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanFramebuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanRenderbuffer.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanShader.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanTexture.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanVertexArray.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Hydrogen/Platform/MacOS/MacOSWindow.hpp")

set(SOURCE_LIST
    src/Core/Platform.cpp
    src/Core/Memory.cpp
    src/Core/Logger.cpp
    src/Core/Application.cpp
    src/Core/Task.cpp
    src/Core/Window.cpp
    src/Core/Assert.cpp
    src/Assets/AssetManager.cpp
    src/Event/EventManager.cpp
    src/Scene/Scene.cpp
    src/Renderer/Buffer.cpp
    src/Renderer/Context.cpp
    src/Renderer/Framebuffer.cpp
    src/Renderer/Renderbuffer.cpp
    src/Renderer/RendererAPI.cpp
    src/Renderer/RenderWindow.cpp
    src/Renderer/Shader.cpp
    src/Renderer/Texture.cpp
    src/Renderer/VertexArray.cpp
    src/Renderer/RenderCommand.cpp
    src/Renderer/Renderer.cpp
    src/Platform/OpenGL/OpenGLBuffer.cpp
    src/Platform/OpenGL/OpenGLContext.cpp
    src/Platform/OpenGL/OpenGLFramebuffer.cpp
    src/Platform/OpenGL/OpenGLRenderbuffer.cpp
    src/Platform/OpenGL/OpenGLRendererAPI.cpp
    src/Platform/OpenGL/OpenGLShader.cpp
    src/Platform/OpenGL/OpenGLTexture.cpp
    src/Platform/OpenGL/OpenGLVertexArray.cpp
    src/Platform/Vulkan/VulkanBuffer.cpp
    src/Platform/Vulkan/VulkanContext.cpp
    src/Platform/Vulkan/VulkanFramebuffer.cpp
    src/Platform/Vulkan/VulkanRenderbuffer.cpp
    src/Platform/Vulkan/VulkanRendererAPI.cpp
    src/Platform/Vulkan/VulkanShader.cpp
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

target_link_libraries(Hydrogen PRIVATE glfw assimp glad Vulkan::Vulkan)
target_link_libraries(Hydrogen PUBLIC spdlog::spdlog imgui glm EnTT::EnTT TracyClient)
