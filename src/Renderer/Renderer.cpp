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
        m_ColorTexture = Texture2D::Create(width, height, Texture2DStorageType::RGBA8F);
        m_DepthStencilRenderbuffer = Renderbuffer::Create(width, height, RenderbufferUsageType::DepthStencil);
        m_Framebuffer->AttachColorTexture(m_ColorTexture);
        m_Framebuffer->AttachDepthStencilRenderbuffer(m_DepthStencilRenderbuffer);
    }

    m_Framebuffer->Bind();

    // Settings
    RenderCommand::SetClearColor({1.0f, 0.0f, 1.0f, 1.0f});
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

void Renderer::BeginFrame(const Reference<Camera>& camera) {
    ZoneScoped;
    HY_LOG_TRACE("Began renderer frame");
    m_Framedata.FrameCamera = camera;
}

void Renderer::EndFrame() {
    ZoneScoped;
    HY_ASSERT(m_Framedata.FrameCamera, "Please run BeginFrame() before EndFrame() and provide a camera");

    m_Framebuffer->Bind();
    RenderCommand::Clear(RendererAPI::ClearBuffer::COLOR);
    RenderCommand::Clear(RendererAPI::ClearBuffer::DEPTH);

    m_Shader->Bind();
    uint32_t numPointLights = 0, numSpotLights = 0, numDirectionalLights = 0;

    for (uint32_t i = 0; i < m_Framedata.Lights.size(); i++) {
        auto& light = m_Framedata.Lights[i];
        auto& transform = m_Framedata.LightTransforms[i];
        switch (light.Type) {
        case LightType::Point: {
            PointLight& pointLight = static_cast<PointLight&>(light);

            glm::vec3 position(transform[3][0], transform[3][1], transform[3][2]);
            m_Shader->UploadFloat3("u_PointLights[" + std::to_string(numPointLights) + "].position", position);

            m_Shader->UploadFloat("u_PointLights[" + std::to_string(numPointLights) + "].constant", pointLight.Constant);
            m_Shader->UploadFloat("u_PointLights[" + std::to_string(numPointLights) + "].linear", pointLight.Linear);
            m_Shader->UploadFloat("u_PointLights[" + std::to_string(numPointLights) + "].quadratic", pointLight.Quadratic);

            m_Shader->UploadFloat3("u_PointLights[" + std::to_string(numPointLights) + "].ambient", pointLight.Ambient);
            m_Shader->UploadFloat3("u_PointLights[" + std::to_string(numPointLights) + "].diffuse", pointLight.Diffuse);
            m_Shader->UploadFloat3("u_PointLights[" + std::to_string(numPointLights) + "].specular", pointLight.Specular);
            numPointLights++;
            break;
        }
        case LightType::Spot: {
            HY_INVOKE_ERROR("Spot lights are not yet supported");
            numSpotLights++;
            break;
        }
        case LightType::Directional: {
            HY_INVOKE_ERROR("Directional lights are not yet supported");
            numDirectionalLights++;
            break;
        }
        default: {
            HY_INVOKE_ERROR("Invalid light type");
            break;
        }
        }
    }

    m_Shader->UploadInt("u_NumPointLights", numPointLights);
    m_Shader->UploadInt("u_NumSpotLights", numSpotLights);
    m_Shader->UploadInt("u_NumDirectionalLights", numDirectionalLights);

    m_Shader->UploadFloat3("u_ViewPos", m_Framedata.FrameCamera->GetPosition());
    m_Shader->UploadMatrix4("u_ViewProj", m_Framedata.FrameCamera->GetViewProj());

    for (uint32_t i = 0; i < m_Framedata.VertexArrays.size(); i++) {
        auto& vertexArray = m_Framedata.VertexArrays[i];
        auto& transform = m_Framedata.VertexArrayTransforms[i];

        m_Shader->UploadMatrix4("u_Model", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
    m_Framebuffer->Unbind();

    m_Framedata.FrameCamera.reset();
    m_Framedata.VertexArrays.clear();
    m_Framedata.VertexArrayTransforms.clear();
    m_Framedata.Lights.clear();
    m_Framedata.LightTransforms.clear();

    HY_LOG_TRACE("Ended renderer frame");
    FrameMark;
}

void Renderer::Submit(const Reference<VertexArray>& vertexArray, const glm::mat4& transform) {
    ZoneScoped;
    m_Framedata.VertexArrays.push_back(vertexArray);
    m_Framedata.VertexArrayTransforms.push_back(transform);
    HY_LOG_TRACE("Submitted vertex array to renderer");
}

void Renderer::Submit(const Light& light, const glm::mat4& transform) {
    ZoneScoped;
    m_Framedata.Lights.push_back(light);
    m_Framedata.LightTransforms.push_back(transform);
    HY_LOG_TRACE("Submitted light to renderer");
}
