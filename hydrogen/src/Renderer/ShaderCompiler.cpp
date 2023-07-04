#include <Hydrogen/Core/Assert.hpp>
#include <Hydrogen/Renderer/ShaderCompiler.hpp>

using namespace Hydrogen;

const TBuiltInResource DefaultTBuiltInResource = {
    /* .MaxLights = */ 32,
    /* .MaxClipPlanes = */ 6,
    /* .MaxTextureUnits = */ 32,
    /* .MaxTextureCoords = */ 32,
    /* .MaxVertexAttribs = */ 64,
    /* .MaxVertexUniformComponents = */ 4096,
    /* .MaxVaryingFloats = */ 64,
    /* .MaxVertexTextureImageUnits = */ 32,
    /* .MaxCombinedTextureImageUnits = */ 80,
    /* .MaxTextureImageUnits = */ 32,
    /* .MaxFragmentUniformComponents = */ 4096,
    /* .MaxDrawBuffers = */ 32,
    /* .MaxVertexUniformVectors = */ 128,
    /* .MaxVaryingVectors = */ 8,
    /* .MaxFragmentUniformVectors = */ 16,
    /* .MaxVertexOutputVectors = */ 16,
    /* .MaxFragmentInputVectors = */ 15,
    /* .MinProgramTexelOffset = */ -8,
    /* .MaxProgramTexelOffset = */ 7,
    /* .MaxClipDistances = */ 8,
    /* .MaxComputeWorkGroupCountX = */ 65535,
    /* .MaxComputeWorkGroupCountY = */ 65535,
    /* .MaxComputeWorkGroupCountZ = */ 65535,
    /* .MaxComputeWorkGroupSizeX = */ 1024,
    /* .MaxComputeWorkGroupSizeY = */ 1024,
    /* .MaxComputeWorkGroupSizeZ = */ 64,
    /* .MaxComputeUniformComponents = */ 1024,
    /* .MaxComputeTextureImageUnits = */ 16,
    /* .MaxComputeImageUniforms = */ 8,
    /* .MaxComputeAtomicCounters = */ 8,
    /* .MaxComputeAtomicCounterBuffers = */ 1,
    /* .MaxVaryingComponents = */ 60,
    /* .MaxVertexOutputComponents = */ 64,
    /* .MaxGeometryInputComponents = */ 64,
    /* .MaxGeometryOutputComponents = */ 128,
    /* .MaxFragmentInputComponents = */ 128,
    /* .MaxImageUnits = */ 8,
    /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
    /* .MaxCombinedShaderOutputResources = */ 8,
    /* .MaxImageSamples = */ 0,
    /* .MaxVertexImageUniforms = */ 0,
    /* .MaxTessControlImageUniforms = */ 0,
    /* .MaxTessEvaluationImageUniforms = */ 0,
    /* .MaxGeometryImageUniforms = */ 0,
    /* .MaxFragmentImageUniforms = */ 8,
    /* .MaxCombinedImageUniforms = */ 8,
    /* .MaxGeometryTextureImageUnits = */ 16,
    /* .MaxGeometryOutputVertices = */ 256,
    /* .MaxGeometryTotalOutputComponents = */ 1024,
    /* .MaxGeometryUniformComponents = */ 1024,
    /* .MaxGeometryVaryingComponents = */ 64,
    /* .MaxTessControlInputComponents = */ 128,
    /* .MaxTessControlOutputComponents = */ 128,
    /* .MaxTessControlTextureImageUnits = */ 16,
    /* .MaxTessControlUniformComponents = */ 1024,
    /* .MaxTessControlTotalOutputComponents = */ 4096,
    /* .MaxTessEvaluationInputComponents = */ 128,
    /* .MaxTessEvaluationOutputComponents = */ 128,
    /* .MaxTessEvaluationTextureImageUnits = */ 16,
    /* .MaxTessEvaluationUniformComponents = */ 1024,
    /* .MaxTessPatchComponents = */ 120,
    /* .MaxPatchVertices = */ 32,
    /* .MaxTessGenLevel = */ 64,
    /* .MaxViewports = */ 16,
    /* .MaxVertexAtomicCounters = */ 0,
    /* .MaxTessControlAtomicCounters = */ 0,
    /* .MaxTessEvaluationAtomicCounters = */ 0,
    /* .MaxGeometryAtomicCounters = */ 0,
    /* .MaxFragmentAtomicCounters = */ 8,
    /* .MaxCombinedAtomicCounters = */ 8,
    /* .MaxAtomicCounterBindings = */ 1,
    /* .MaxVertexAtomicCounterBuffers = */ 0,
    /* .MaxTessControlAtomicCounterBuffers = */ 0,
    /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
    /* .MaxGeometryAtomicCounterBuffers = */ 0,
    /* .MaxFragmentAtomicCounterBuffers = */ 1,
    /* .MaxCombinedAtomicCounterBuffers = */ 1,
    /* .MaxAtomicCounterBufferSize = */ 16384,
    /* .MaxTransformFeedbackBuffers = */ 4,
    /* .MaxTransformFeedbackInterleavedComponents = */ 64,
    /* .MaxCullDistances = */ 8,
    /* .MaxCombinedClipAndCullDistances = */ 8,
    /* .MaxSamples = */ 4,
    /* .maxMeshOutputVerticesNV = */ 256,
    /* .maxMeshOutputPrimitivesNV = */ 512,
    /* .maxMeshWorkGroupSizeX_NV = */ 32,
    /* .maxMeshWorkGroupSizeY_NV = */ 1,
    /* .maxMeshWorkGroupSizeZ_NV = */ 1,
    /* .maxTaskWorkGroupSizeX_NV = */ 32,
    /* .maxTaskWorkGroupSizeY_NV = */ 1,
    /* .maxTaskWorkGroupSizeZ_NV = */ 1,
    /* .maxMeshViewCountNV = */ 4,
    /* .maxMeshOutputVerticesEXT = */ 256,
    /* .maxMeshOutputPrimitivesEXT = */ 256,
    /* .maxMeshWorkGroupSizeX_EXT = */ 128,
    /* .maxMeshWorkGroupSizeY_EXT = */ 128,
    /* .maxMeshWorkGroupSizeZ_EXT = */ 128,
    /* .maxTaskWorkGroupSizeX_EXT = */ 128,
    /* .maxTaskWorkGroupSizeY_EXT = */ 128,
    /* .maxTaskWorkGroupSizeZ_EXT = */ 128,
    /* .maxMeshViewCountEXT = */ 4,
    /* .maxDualSourceDrawBuffersEXT = */ 1,

    /* .limits = */
    {
        /* .nonInductiveForLoops = */ 1,
        /* .whileLoops = */ 1,
        /* .doWhileLoops = */ 1,
        /* .generalUniformIndexing = */ 1,
        /* .generalAttributeMatrixVectorIndexing = */ 1,
        /* .generalVaryingIndexing = */ 1,
        /* .generalSamplerIndexing = */ 1,
        /* .generalVariableIndexing = */ 1,
        /* .generalConstantMatrixVectorIndexing = */ 1,
    }};

