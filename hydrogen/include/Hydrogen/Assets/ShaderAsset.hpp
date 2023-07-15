#pragma once

#include <functional>
#include <ostream>
#include <sstream>

#include "../Core/Cache.hpp"
#include "../Renderer/Shader.hpp"
#include "../Renderer/ShaderCompiler.hpp"
#include "Asset.hpp"

namespace Hydrogen {
class ShaderAsset : public Asset {
 public:
  ShaderAsset() { m_AssetInfo.Preload = true; }

  void Load(const String& filepath) override {
    HY_ASSERT(!filepath.empty(),
              "Parameter 'filepath' of type 'const String&' in function "
              "ShaderAsset::Load(const String& filepath) is an empty string!");
    HY_LOG_INFO("Loading shader asset '{}'!", filepath);

    m_Name = FILENAME_FROM_PATH(filepath);

    if (filepath.substr(filepath.find_last_of(".") + 1) == "glsl") {
      for (const auto& dirEntry : std::filesystem::directory_iterator(filepath)) {
        ShaderStage stage;
        DynamicArray<uint32_t>* currentShader;

        if (dirEntry.path().extension().string() == ".vert") {
          currentShader = &m_VertexShader;
          stage = ShaderStage::VertexShader;
        } else if (dirEntry.path().extension().string() == ".frag") {
          currentShader = &m_PixelShader;
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

          ShaderCompiler compiler(ShaderLanguage::GLSL, ShaderClient::Vulkan_1_3, SpriVVersion::SpriV_1_6, stage, 330);
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
          for (auto t : *currentShader) outfile << t;
          outfile.close();
        } else {
          std::ifstream infile;
          infile.open(outfilepath, std::ios::in | std::ios::binary);
          HY_ASSERT(infile.is_open(), "Failed to open file {}!", outfilepath);

          infile.unsetf(std::ios::skipws);
          std::streampos fileSize;
          infile.seekg(0, std::ios::end);
          fileSize = infile.tellg();
          infile.seekg(0, std::ios::beg);

          currentShader->reserve(fileSize);
          currentShader->insert(currentShader->begin(), std::istream_iterator<uint32_t>(infile), std::istream_iterator<uint32_t>());

          infile.close();
        }

        cache.UpdateCacheChecksum();
      }
    } else {
      HY_INVOKE_ERROR("Only glsl is supported for now!");
    }

    HY_LOG_INFO("Finished loading shader asset '{}'!", filepath);
  }

  const DynamicArray<uint32_t>& GetVertexShader() { return m_VertexShader; }
  const DynamicArray<uint32_t>& GetPixelShader() { return m_PixelShader; }
  const DynamicArray<uint32_t>& GetGeometryShader() { return m_GeometryShader; }
  const String& GetName() { return m_Name; }

  static const DynamicArray<const String> GetFileExtensions() { return DynamicArray<const String>{".glsl"}; }

  static bool CheckFileExtensions(const String& ext) {
    auto exts = GetFileExtensions();
    return std::find(exts.begin(), exts.end(), ext.c_str()) != exts.end();
  }

 private:
  DynamicArray<uint32_t> m_VertexShader;
  DynamicArray<uint32_t> m_PixelShader;
  DynamicArray<uint32_t> m_GeometryShader;
  String m_Name;
};
}  // namespace Hydrogen
