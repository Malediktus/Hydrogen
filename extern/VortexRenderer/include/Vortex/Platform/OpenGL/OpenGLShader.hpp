#pragma once

#include "../../Shader.hpp"

#include <glm/glm.hpp>

#include <unordered_map>

typedef unsigned int GLenum;

namespace Vortex::OpenGL {
class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc);
    virtual ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual const std::string& GetName() const override {
        return m_Name;
    }

    virtual void SetInt(const std::string& name, int value) override;

    virtual void SetFloat(const std::string& name, float value) override;
    virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
    virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
    virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;

    virtual void SetMatrix3(const std::string& name, const glm::mat3& matrix) override;
    virtual void SetMatrix4(const std::string& name, const glm::mat4& matrix) override;

    virtual void UploadInt(const std::string& name, int value) override;

    virtual void UploadFloat(const std::string& name, float value) override;
    virtual void UploadFloat2(const std::string& name, const glm::vec2& value) override;
    virtual void UploadFloat3(const std::string& name, const glm::vec3& value) override;
    virtual void UploadFloat4(const std::string& name, const glm::vec4& value) override;

    virtual void UploadMatrix3(const std::string& name, const glm::mat3& matrix) override;
    virtual void UploadMatrix4(const std::string& name, const glm::mat4& matrix) override;

    virtual void UploadUniformQueues() override;

private:
    std::string ReadFile(const std::string& filepath);
    std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
    void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

    std::unordered_map<std::string, int> m_UniformInts;
    std::unordered_map<std::string, float> m_UniformFloats;
    std::unordered_map<std::string, glm::vec2> m_UniformFloat2s;
    std::unordered_map<std::string, glm::vec3> m_UniformFloat3s;
    std::unordered_map<std::string, glm::vec4> m_UniformFloat4s;
    std::unordered_map<std::string, glm::mat3> m_UniformMatrix3s;
    std::unordered_map<std::string, glm::mat4> m_UniformMatrix4s;

    uint32_t m_RendererID;
    std::string m_Name;
};
} // namespace Vortex::OpenGL
