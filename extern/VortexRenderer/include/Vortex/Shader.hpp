#pragma once

#include "Core.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace Vortex {
/**
 * @brief This class stored a shader and loads and compiles that. It also handles uniforms.
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
class Shader {
public:
    virtual VT_API ~Shader() = default;

    virtual VT_API void Bind() const = 0;
    virtual VT_API void Unbind() const = 0;

    virtual VT_API void SetInt(const std::string& name, int value) = 0;

    virtual VT_API void SetFloat(const std::string& name, float value) = 0;
    virtual VT_API void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
    virtual VT_API void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
    virtual VT_API void SetFloat4(const std::string& name, const glm::vec4& value) = 0;

    virtual VT_API void SetMatrix3(const std::string& name, const glm::mat3& matrix) = 0;
    virtual VT_API void SetMatrix4(const std::string& name, const glm::mat4& matrix) = 0;

    virtual VT_API void UploadInt(const std::string& name, int value) = 0;

    virtual VT_API void UploadFloat(const std::string& name, float value) = 0;
    virtual VT_API void UploadFloat2(const std::string& name, const glm::vec2& value) = 0;
    virtual VT_API void UploadFloat3(const std::string& name, const glm::vec3& value) = 0;
    virtual VT_API void UploadFloat4(const std::string& name, const glm::vec4& value) = 0;

    virtual VT_API void UploadMatrix3(const std::string& name, const glm::mat3& matrix) = 0;
    virtual VT_API void UploadMatrix4(const std::string& name, const glm::mat4& matrix) = 0;

    virtual VT_API void UploadUniformQueues() = 0;

    virtual VT_API const std::string& GetName() const = 0;
};

/**
 * @brief This function is the only way that should be used for creating a Shader from a file.
 *
 * @param filepath
 * @return std::shared_ptr<Shader>
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
VT_API std::shared_ptr<Shader> ShaderCreate(const std::string& filepath);
/**
 * @brief This function is the only way that should be used for creating a Shader from source strings.
 *
 * @param filepath
 * @return std::shared_ptr<Shader>
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
VT_API std::shared_ptr<Shader> ShaderCreate(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc = "");

/**
 * @brief This is a high level API for managing shaders. It automaticly handles allocation and deallocation of shaders.
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
class ShaderLibrary {
public:
    VT_API void Add(const std::string& name, const std::shared_ptr<Shader>& shader);
    VT_API void Add(const std::shared_ptr<Shader>& shader);
    VT_API std::shared_ptr<Shader> Load(const std::string& filepath);
    VT_API std::shared_ptr<Shader> Load(const std::string& name, const std::string& filepath);

    VT_API std::shared_ptr<Shader> Get(const std::string& name);

    VT_API bool Exists(const std::string& name) const;

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
};
} // namespace Vortex
