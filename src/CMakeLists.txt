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
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Math/Math.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Renderer/Buffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Renderer/Camera.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Renderer/Context.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Renderer/Framebuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Renderer/Renderbuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Renderer/RendererAPI.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Renderer/RenderWindow.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Renderer/Shader.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Renderer/Texture.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Renderer/VertexArray.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Renderer/RenderCommand.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Renderer/Renderer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLBuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLContext.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLFramebuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLRenderbuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLRendererAPI.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLShader.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLTexture.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/OpenGL/OpenGLVertexArray.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanBuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanContext.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanFramebuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanRenderbuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanShader.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanTexture.hpp"
    "${PROJECT_SOURCE_DIR}/include/Hydrogen/Platform/Vulkan/VulkanVertexArray.hpp"
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
    Renderer/Buffer.cpp
    Renderer/Context.cpp
    Renderer/Framebuffer.cpp
    Renderer/Renderbuffer.cpp
    Renderer/RendererAPI.cpp
    Renderer/RenderWindow.cpp
    Renderer/Shader.cpp
    Renderer/Texture.cpp
    Renderer/VertexArray.cpp
    Renderer/RenderCommand.cpp
    Renderer/Renderer.cpp
    Platform/OpenGL/OpenGLBuffer.cpp
    Platform/OpenGL/OpenGLContext.cpp
    Platform/OpenGL/OpenGLFramebuffer.cpp
    Platform/OpenGL/OpenGLRenderbuffer.cpp
    Platform/OpenGL/OpenGLRendererAPI.cpp
    Platform/OpenGL/OpenGLShader.cpp
    Platform/OpenGL/OpenGLTexture.cpp
    Platform/OpenGL/OpenGLVertexArray.cpp
    Platform/Vulkan/VulkanBuffer.cpp
    Platform/Vulkan/VulkanContext.cpp
    Platform/Vulkan/VulkanFramebuffer.cpp
    Platform/Vulkan/VulkanRenderbuffer.cpp
    Platform/Vulkan/VulkanRendererAPI.cpp
    Platform/Vulkan/VulkanShader.cpp
    Platform/Vulkan/VulkanTexture.cpp
    Platform/Vulkan/VulkanVertexArray.cpp
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

target_link_libraries(Hydrogen PRIVATE glfw assimp glad Vulkan::Vulkan)
target_link_libraries(Hydrogen PUBLIC spdlog::spdlog glm EnTT::EnTT TracyClient)
