#include <Vortex/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>
#include <glad/glad.h>

using namespace Vortex::OpenGL;

namespace Vortex::Utils {
static GLenum StencilActionToGLenum(RendererAPI::StencilTestAction action) {
    switch (action) {
    case RendererAPI::StencilTestAction::KEEP:
        return GL_KEEP;
    case RendererAPI::StencilTestAction::ZERO:
        return GL_ZERO;
    case RendererAPI::StencilTestAction::REPLACE:
        return GL_REPLACE;
    case RendererAPI::StencilTestAction::INCR:
        return GL_INCR;
    case RendererAPI::StencilTestAction::INCR_WRAP:
        return GL_INCR_WRAP;
    case RendererAPI::StencilTestAction::DECR:
        return GL_DECR;
    case RendererAPI::StencilTestAction::DECR_WRAP:
        return GL_DECR_WRAP;
    case RendererAPI::StencilTestAction::INVERT:
        return GL_INVERT;
    default:
        VT_ASSERT(false, "Invalid action");
        return 0;
    }
}

static GLenum BlendingFuncToGLenum(RendererAPI::BlendingFunc action) {
    switch (action) {
    case RendererAPI::BlendingFunc::ZERO:
        return GL_ZERO;
    case RendererAPI::BlendingFunc::ONE:
        return GL_ONE;
    case RendererAPI::BlendingFunc::SRC_COLOR:
        return GL_SRC_COLOR;
    case RendererAPI::BlendingFunc::ONE_MINUS_SRC_COLOR:
        return GL_ONE_MINUS_SRC_COLOR;
    case RendererAPI::BlendingFunc::DST_COLOR:
        return GL_DST_COLOR;
    case RendererAPI::BlendingFunc::ONE_MINUS_DST_COLOR:
        return GL_ONE_MINUS_DST_COLOR;
    case RendererAPI::BlendingFunc::SRC_ALPHA:
        return GL_SRC_ALPHA;
    case RendererAPI::BlendingFunc::ONE_MINUS_SRC_ALPHA:
        return GL_ONE_MINUS_SRC_ALPHA;
    case RendererAPI::BlendingFunc::DST_ALPHA:
        return GL_DST_ALPHA;
    case RendererAPI::BlendingFunc::ONE_MINUS_DST_ALPHA:
        return GL_ONE_MINUS_DST_ALPHA;
    case RendererAPI::BlendingFunc::CONSTANT_COLOR:
        return GL_CONSTANT_COLOR;
    case RendererAPI::BlendingFunc::ONE_MINUS_CONSTANT_COLOR:
        return GL_ONE_MINUS_CONSTANT_COLOR;
    case RendererAPI::BlendingFunc::CONSTANT_ALPHA:
        return GL_CONSTANT_ALPHA;
    case RendererAPI::BlendingFunc::ONE_MINUS_CONSTANT_ALPHA:
        return GL_ONE_MINUS_CONSTANT_ALPHA;
    default:
        VT_ASSERT(false, "Invalid action");
        return 0;
    }
}

static GLenum PrimitiveToGLenum(VertexArray::Primitive primitive) {
    switch (primitive) {
    case VertexArray::Primitive::Points:
        return GL_POINTS;
    case VertexArray::Primitive::Lines:
        return GL_LINES;
    case VertexArray::Primitive::LineStrip:
        return GL_LINE_STRIP;
    case VertexArray::Primitive::LineLoop:
        return GL_LINE_LOOP;
    case VertexArray::Primitive::Triangles:
        return GL_TRIANGLES;
    case VertexArray::Primitive::TriangleStrip:
        return GL_TRIANGLE_STRIP;
    case VertexArray::Primitive::TriangleFan:
        return GL_TRIANGLE_FAN;
    default:
        VT_ASSERT(false, "Invalid primitive");
        return 0;
    }
}
} // namespace Vortex::Utils

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
    ZoneScoped;
    glClearColor(color.r, color.g, color.b, color.a);
    glCheckError();
    spdlog::trace("Set OpenGL clear color to ({}, {}, {}, {})", color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear(const ClearBuffer clearBuffer) {
    ZoneScoped;
    switch (clearBuffer) {
    case ClearBuffer::COLOR:
        glClear(GL_COLOR_BUFFER_BIT);
        spdlog::trace("Cleared OpenGL color buffer");
        break;
    case ClearBuffer::DEPTH:
        glClear(GL_DEPTH_BUFFER_BIT);
        spdlog::trace("Cleared OpenGL color depth");
        break;
    case ClearBuffer::STENCIL:
        glClear(GL_STENCIL_BUFFER_BIT);
        spdlog::trace("Cleared OpenGL color stencil");
        break;
    }
    glCheckError();
}

void OpenGLRendererAPI::SetViewport(const int width, const int height) {
    ZoneScoped;
    glViewport(0, 0, width, height);
    glCheckError();
    spdlog::trace("Set OpenGL viewport size to ({}, {})", width, height);
}

void OpenGLRendererAPI::ConfigureDepthTesting(const bool enable, const bool depthMask, const DepthTestFunc func) {
    ZoneScoped;
    if (enable)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    if (depthMask)
        glDepthMask(GL_TRUE);
    else
        glDepthMask(GL_FALSE);

    switch (func) {
    case DepthTestFunc::ALWAYS:
        glDepthFunc(GL_ALWAYS);
        break;
    case DepthTestFunc::NEVER:
        glDepthFunc(GL_NEVER);
        break;
    case DepthTestFunc::LESS:
        glDepthFunc(GL_LESS);
        break;
    case DepthTestFunc::EQUAL:
        glDepthFunc(GL_EQUAL);
        break;
    case DepthTestFunc::LEQUAL:
        glDepthFunc(GL_LEQUAL);
        break;
    case DepthTestFunc::GREATER:
        glDepthFunc(GL_GREATER);
        break;
    case DepthTestFunc::NOTEQUAL:
        glDepthFunc(GL_NOTEQUAL);
        break;
    case DepthTestFunc::GEQUAL:
        glDepthFunc(GL_GEQUAL);
        break;
    }

    glCheckError();
    spdlog::trace("Configured OpenGL depth testing: (enable: {}, mask: {}, func: {})", enable, depthMask, (int) func);
}

void OpenGLRendererAPI::ConfigureStencilTesting(const bool enable, const int writeMask, const int readMask, const StencilTestFunc func, const int ref,
                                                const StencilTestAction stencilFailAction, const StencilTestAction stencilPassDepthFailAction,
                                                const StencilTestAction stencilPassDepthPassAction) {
    ZoneScoped;
    if (enable)
        glEnable(GL_STENCIL_TEST);
    else
        glDisable(GL_STENCIL_TEST);

    glStencilMask(writeMask);

    switch (func) {
    case StencilTestFunc::ALWAYS:
        glStencilFunc(GL_ALWAYS, ref, readMask);
        break;
    case StencilTestFunc::NEVER:
        glStencilFunc(GL_NEVER, ref, readMask);
        break;
    case StencilTestFunc::LESS:
        glStencilFunc(GL_LESS, ref, readMask);
        break;
    case StencilTestFunc::EQUAL:
        glStencilFunc(GL_EQUAL, ref, readMask);
        break;
    case StencilTestFunc::LEQUAL:
        glStencilFunc(GL_LEQUAL, ref, readMask);
        break;
    case StencilTestFunc::GREATER:
        glStencilFunc(GL_GREATER, ref, readMask);
        break;
    case StencilTestFunc::NOTEQUAL:
        glStencilFunc(GL_NOTEQUAL, ref, readMask);
        break;
    case StencilTestFunc::GEQUAL:
        glStencilFunc(GL_GEQUAL, ref, readMask);
        break;
    }

    glStencilOp(Utils::StencilActionToGLenum(stencilFailAction), Utils::StencilActionToGLenum(stencilPassDepthFailAction),
                Utils::StencilActionToGLenum(stencilPassDepthPassAction));

    glCheckError();
    spdlog::trace(
        "Configured OpenGL stencil testing: (enable: {}, writeMask: {}, readMask: {}, ref: {}, stencilFailAction: {}, stencilPassDepthFailAction: {}, stencilPassDepthPassAction: {})",
        enable, writeMask, readMask, ref, (int) stencilFailAction, (int) stencilPassDepthFailAction, (int) stencilPassDepthPassAction);
}

void OpenGLRendererAPI::ConfigureBlending(const bool enable, const BlendingFunc blendingFunc1, const BlendingFunc blendingFunc2, const BlendingFunc blendingFuncR,
                                          const BlendingFunc blendingFuncG, const BlendingFunc blendingFuncB, const BlendingFunc blendingFuncA) {
    ZoneScoped;
    if (enable)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);

    glBlendFunc(Utils::BlendingFuncToGLenum(blendingFunc1), Utils::BlendingFuncToGLenum(blendingFunc2));
    glBlendFuncSeparate(Utils::BlendingFuncToGLenum(blendingFuncR), Utils::BlendingFuncToGLenum(blendingFuncG), Utils::BlendingFuncToGLenum(blendingFuncB),
                        Utils::BlendingFuncToGLenum(blendingFuncA));

    glCheckError();
    spdlog::trace(
        "Configured OpenGL blending testing: (enable: {}, blendingFunc1: {}, blendingFunc2: {}, blendingFuncR: {}, blendingFuncG: {}, blendingFuncB: {}, blendingFuncA: {})",
        enable, (int) blendingFunc1, (int) blendingFunc2, (int) blendingFuncR, (int) blendingFuncG, (int) blendingFuncB, (int) blendingFuncA);
}

