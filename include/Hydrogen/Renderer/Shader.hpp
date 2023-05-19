#pragma once

#include "../Core/Base.hpp"
#include "../Core/Assert.hpp"
#include "../Core/Memory.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace Hydrogen {
class Shader {
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetInt(const std::string& name, int value) = 0;

    virtual void SetFloat(const std::string& name, float value) = 0;
    virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
    virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
    virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;

    virtual void SetMatrix3(const std::string& name, const glm::mat3& matrix) = 0;
    virtual void SetMatrix4(const std::string& name, const glm::mat4& matrix) = 0;

    virtual void UploadInt(const std::string& name, int value) = 0;

    virtual void UploadFloat(const std::string& name, float value) = 0;
    virtual void UploadFloat2(const std::string& name, const glm::vec2& value) = 0;
    virtual void UploadFloat3(const std::string& name, const glm::vec3& value) = 0;
    virtual void UploadFloat4(const std::string& name, const glm::vec4& value) = 0;

    virtual void UploadMatrix3(const std::string& name, const glm::mat3& matrix) = 0;
    virtual void UploadMatrix4(const std::string& name, const glm::mat4& matrix) = 0;

    virtual void UploadUniformQueues() = 0;

    virtual const std::string& GetName() const = 0;

    static Reference<Shader> Create(const std::string& filepath);
    static Reference<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc = "");
};

class ShaderLibrary {
public:
    void Add(const std::string& name, const Reference<Shader>& shader);
    void Add(const Reference<Shader>& shader);
    Reference<Shader> Load(const std::string& filepath);
    Reference<Shader> Load(const std::string& name, const std::string& filepath);

    Reference<Shader> Get(const std::string& name);

    bool Exists(const std::string& name) const;

private:
    std::unordered_map<std::string, Reference<Shader>> m_Shaders;
};
} // namespace Hydrogen