ShaderCompiler::ShaderCompiler(ShaderLanguage frontEnd, ShaderClient client,
                               SpriVVersion spirvVersion, ShaderStage stage,
                               uint32_t version)
    : m_Version(version) {
  glslang_initialize_process();

  switch (frontEnd) {
    case ShaderLanguage::GLSL:
      m_FrontEndLang = GLSLANG_SOURCE_GLSL;
      break;
    case ShaderLanguage::HLSL:
      m_FrontEndLang = GLSLANG_SOURCE_HLSL;
      break;
    default:
      HY_INVOKE_ERROR("Invalid ShaderLanguage!");
      break;
  }

  m_ShaderClient = GLSLANG_CLIENT_VULKAN;
  switch (client) {
    case ShaderClient::Vulkan_1_0:
      m_ShaderClientVersion = GLSLANG_TARGET_VULKAN_1_0;
      break;
    case ShaderClient::Vulkan_1_1:
      m_ShaderClientVersion = GLSLANG_TARGET_VULKAN_1_1;
      break;
    case ShaderClient::Vulkan_1_2:
      m_ShaderClientVersion = GLSLANG_TARGET_VULKAN_1_2;
      break;
    case ShaderClient::Vulkan_1_3:
      m_ShaderClientVersion = GLSLANG_TARGET_VULKAN_1_3;
      break;
    default:
      HY_INVOKE_ERROR("Invalid ShaderClient!");
      break;
  }

  switch (spirvVersion) {
    case SpriVVersion::SpriV_1_0:
      m_SpirvVersion = GLSLANG_TARGET_SPV_1_0;
      break;
    case SpriVVersion::SpriV_1_1:
      m_SpirvVersion = GLSLANG_TARGET_SPV_1_1;
      break;
    case SpriVVersion::SpriV_1_2:
      m_SpirvVersion = GLSLANG_TARGET_SPV_1_2;
      break;
    case SpriVVersion::SpriV_1_3:
      m_SpirvVersion = GLSLANG_TARGET_SPV_1_3;
      break;
    case SpriVVersion::SpriV_1_4:
      m_SpirvVersion = GLSLANG_TARGET_SPV_1_4;
      break;
    case SpriVVersion::SpriV_1_5:
      m_SpirvVersion = GLSLANG_TARGET_SPV_1_5;
      break;
    case SpriVVersion::SpriV_1_6:
      m_SpirvVersion = GLSLANG_TARGET_SPV_1_6;
      break;
    default:
      HY_INVOKE_ERROR("Invalid SpirVVersion!");
      break;
  }

  switch (stage) {
    case ShaderStage::VertexShader:
      m_ShaderStage = GLSLANG_STAGE_VERTEX;
      break;
    case ShaderStage::PixelShader:
      m_ShaderStage = GLSLANG_STAGE_FRAGMENT;
      break;
    case ShaderStage::GeometryShader:
      m_ShaderStage = GLSLANG_STAGE_GEOMETRY;
      break;
    default:
      HY_INVOKE_ERROR("Invalid ShaderStage!");
      break;
  }

  m_Program = glslang_program_create();
}

