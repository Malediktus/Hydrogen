#pragma once

#include "RenderCommand.hpp"
#include "RendererAPI.hpp"
#include "Core.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "Context.hpp"
#include "Framebuffer.hpp"
#include "Scene.hpp"

#include <glm/glm.hpp>

#include <memory>
#include <string>

namespace Vortex {
/**
 * @brief The Renderer is a high level API for rendering. Init() should be called after context creation. LoadShader() and
 * AddShader() function are used for adding shaders. Submit() renders a provided VertexArray and should be surrounded by
 * BeginScene() and EndScene().
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
class Renderer {
public:
    VT_API Renderer(const std::shared_ptr<Shader>& defaultShader, const int width, const int height, bool renderToTexture);
    VT_API ~Renderer() = default;

    VT_API void OnResize(const int width, const int height);

    VT_API void BeginFrame();
    VT_API void EndFrame();

    VT_API void Submit(const std::shared_ptr<VertexArray>& vertexArray, std::shared_ptr<Camera> camera);
    VT_API void Submit(const std::shared_ptr<Scene>& scene, std::shared_ptr<Camera> camera);

    VT_API std::shared_ptr<Texture2D> GetTexture() {
        return m_ColorTexture;
    }

    inline static VT_API RendererAPI::API GetAPI() {
        return RendererAPI::GetAPI();
    }

    static VT_API void SetContext(std::shared_ptr<Context> context);
    static VT_API std::shared_ptr<Context> GetContext();

private:
    bool m_RenderToTexture;
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<Framebuffer> m_Framebuffer;
    std::shared_ptr<Texture2D> m_ColorTexture;
    std::shared_ptr<Renderbuffer> m_DepthStencilRenderbuffer;
    static std::shared_ptr<Context> s_Context;
};
} // namespace Vortex
