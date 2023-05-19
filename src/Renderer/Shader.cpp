#include <Hydrogen/Renderer/Shader.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLShader.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

Reference<Shader> Shader::Create(const std::string& filepath) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return NewReference<OpenGL::OpenGLShader>(filepath);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}

Reference<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return NewReference<OpenGL::OpenGLShader>(name, vertexSrc, fragmentSrc, geometrySrc);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}

void ShaderLibrary::Add(const std::string& name, const Reference<Shader>& shader) {
    ZoneScoped;
    HY_ASSERT(Exists(name), "Shader name already registered in ShaderLibrary");
    m_Shaders[name] = shader;
}

void ShaderLibrary::Add(const Reference<Shader>& shader) {
    ZoneScoped;
    auto& name = shader->GetName();
    Add(name, shader);
}

Reference<Shader> ShaderLibrary::Load(const std::string& filepath) {
    ZoneScoped;
    auto shader = Shader::Create(filepath);
    Add(shader);
    return shader;
}

Reference<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
    ZoneScoped;
    auto shader = Shader::Create(filepath);
    Add(name, shader);
    return shader;
}

Reference<Shader> ShaderLibrary::Get(const std::string& name) {
    ZoneScoped;
    HY_ASSERT(!Exists(name), "Shader name not registered in ShaderLibrary");
    return m_Shaders[name];
}

bool ShaderLibrary::Exists(const std::string& name) const {
    ZoneScoped;
    return m_Shaders.find(name) != m_Shaders.end();
}
