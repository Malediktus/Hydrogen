#pragma once

#include "../Core/Base.hpp"
#include "../Core/Assert.hpp"
#include "../Core/Memory.hpp"
#include "../Math/Math.hpp"
#include <unordered_map>

namespace Hydrogen {
class Shader {
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetInt(const String& name, int value) = 0;

    virtual void SetFloat(const String& name, float value) = 0;
    virtual void SetFloat2(const String& name, const Vector2& value) = 0;
    virtual void SetFloat3(const String& name, const Vector3& value) = 0;
    virtual void SetFloat4(const String& name, const Vector4& value) = 0;

    virtual void SetMatrix3(const String& name, const Matrix3& matrix) = 0;
    virtual void SetMatrix4(const String& name, const Matrix4& matrix) = 0;

    virtual void UploadInt(const String& name, int value) = 0;

    virtual void UploadFloat(const String& name, float value) = 0;
    virtual void UploadFloat2(const String& name, const Vector2& value) = 0;
    virtual void UploadFloat3(const String& name, const Vector3& value) = 0;
    virtual void UploadFloat4(const String& name, const Vector4& value) = 0;

    virtual void UploadMatrix3(const String& name, const Matrix3& matrix) = 0;
    virtual void UploadMatrix4(const String& name, const Matrix4& matrix) = 0;

    virtual void UploadUniformQueues() = 0;

    virtual const String& GetName() const = 0;

    static ReferencePointer<Shader> Create(const String& filepath);
    static ReferencePointer<Shader> Create(const String& name, const String& vertexSrc, const String& fragmentSrc, const String& geometrySrc = "");
};

class ShaderLibrary {
public:
    void Add(const String& name, const ReferencePointer<Shader>& shader);
    void Add(const ReferencePointer<Shader>& shader);
    ReferencePointer<Shader> Load(const String& filepath);
    ReferencePointer<Shader> Load(const String& name, const String& filepath);

    ReferencePointer<Shader> Get(const String& name);

    bool Exists(const String& name) const;

private:
    std::unordered_map<String, ReferencePointer<Shader>> m_Shaders;
};
} // namespace Hydrogen
