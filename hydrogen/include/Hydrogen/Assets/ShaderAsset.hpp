#pragma once

#include "Asset.hpp"
#include "../Core/Cache.hpp"
#include "../Renderer/Shader.hpp"
#include "../Renderer/ShaderCompiler.hpp"
#include <ostream>
#include <sstream>
#include <functional>

namespace Hydrogen {
class ShaderAsset : public Asset {
public:
    ShaderAsset() {
        m_AssetInfo.Preload = true;
    }

    void Load(const String& filepath) override {
        HY_ASSERT(!filepath.empty(), "Parameter 'filepath' of type 'const String&' in function ShaderAsset::Load(const String& filepath) is an empty string!");
        HY_LOG_INFO("Loading shader asset '{}'!", filepath);

        DynamicArray<uint32_t> vertexShader;
        DynamicArray<uint32_t> pixelShader;
        DynamicArray<uint32_t> geometryShader;

        if (filepath.substr(filepath.find_last_of(".") + 1) == "glsl") {
            for (const auto& dirEntry : std::filesystem::directory_iterator(filepath)) {
                ShaderStage stage;
                DynamicArray<uint32_t>* currentShader;

                if (dirEntry.path().extension().string() == ".vert") {
                    currentShader = &vertexShader;
                    stage = ShaderStage::VertexShader;
                } else if (dirEntry.path().extension().string() == ".frag") {
                    currentShader = &pixelShader;
                    stage = ShaderStage::PixelShader;
                } else if (dirEntry.path().extension().string() == ".geo") {
                    currentShader = &geometryShader;
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
                    for (auto t : *currentShader)
                        outfile << t;
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

            m_Shader = Shader::Create(FILENAME_FROM_PATH(filepath), vertexShader, pixelShader, geometryShader);
        } else {
            HY_INVOKE_ERROR("Only glsl is supported for now!");
        }

        HY_LOG_INFO("Finished loading shader asset '{}'!", filepath);
    }

    const ReferencePointer<Shader>& GetShader() {
        HY_ASSERT(m_Shader, "ShaderAsset::GetShader() was about to return null! ShaderAsset::Load(const String& filename) shall be called first!");
        return m_Shader;
    }

    static const DynamicArray<const String> GetFileExtensions() {
        return DynamicArray<const String> {".glsl"};
    }

    static bool CheckFileExtensions(const String& ext) {
        auto exts = GetFileExtensions();
        return std::find(exts.begin(), exts.end(), ext.c_str()) != exts.end();
    }

private:
    ReferencePointer<Shader> m_Shader;
};
} // namespace Hydrogen
