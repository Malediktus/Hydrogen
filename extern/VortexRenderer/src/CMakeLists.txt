set(HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/Vortex/Vortex.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Core.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Window.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Context.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/RenderCommand.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Renderer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/RendererAPI.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Shader.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Camera.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Buffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Texture.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/VertexArray.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Framebuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Renderbuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Scene.hpp")

set(SOURCE_LIST
    Context.cpp
    Shader.cpp
    Buffer.cpp
    Camera.cpp
    Texture.cpp
    VertexArray.cpp
    Renderer.cpp
    RenderCommand.cpp
    RendererAPI.cpp
    Framebuffer.cpp
    Renderbuffer.cpp
    Asserts.cpp
    Window.cpp)

set(OPENGL_HEADERS
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLShader.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLContext.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLBuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLTexture.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLVertexArray.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLRendererAPI.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLFramebuffer.hpp"
    "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL/OpenGLRenderbuffer.hpp")

set(OPENGL_SOURCES
    Platform/OpenGL/OpenGLContext.cpp
    Platform/OpenGL/OpenGLShader.cpp
    Platform/OpenGL/OpenGLBuffer.cpp
    Platform/OpenGL/OpenGLTexture.cpp
    Platform/OpenGL/OpenGLVertexArray.cpp
    Platform/OpenGL/OpenGLRendererAPI.cpp
    Platform/OpenGL/OpenGLFramebuffer.cpp
    Platform/OpenGL/OpenGLRenderbuffer.cpp)

add_library(Vortex ${HEADER_LIST} ${SOURCE_LIST} ${OPENGL_SOURCES} ${OPENGL_HEADERS})

target_include_directories(Vortex PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(Vortex PRIVATE VT_EXPORT_API)
target_compile_definitions(Vortex PRIVATE $<$<CONFIG:Debug>:VT_DEBUG>)
target_compile_definitions(Vortex PRIVATE $<$<CONFIG:Release>:VT_RELEASE>)
target_compile_features(Vortex PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(Vortex PRIVATE /W4)
else()
    target_compile_options(Vortex PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(BUILD_SHARED_LIBS)
    target_compile_definitions(Vortex PRIVATE VT_BUILD_SHARED)
endif()

target_link_libraries(Vortex PUBLIC glm TracyClient)
target_link_libraries(Vortex PRIVATE glad spdlog::spdlog)

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include/Vortex"
    PREFIX "Header Files"
    FILES ${HEADER_LIST})

source_group(
    TREE "${PROJECT_SOURCE_DIR}/src"
    PREFIX "Source Files"
    FILES ${SOURCE_LIST})

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/OpenGL"
    PREFIX "Header Files/OpenGL"
    FILES ${OPENGL_HEADERS})

source_group(
    TREE "${PROJECT_SOURCE_DIR}/src/Platform/OpenGL"
    PREFIX "Source Files/OpenGL"
    FILES ${OPENGL_SOURCES})

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include/Vortex/Platform/Vulkan"
    PREFIX "Header Files/Vulkan"
    FILES ${VULKAN_HEADERS})

source_group(
    TREE "${PROJECT_SOURCE_DIR}/src/Platform/Vulkan"
    PREFIX "Source Files/Vulkan"
    FILES ${VULKAN_SOURCES})

add_library(Vortex::Vortex ALIAS Vortex)
