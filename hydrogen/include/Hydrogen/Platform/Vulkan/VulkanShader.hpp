#pragma once

#include "VulkanContext.hpp"
#include "../../Renderer/Shader.hpp"
#include "../../Renderer/Renderer.hpp"
#include "../../Math/Math.hpp"
#include <unordered_map>

typedef unsigned int GLenum;

namespace Hydrogen::Vulkan {
class VulkanShader : public Shader {
public:
    VulkanShader(const String& name, const DynamicArray<uint32_t>& vertexSrc, const DynamicArray<uint32_t>& fragmentSrc, const DynamicArray<uint32_t>& geometrySrc);
    virtual ~VulkanShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual const String& GetName() const override {
        return m_Name;
    }

private:
    String m_Name;
    VkShaderModule m_VertexShaderModule;
    VkShaderModule m_FragmentShaderModule;
    VkShaderModule m_GeometryShaderModule;
};
} // namespace Hydrogen::Vulkan
