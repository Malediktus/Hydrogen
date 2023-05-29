#pragma once

#include "../../Renderer/Shader.hpp"
#include "../../Math/Math.hpp"
#include <unordered_map>

typedef unsigned int GLenum;

namespace Hydrogen::OpenGL {
class OpenGLShader : public Shader {
public:
    OpenGLShader(const String& filepath);
    OpenGLShader(const String& name, const String& vertexSrc, const String& fragmentSrc, const String& geometrySrc);
    virtual ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual const String& GetName() const override {
        return m_Name;
    }

    virtual void SetInt(const String& name, int value) override;

    virtual void SetFloat(const String& name, float value) override;
    virtual void SetFloat2(const String& name, const Vector2& value) override;
    virtual void SetFloat3(const String& name, const Vector3& value) override;
    virtual void SetFloat4(const String& name, const Vector4& value) override;

    virtual void SetMatrix3(const String& name, const Matrix3& matrix) override;
    virtual void SetMatrix4(const String& name, const Matrix4& matrix) override;

    virtual void UploadInt(const String& name, int value) override;

    virtual void UploadFloat(const String& name, float value) override;
    virtual void UploadFloat2(const String& name, const Vector2& value) override;
    virtual void UploadFloat3(const String& name, const Vector3& value) override;
    virtual void UploadFloat4(const String& name, const Vector4& value) override;

    virtual void UploadMatrix3(const String& name, const Matrix3& matrix) override;
    virtual void UploadMatrix4(const String& name, const Matrix4& matrix) override;

    virtual void UploadUniformQueues() override;

private:
    String ReadFile(const String& filepath);
    std::unordered_map<GLenum, String> PreProcess(const String& source);
    void Compile(const std::unordered_map<GLenum, String>& shaderSources);

    std::unordered_map<String, int> m_UniformInts;
    std::unordered_map<String, float> m_UniformFloats;
    std::unordered_map<String, Vector2> m_UniformFloat2s;
    std::unordered_map<String, Vector3> m_UniformFloat3s;
    std::unordered_map<String, Vector4> m_UniformFloat4s;
    std::unordered_map<String, Matrix3> m_UniformMatrix3s;
    std::unordered_map<String, Matrix4> m_UniformMatrix4s;

    uint32_t m_RendererID;
    String m_Name;
};
} // namespace Hydrogen::OpenGL
