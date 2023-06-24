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

VulkanShader::VulkanShader(const String& filepath) {
    ZoneScoped;
}

VulkanShader::VulkanShader(const String& name, const String& vertexSrc, const String& fragmentSrc, const String& geometrySrc) : m_Name(name) {
    ZoneScoped;
}

VulkanShader::~VulkanShader() {
    ZoneScoped;
    // HY_LOG_TRACE("Deleted Vulkan shader program (ID: {})", m_RendererID); // Dont now why but this segfaults (maybe lifetime related)
}

String VulkanShader::ReadFile(const String& filepath) {
    ZoneScoped;
}

std::unordered_map<GLenum, String> VulkanShader::PreProcess(const String& source) {
    ZoneScoped;
}

void VulkanShader::Compile(const std::unordered_map<GLenum, String>& shaderSources) {
    ZoneScoped;
}

void VulkanShader::Bind() const {
    ZoneScoped;
    HY_LOG_TRACE("Bound Vulkan shader program (ID: {})", m_RendererID);
}

void VulkanShader::Unbind() const {
    ZoneScoped;
    HY_LOG_TRACE("Unbound Vulkan shader program (ID: {})", m_RendererID);
}

void VulkanShader::SetInt(const String& name, int value) {
    ZoneScoped;
}

void VulkanShader::SetFloat(const String& name, float value) {
    ZoneScoped;
}

void VulkanShader::SetFloat2(const String& name, const glm::vec2& value) {
    ZoneScoped;
}

void VulkanShader::SetFloat3(const String& name, const glm::vec3& value) {
    ZoneScoped;
}

void VulkanShader::SetFloat4(const String& name, const glm::vec4& value) {
    ZoneScoped;
}

void VulkanShader::SetMatrix3(const String& name, const glm::mat3& matrix) {
    ZoneScoped;
}

void VulkanShader::SetMatrix4(const String& name, const glm::mat4& matrix) {
    ZoneScoped;
}

void VulkanShader::UploadInt(const String& name, int value) {
    ZoneScoped;
    HY_LOG_TRACE("Uploaded int to Vulkan shader program (name: {}, value: {}, ID: {})", name, value, m_RendererID);
}

void VulkanShader::UploadFloat(const String& name, float value) {
    ZoneScoped;
    HY_LOG_TRACE("Uploaded float to Vulkan shader program (name: {}, value: {}, ID: {})", name, value, m_RendererID);
}

void VulkanShader::UploadFloat2(const String& name, const glm::vec2& value) {
    ZoneScoped;
    HY_LOG_TRACE("Uploaded vector2 to Vulkan shader program (name: {}, value: ({}, {}), ID: {})", name, value.x, value.y, m_RendererID);
}

void VulkanShader::UploadFloat3(const String& name, const glm::vec3& value) {
    ZoneScoped;
    HY_LOG_TRACE("Uploaded vector3 to Vulkan shader program (name: {}, value: ({}, {}, {}), ID: {})", name, value.x, value.y, value.z, m_RendererID);
}

void VulkanShader::UploadFloat4(const String& name, const glm::vec4& value) {
    ZoneScoped;
    HY_LOG_TRACE("Uploaded vector4 to Vulkan shader program (name: {}, value: ({}, {}, {}, {}), ID: {})", name, value.x, value.y, value.z, value.w, m_RendererID);
}

void VulkanShader::UploadMatrix3(const String& name, const glm::mat3& matrix) {
    ZoneScoped;
    HY_LOG_TRACE("Uploaded matrix3x3 to Vulkan shader program (name: {}, ID: {})", name, m_RendererID);
}

void VulkanShader::UploadMatrix4(const String& name, const glm::mat4& matrix) {
    ZoneScoped;
    HY_LOG_TRACE("Uploaded matrix4x4 to Vulkan shader program (name: {}, ID: {})", name, m_RendererID);
}

void VulkanShader::UploadUniformQueues() {
    ZoneScoped;
}
