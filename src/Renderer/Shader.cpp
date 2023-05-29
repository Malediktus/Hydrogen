#include <Hydrogen/Renderer/Shader.hpp>
#include <Hydrogen/Renderer/Renderer.hpp>
#include <Hydrogen/Platform/OpenGL/OpenGLShader.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen;

ReferencePointer<Shader> Shader::Create(const String& filepath) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return NewReferencePointer<OpenGL::OpenGLShader>(filepath);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}

ReferencePointer<Shader> Shader::Create(const String& name, const String& vertexSrc, const String& fragmentSrc, const String& geometrySrc) {
    ZoneScoped;
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
        return NewReferencePointer<OpenGL::OpenGLShader>(name, vertexSrc, fragmentSrc, geometrySrc);
    default:
        HY_ASSERT_CHECK(false, "Invalid renderer API value returned from Renderer::GetRendererAPI()");
    }
    return nullptr;
}

void ShaderLibrary::Add(const String& name, const ReferencePointer<Shader>& shader) {
    ZoneScoped;
    HY_ASSERT(Exists(name), "Shader name already registered in ShaderLibrary");
    m_Shaders[name] = shader;
}

void ShaderLibrary::Add(const ReferencePointer<Shader>& shader) {
    ZoneScoped;
    auto& name = shader->GetName();
    Add(name, shader);
}

ReferencePointer<Shader> ShaderLibrary::Load(const String& filepath) {
    ZoneScoped;
    auto shader = Shader::Create(filepath);
    Add(shader);
    return shader;
}

ReferencePointer<Shader> ShaderLibrary::Load(const String& name, const String& filepath) {
    ZoneScoped;
    auto shader = Shader::Create(filepath);
    Add(name, shader);
    return shader;
}

ReferencePointer<Shader> ShaderLibrary::Get(const String& name) {
    ZoneScoped;
    HY_ASSERT(!Exists(name), "Shader name not registered in ShaderLibrary");
    return m_Shaders[name];
}

bool ShaderLibrary::Exists(const String& name) const {
    ZoneScoped;
    return m_Shaders.find(name) != m_Shaders.end();
}
