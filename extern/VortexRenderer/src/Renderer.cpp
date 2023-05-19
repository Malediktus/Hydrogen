#include <Vortex/Renderer.hpp>
#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_access.hpp>
#include <tracy/Tracy.hpp>
#include <unordered_map>

using namespace Vortex;

std::shared_ptr<Context> Renderer::s_Context;

Renderer::Renderer(const std::shared_ptr<Shader>& defaultShader, const int width, const int height, bool renderToTexture) {
    ZoneScoped;

    m_RenderToTexture = renderToTexture;
    m_Shader = defaultShader;
    m_Framebuffer = FramebufferCreate(s_Context->GetWindow());

    if (renderToTexture) {
        m_ColorTexture = Texture2DCreate(width, height, Texture2D::Texture2DUsageType::Color);
        m_DepthStencilRenderbuffer = RenderbufferCreate(width, height, Renderbuffer::RenderbufferUsageType::DepthStencil);
        m_Framebuffer->AttachColorTexture(m_ColorTexture);
        m_Framebuffer->AttachDepthStencilRenderbuffer(m_DepthStencilRenderbuffer);
    }

    m_Framebuffer->Bind();

    // Settings
    Vortex::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    Vortex::RenderCommand::ConfigureStencilTesting(false, 0xFF, 0xFF, Vortex::RendererAPI::StencilTestFunc::ALWAYS, 0x11, Vortex::RendererAPI::StencilTestAction::KEEP,
                                                   Vortex::RendererAPI::StencilTestAction::KEEP, Vortex::RendererAPI::StencilTestAction::KEEP);
    Vortex::RenderCommand::ConfigureBlending(true, Vortex::RendererAPI::BlendingFunc::SRC_ALPHA, Vortex::RendererAPI::BlendingFunc::ONE_MINUS_SRC_ALPHA,
                                             Vortex::RendererAPI::BlendingFunc::ONE, Vortex::RendererAPI::BlendingFunc::ZERO, Vortex::RendererAPI::BlendingFunc::ONE,
                                             Vortex::RendererAPI::BlendingFunc::ZERO);
    Vortex::RenderCommand::ConfigureDepthTesting(true, true, Vortex::RendererAPI::DepthTestFunc::LESS);
    Vortex::RenderCommand::ConfigureCulling(false, Vortex::RendererAPI::CullingType::BACK);
    Vortex::RenderCommand::SetViewport(width, height);

    m_Framebuffer->Unbind();

    spdlog::info("Initialized renderer");
}

void Renderer::SetContext(std::shared_ptr<Context> context) {
    s_Context = context;
}

std::shared_ptr<Context> Renderer::GetContext() {
    return s_Context;
}

void Renderer::OnResize(const int width, const int height) {
    ZoneScoped;
    if (m_RenderToTexture) {
        m_ColorTexture->Resize(width, height);
        m_DepthStencilRenderbuffer->Resize(width, height);
    }
    m_Framebuffer->Bind();
    Vortex::RenderCommand::SetViewport(width, height);
    m_Framebuffer->Unbind();
    spdlog::trace("Resized renderer (width: {}, height: {})", width, height);
}

void Renderer::BeginFrame() {
    ZoneScoped;
    m_Framebuffer->Bind();
    Vortex::RenderCommand::Clear(Vortex::RendererAPI::ClearBuffer::COLOR);
    Vortex::RenderCommand::Clear(Vortex::RendererAPI::ClearBuffer::DEPTH);
    m_Framebuffer->Unbind();
    spdlog::trace("Began renderer frame");
}

void Renderer::EndFrame() {
    ZoneScoped;
    spdlog::trace("Ended renderer frame");
    FrameMark;
}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, std::shared_ptr<Camera>) {
    ZoneScoped;
    m_Framebuffer->Bind();
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
    m_Framebuffer->Unbind();
    spdlog::trace("Submited vertex array to renderer");
}

