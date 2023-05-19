#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <tracy/Tracy.hpp>
#include <unordered_map>

using namespace Hydrogen;

Reference<Context> Renderer::s_Context;

Renderer::Renderer(const Reference<Shader>& defaultShader, const int width, const int height, bool renderToTexture) {
    ZoneScoped;

    m_RenderToTexture = renderToTexture;
    m_Shader = defaultShader;
    m_Framebuffer = Framebuffer::Create(s_Context->GetWindow());

    if (renderToTexture) {
        m_ColorTexture = Texture2D::Create(width, height, Texture2DUsageType::Color);
        m_DepthStencilRenderbuffer = Renderbuffer::Create(width, height, RenderbufferUsageType::DepthStencil);
        m_Framebuffer->AttachColorTexture(m_ColorTexture);
        m_Framebuffer->AttachDepthStencilRenderbuffer(m_DepthStencilRenderbuffer);
    }

    m_Framebuffer->Bind();

    // Settings
    RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    RenderCommand::ConfigureStencilTesting(false, 0xFF, 0xFF, RendererAPI::StencilTestFunc::ALWAYS, 0x11, RendererAPI::StencilTestAction::KEEP,
                                           RendererAPI::StencilTestAction::KEEP, RendererAPI::StencilTestAction::KEEP);
    RenderCommand::ConfigureBlending(true, RendererAPI::BlendingFunc::SRC_ALPHA, RendererAPI::BlendingFunc::ONE_MINUS_SRC_ALPHA, RendererAPI::BlendingFunc::ONE,
                                     RendererAPI::BlendingFunc::ZERO, RendererAPI::BlendingFunc::ONE, RendererAPI::BlendingFunc::ZERO);
    RenderCommand::ConfigureDepthTesting(true, true, RendererAPI::DepthTestFunc::LESS);
    RenderCommand::ConfigureCulling(false, RendererAPI::CullingType::BACK);
    RenderCommand::SetViewport(width, height);

    m_Framebuffer->Unbind();

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
    if (m_RenderToTexture) {
        m_ColorTexture->Resize(width, height);
        m_DepthStencilRenderbuffer->Resize(width, height);
    }
    m_Framebuffer->Bind();
    RenderCommand::SetViewport(width, height);
    m_Framebuffer->Unbind();
    HY_LOG_TRACE("Resized renderer (width: {}, height: {})", width, height);
}

void Renderer::BeginFrame() {
    ZoneScoped;
    m_Framebuffer->Bind();
    RenderCommand::Clear(RendererAPI::ClearBuffer::COLOR);
    RenderCommand::Clear(RendererAPI::ClearBuffer::DEPTH);
    m_Framebuffer->Unbind();
    HY_LOG_TRACE("Began renderer frame");
}

void Renderer::EndFrame() {
    ZoneScoped;
    HY_LOG_TRACE("Ended renderer frame");
    FrameMark;
}

void Renderer::Submit(const Reference<VertexArray>& vertexArray, Reference<Camera>) {
    ZoneScoped;
    m_Framebuffer->Bind();
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
    m_Framebuffer->Unbind();
    HY_LOG_TRACE("Submited vertex array to renderer");
}
