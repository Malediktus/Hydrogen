#include <Hydrogen/Assets/ShaderAsset.hpp>
#include <Hydrogen/Core/Cache.hpp>
#include <Hydrogen/Renderer/Buffer.hpp>
#include <Hydrogen/Renderer/RenderDevice.hpp>
#include <Hydrogen/Renderer/Shader.hpp>
#include <Hydrogen/Renderer/ShaderCompiler.hpp>

using namespace Hydrogen;

void ShaderAsset::Load(const std::filesystem::path& filepath) {
  HY_ASSERT(!filepath.empty(),
            "Parameter 'filepath' of type 'const String&' in function "
            "ShaderAsset::Load(const String& filepath) is an empty string!");
  HY_LOG_INFO("Loading shader asset '{}'!", filepath.string());

  m_Name = filepath.filename().string();

  if (filepath.extension() == ".glsl") {
    for (const auto& dirEntry : std::filesystem::directory_iterator(filepath)) {
      ShaderStage stage;
      DynamicArray<uint32_t>* currentShader;

      if (dirEntry.path().extension().string() == ".vert") {
        currentShader = &m_VertexShader;
        stage = ShaderStage::VertexShader;
      } else if (dirEntry.path().extension().string() == ".frag") {
        currentShader = &m_FragmentShader;
        stage = ShaderStage::PixelShader;
      } else if (dirEntry.path().extension().string() == ".geo") {
        currentShader = &m_GeometryShader;
        stage = ShaderStage::GeometryShader;
      } else {
        continue;
      }

      String shaderFilepath = dirEntry.path().string();

      std::ifstream infile;
      infile.open(shaderFilepath, std::ios::in);
      HY_ASSERT(infile.is_open(), "Failed to open file {}", shaderFilepath);
      std::stringstream instream;
      instream << infile.rdbuf();
      infile.close();
      String inbuf = instream.str();

      String outfilepath = "cache/" + shaderFilepath;
      CacheFile cache(outfilepath, inbuf);

      if (!cache.CacheValid()) {
        HY_LOG_INFO("Shader cache {} is invalid. Recompiling!", shaderFilepath)

        ShaderCompiler compiler(ShaderLanguage::GLSL, ShaderClient::Vulkan_1_0, SpriVVersion::SpriV_1_0, stage, 450);
        compiler.AddShader(inbuf);
        compiler.Link();
        auto spirv = compiler.GetSpriv();
        currentShader->insert(currentShader->end(), spirv.begin(), spirv.end());

        String directory = DIRECTORY_FROM_PATH(outfilepath);
        if (!std::filesystem::exists(directory)) {
          std::filesystem::create_directories(directory);
        }

        std::ofstream outfile;
        outfile.open(outfilepath, std::ios::out | std::ios::binary);
        HY_ASSERT(outfile.is_open(), "Failed to open file {}!", outfilepath);
        outfile.write(reinterpret_cast<const char*>(currentShader->data()), currentShader->size() * sizeof(uint32_t));
        outfile.close();
      } else {
        std::uintmax_t fileSize = std::filesystem::file_size(outfilepath);
        std::ifstream infile2;

        infile2.open(outfilepath, std::ios::in | std::ios::binary);
        HY_ASSERT(infile2.is_open(), "Failed to open file {}!", outfilepath);
        infile2.unsetf(std::ios::skipws);

        DynamicArray<char> fileData(fileSize);
        infile2.read(fileData.data(), fileSize);

        currentShader->resize(fileSize / sizeof(uint32_t));
        std::memcpy(currentShader->data(), fileData.data(), fileSize);

        infile2.close();
      }

      cache.UpdateCacheChecksum();
    }
  } else {
    HY_INVOKE_ERROR("Only glsl is supported for now!");
  }

  HY_LOG_INFO("Finished loading shader asset '{}'!", filepath.string());
}

const ReferencePointer<Shader>& ShaderAsset::GetShader(const ReferencePointer<RenderWindow>& window, const BufferLayout& vertexLayout, const ShaderDependencyGraph dependencyGraph) {
  if (m_Shader) {
    return m_Shader;
  }
  m_Shader = Shader::Create(window, vertexLayout, dependencyGraph, m_Name, m_VertexShader, m_FragmentShader, m_GeometryShader);
  return m_Shader;
}
