#pragma once

#include <glslang/Include/ResourceLimits.h>
#include <glslang/Include/glslang_c_interface.h>

#include "../Core/Memory.hpp"

namespace Hydrogen {
enum class ShaderLanguage { GLSL = 0, HLSL = 1 };
enum class ShaderClient { Vulkan_1_0 = 0, Vulkan_1_1 = 1, Vulkan_1_2 = 2, Vulkan_1_3 = 3 };
enum class SpriVVersion { SpriV_1_0 = 0, SpriV_1_1 = 1, SpriV_1_2 = 2, SpriV_1_3 = 3, SpriV_1_4 = 4, SpriV_1_5 = 5, SpriV_1_6 = 6 };
enum class ShaderStage { VertexShader = 0, PixelShader = 1, GeometryShader = 2 };

class ShaderCompiler {
 public:
  ShaderCompiler(ShaderLanguage frontEnd, ShaderClient client, SpriVVersion spirvVersion, ShaderStage stage, uint32_t version);
  ~ShaderCompiler();

  void AddShader(const String& source);

  void Link();
  DynamicArray<uint32_t> GetSpriv();

 private:
  glslang_source_t m_FrontEndLang;
  glslang_client_t m_ShaderClient;
  glslang_stage_t m_ShaderStage;
  uint32_t m_Version;
  glslang_target_client_version_t m_ShaderClientVersion;
  glslang_target_language_version_t m_SpirvVersion;
  glslang_program_t* m_Program;
  DynamicArray<glslang_shader_t*> m_Shader;
};
}  // namespace Hydrogen
