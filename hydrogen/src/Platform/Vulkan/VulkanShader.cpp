#include <Hydrogen/Platform/Vulkan/VulkanShader.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRendererAPI.hpp>
#include <Hydrogen/Core/Memory.hpp>
#include <Hydrogen/Core/Logger.hpp>
#include <array>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <tracy/Tracy.hpp>
#include <iostream>
#include <vector>

using namespace Hydrogen::Vulkan;
using namespace Hydrogen;

VulkanShader::VulkanShader(const String& name, const DynamicArray<uint32_t>& vertexSrc, const DynamicArray<uint32_t>& fragmentSrc, const DynamicArray<uint32_t>& geometrySrc)
    : m_Name(name) {
    ZoneScoped;
}

VulkanShader::~VulkanShader() {
    ZoneScoped;
}

void VulkanShader::Bind() const {
    ZoneScoped;
}

void VulkanShader::Unbind() const {
    ZoneScoped;
}
