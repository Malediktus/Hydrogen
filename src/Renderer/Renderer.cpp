#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <tracy/Tracy.hpp>
#include <unordered_map>

using namespace Hydrogen;

Reference<Context> Renderer::s_Context;

Renderer::Renderer(const Reference<Shader>& defaultShader, const int width, const int height) : m_Shader(defaultShader) {
    ZoneScoped;

    // Settings
    RenderCommand::SetClearColor({1.0f, 0.0f, 1.0f, 1.0f});
    RenderCommand::ConfigureStencilTesting(false, 0xFF, 0xFF, RendererAPI::StencilTestFunc::ALWAYS, 0x11, RendererAPI::StencilTestAction::KEEP,
                                           RendererAPI::StencilTestAction::KEEP, RendererAPI::StencilTestAction::KEEP);
    RenderCommand::ConfigureBlending(true, RendererAPI::BlendingFunc::SRC_ALPHA, RendererAPI::BlendingFunc::ONE_MINUS_SRC_ALPHA, RendererAPI::BlendingFunc::ONE,
                                     RendererAPI::BlendingFunc::ZERO, RendererAPI::BlendingFunc::ONE, RendererAPI::BlendingFunc::ZERO);
    RenderCommand::ConfigureDepthTesting(true, true, RendererAPI::DepthTestFunc::LESS);
    RenderCommand::ConfigureCulling(false, RendererAPI::CullingType::BACK);
    RenderCommand::SetViewport(width, height);

    HY_LOG_INFO("Initialized renderer");
}

void Renderer::SetContext(Reference<Context> context) {
    s_Context = context;
}

Reference<Context> Renderer::GetContext() {
    return s_Context;
}

void Renderer::OnResize(const int width, const int height) {
    ZoneScoped;
    RenderCommand::SetViewport(width, height);
    HY_LOG_TRACE("Resized renderer (width: {}, height: {})", width, height);
}

void Renderer::BeginFrame(const Reference<Camera>& camera) {
    ZoneScoped;
    RenderCommand::Clear(RendererAPI::ClearBuffer::COLOR);
    RenderCommand::Clear(RendererAPI::ClearBuffer::DEPTH);
    HY_LOG_TRACE("Began renderer frame");
    m_Framedata.FrameCamera = camera;
}

void Renderer::EndFrame() {
    ZoneScoped;
    HY_ASSERT(m_Framedata.FrameCamera, "Please run BeginFrame() before EndFrame() and provide a camera");
    HY_LOG_TRACE("Ended renderer frame");
    FrameMark;
}

void Renderer::Submit(const Reference<VertexArray>& vertexArray, const glm::mat4& transform) {
    ZoneScoped;
    m_Shader->Bind();
    m_Shader->UploadMatrix4("u_Model", transform);
    m_Shader->UploadMatrix4("u_ViewProj", m_Framedata.FrameCamera->GetViewProj());
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
    HY_LOG_TRACE("Submitted vertex array to renderer");
}

void Renderer::Submit(const Light& light, const glm::mat4& transform) {
    ZoneScoped;
    HY_LOG_TRACE("Submitted light to renderer");
}
