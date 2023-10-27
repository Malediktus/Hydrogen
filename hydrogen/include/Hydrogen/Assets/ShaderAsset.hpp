#pragma once

#include <functional>
#include <iostream>
#include <ostream>
#include <sstream>

#include "../Core/Base.hpp"
#include "../Core/Assert.hpp"
#include "Asset.hpp"

namespace Hydrogen {
class ShaderAsset : public Asset {
 public:
  ShaderAsset() { m_AssetInfo.Preload = true; }

  void Load(const std::filesystem::path& filepath) override;
  ReferencePointer<class Shader> CreateShader(const ReferencePointer<class RenderDevice>& renderDevice, const ReferencePointer<class SwapChain>& swapChain,
                                              const ReferencePointer<class Framebuffer>& framebuffer, const class BufferLayout& vertexLayout,
                                              const struct ShaderDependencyGraph dependencyGraph);

  const DynamicArray<uint32_t>& GetVertexShader() { return m_VertexShader; }
  const DynamicArray<uint32_t>& GetPixelShader() { return m_FragmentShader; }
  const DynamicArray<uint32_t>& GetGeometryShader() { return m_GeometryShader; }
  const String& GetName() { return m_Name; }

  static const DynamicArray<String> GetFileExtensions() { return DynamicArray<String>{".glsl"}; }

  static bool CheckFileExtensions(const String& ext) {
    auto exts = GetFileExtensions();
    return std::find(exts.begin(), exts.end(), ext.c_str()) != exts.end();
  }

 private:
  DynamicArray<uint32_t> m_VertexShader;
  DynamicArray<uint32_t> m_FragmentShader;
  DynamicArray<uint32_t> m_GeometryShader;
  String m_Name;
};
}  // namespace Hydrogen