void Renderer::Submit(const std::shared_ptr<Scene>& scene, std::shared_ptr<Camera> camera) {
    ZoneScoped;
    m_Framebuffer->Bind();
    std::vector<std::shared_ptr<SceneLight>> sceneLights;
    std::vector<glm::mat4> sceneLightTransforms;
    std::unordered_map<std::shared_ptr<Mesh>, glm::mat4> meshes;

    for (auto object : scene->GetObjects()) {
        auto transform = object->GetTransform();
        auto objectPointLights = object->GetLights();
        auto objectMeshes = object->GetMeshs();
        for (auto pointLight : objectPointLights) {
            sceneLights.push_back(pointLight);
            sceneLightTransforms.push_back(transform);
        }
        for (auto mesh : objectMeshes) {
            meshes[mesh] = transform;
        }
    }

    for (auto& [mesh, transform] : meshes) {
        const std::shared_ptr<Shader>& shader = m_Shader;

        shader->Bind();
        shader->UploadMatrix4("u_ViewProj", camera->GetViewProj());
        shader->UploadFloat3("u_ViewPos", camera->GetPosition());

        uint32_t numPointLights = 0, numDirectionalLights = 0, numSpotLights = 0;

        for (uint32_t i = 0; i < sceneLights.size(); i++) {
            if (sceneLights[i]->Type == SceneLight::SceneLightType::Point) {
                PointLight* pointLight = (PointLight*) sceneLights[i].get();
                std::string prefix = "u_PointLights[" + std::to_string(i) + "].";
                glm::vec3 position = glm::vec3(glm::column(sceneLightTransforms[i], 3));
                shader->UploadFloat3(prefix + "position", position);
                shader->UploadFloat(prefix + "constant", pointLight->Constant);
                shader->UploadFloat(prefix + "linear", pointLight->Linear);
                shader->UploadFloat(prefix + "quadratic", pointLight->Quadratic);
                shader->UploadFloat3(prefix + "ambient", pointLight->Ambient);
                shader->UploadFloat3(prefix + "diffuse", pointLight->Diffuse);
                shader->UploadFloat3(prefix + "specular", pointLight->Specular);
                numPointLights++;
            } else if (sceneLights[i]->Type == SceneLight::SceneLightType::Directional) {
                DirectionalLight* directionalLight = (DirectionalLight*) sceneLights[i].get();
                std::string prefix = "u_DirectionalLights[" + std::to_string(i) + "].";
                glm::vec3 position = glm::vec3(glm::column(sceneLightTransforms[i], 3));
                shader->UploadFloat3(prefix + "position", position);
                shader->UploadFloat3(prefix + "ambient", directionalLight->Ambient);
                shader->UploadFloat3(prefix + "diffuse", directionalLight->Diffuse);
                shader->UploadFloat3(prefix + "specular", directionalLight->Specular);
                numDirectionalLights++;
            } else if (sceneLights[i]->Type == SceneLight::SceneLightType::Spot) {
                SpotLight* spotLight = (SpotLight*) sceneLights[i].get();
                std::string prefix = "u_SpotLights[" + std::to_string(i) + "].";
                glm::vec3 position = glm::vec3(glm::column(sceneLightTransforms[i], 3));
                glm::vec3 direction = glm::vec3(glm::column(sceneLightTransforms[i], 2));
                shader->UploadFloat3(prefix + "position", position);
                shader->UploadFloat3(prefix + "direction", direction);
                shader->UploadFloat(prefix + "cutOff", spotLight->CutOff);
                shader->UploadFloat(prefix + "outerCutOff", spotLight->OuterCutOff);
                shader->UploadFloat(prefix + "constant", spotLight->Constant);
                shader->UploadFloat(prefix + "linear", spotLight->Linear);
                shader->UploadFloat(prefix + "quadratic", spotLight->Quadratic);
                shader->UploadFloat3(prefix + "ambient", spotLight->Ambient);
                shader->UploadFloat3(prefix + "diffuse", spotLight->Diffuse);
                shader->UploadFloat3(prefix + "specular", spotLight->Specular);
                numSpotLights++;
            }
        }

        shader->UploadInt("u_NumPointLights", numPointLights);
        shader->UploadInt("u_NumDirectionalLights", numDirectionalLights);
        shader->UploadInt("u_NumSpotLights", numSpotLights);

        shader->UploadMatrix4("u_Model", transform);

        for (auto& vertexArray : mesh->GetVertexArrays()) {
            shader->UploadUniformQueues();
            RenderCommand::DrawIndexed(vertexArray);
        }
    }

    m_Framebuffer->Unbind();
    spdlog::trace("Submitted scene to renderer (NumLights: {}, NumMeshes: {})", sceneLights.size(), meshes.size());
}