void OpenGLRendererAPI::ConfigureCulling(const bool enable, const CullingType type) {
    ZoneScoped;
    if (enable)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    switch (type) {
    case CullingType::BACK:
        glCullFace(GL_BACK);
        break;
    case CullingType::FRONT:
        glCullFace(GL_FRONT);
        break;
    case CullingType::FRONT_AND_BACK:
        glCullFace(GL_FRONT_AND_BACK);
        break;
    }

    glCheckError();
    spdlog::trace("Configured OpenGL culling: (enable: {}, type: {})", enable, (int) type);
}

void OpenGLRendererAPI::ConfigureWireframeView(const bool enable) {
    if (enable)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OpenGLRendererAPI::ConfigureAntiAliasing(const bool enable) {
    if (enable)
        glEnable(GL_MULTISAMPLE);
    else
        glDisable(GL_MULTISAMPLE);
}

void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) {
    ZoneScoped;
    uint32_t count = indexCount == 0 ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
    glDrawElements(Utils::PrimitiveToGLenum(vertexArray->GetPrimitive()), count, GL_UNSIGNED_INT, nullptr);
    glCheckError();
    spdlog::trace("Drew OpenGL indexed vertex array (elements: {})", count);
}

void Vortex::OpenGL::CheckOpenGLError(const char* file, int line) {
    ZoneScoped;
    GLenum errorCode;
    bool failure = false;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        failure = true;
        std::string error;
        switch (errorCode) {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }
        spdlog::error("An OpenGL error of type {} occured in file {}, line {}", error, file, line);
    }

    VT_ASSERT_CHECK(!failure, "An OpenGL error occured");
}