ShaderCompiler::~ShaderCompiler() {
  for (auto& shader : m_Shader) {
    glslang_shader_delete(shader);
  }
  glslang_program_delete(m_Program);
  glslang_finalize_process();
}

void ShaderCompiler::AddShader(const String& source) {
  const glslang_input_t input = {
      .language = m_FrontEndLang,
      .stage = m_ShaderStage,
      .client = m_ShaderClient,
      .client_version = m_ShaderClientVersion,
      .target_language = GLSLANG_TARGET_SPV,
      .target_language_version = m_SpirvVersion,
      .code = source.c_str(),  // TODO: Maybe unsafe
      .default_version = static_cast<int>(m_Version),
      .default_profile = GLSLANG_NO_PROFILE,
      .force_default_version_and_profile = false,
      .forward_compatible = false,
      .messages = GLSLANG_MSG_DEFAULT_BIT,
      .resource =
          reinterpret_cast<const glslang_resource_t*>(&DefaultTBuiltInResource),
  };

  glslang_shader_t* shader = glslang_shader_create(&input);

  if (!glslang_shader_preprocess(shader, &input)) {
    HY_INVOKE_ERROR("Shader preprocessing failed:\n{}",
                    glslang_shader_get_info_log(shader))
  }

  if (!glslang_shader_parse(shader, &input)) {
    HY_INVOKE_ERROR("Shader compilation failed:\n{}",
                    glslang_shader_get_info_log(shader))
  }

  glslang_program_add_shader(m_Program, shader);
  m_Shader.push_back(shader);
}

void ShaderCompiler::Link() {
  if (!glslang_program_link(m_Program, GLSLANG_MSG_SPV_RULES_BIT |
                                           GLSLANG_MSG_VULKAN_RULES_BIT)) {
    HY_INVOKE_ERROR("Shader linking failed:\n{}",
                    glslang_program_get_info_log(m_Program))
  }
}

DynamicArray<uint32_t> ShaderCompiler::GetSpriv() {
  glslang_program_SPIRV_generate(m_Program, m_ShaderStage);

  size_t sprvSize = glslang_program_SPIRV_get_size(m_Program);
  uint32_t* sprvData = glslang_program_SPIRV_get_ptr(m_Program);

  DynamicArray<uint32_t> res;
  res.reserve(sprvSize);

  for (uint32_t i = 0; i < sprvSize; i++) {
    res.push_back(sprvData[i]);
  }

  return res;
}
