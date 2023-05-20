#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <tracy/Tracy.hpp>
#include <unordered_map>

using namespace Hydrogen;

Reference<Context> Renderer::s_Context;

Renderer::Renderer(const Reference<Shader>& defaultShader, const Reference<Shader>& geometryShader, const int width, const int height)
    : m_Shader(defaultShader), m_GeometryShader(geometryShader) {
    ZoneScoped;

    m_GBuffer = Framebuffer::Create(s_Context->GetWindow());

    m_GBufferPositionTexture = Texture2D::Create(width, height, Texture2DStorageType::RGB32F);
    m_GBufferDiffuseTexture = Texture2D::Create(width, height, Texture2DStorageType::RGB32F);
    m_GBufferNormalTexture = Texture2D::Create(width, height, Texture2DStorageType::RGB32F);
    m_GBufferTexCoordTexture = Texture2D::Create(width, height, Texture2DStorageType::RGB32F);
    m_GBufferDepthTexture = Texture2D::Create(width, height, Texture2DStorageType::Depth);

    m_GBuffer->AttachColorTexture(m_GBufferPositionTexture);
    m_GBuffer->AttachColorTexture(m_GBufferDiffuseTexture);
    m_GBuffer->AttachColorTexture(m_GBufferNormalTexture);
    m_GBuffer->AttachColorTexture(m_GBufferTexCoordTexture);
    m_GBuffer->AttachDepthTexture(m_GBufferDepthTexture);

    std::vector<std::pair<FramebufferAttachment, uint32_t>> drawBuffers = {std::make_pair(FramebufferAttachment::Color, 0), std::make_pair(FramebufferAttachment::Color, 1),
                                                                           std::make_pair(FramebufferAttachment::Color, 2), std::make_pair(FramebufferAttachment::Color, 3)};
    m_GBuffer->SetDrawBuffers(drawBuffers);

    // Settings
    RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
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
    m_GBuffer->BindWrite();
    RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    RenderCommand::Clear(RendererAPI::ClearBuffer::COLOR);
    RenderCommand::Clear(RendererAPI::ClearBuffer::DEPTH);
    m_GBuffer->Unbind();
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
    m_GBuffer->BindWrite();
    m_GeometryShader->Bind();
    m_GeometryShader->UploadMatrix4("u_Model", transform);
    m_GeometryShader->UploadMatrix4("u_ViewProj", m_Framedata.FrameCamera->GetViewProj());
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
    m_GBuffer->Unbind();
    HY_LOG_TRACE("Submitted vertex array to renderer");
}

void Renderer::Submit(const Light&, const glm::mat4&) {
    ZoneScoped;
    HY_LOG_TRACE("Submitted light to renderer");
}
