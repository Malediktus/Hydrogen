#pragma once

#include <unordered_map>

#include "../Renderer/ShaderCompiler.hpp"
#include "../Core/Memory.hpp"

namespace Hydrogen {
enum class ShaderDependencyType { UniformBuffer = 0, Texture = 1 };

struct ShaderDependency {
  ShaderDependency() : Type(ShaderDependencyType::UniformBuffer), Stage(ShaderStage::VertexShader), Location(0) {}
  ~ShaderDependency() {}

  ShaderDependencyType Type;
  ShaderStage Stage;
  uint32_t Location;
  
  ReferencePointer<class UniformBuffer> UniformBuffer;
  ReferencePointer<class  Texture2D> Texture;
};

struct ShaderDependencyGraph {
  ShaderDependencyGraph(const std::initializer_list<ShaderDependency>& dependencies) : Dependencies(dependencies) {}
  DynamicArray<ShaderDependency> Dependencies;
};

class Shader {
 public:
  virtual ~Shader() = default;

  virtual void Bind(const ReferencePointer<class CommandBuffer>& commandBuffer) const = 0;

  virtual const String& GetName() const = 0;

  static ReferencePointer<Shader> Create(const ReferencePointer<class RenderDevice>& renderDevice, const ReferencePointer<class SwapChain>& swapChain,
                                         const ReferencePointer<class Framebuffer>& framebuffer, const class BufferLayout& vertexLayout, ShaderDependencyGraph dependencyGraph,
                                         const String& name, const DynamicArray<uint32_t>& vertexSrc, const DynamicArray<uint32_t>& fragmentSrc,
                                         const DynamicArray<uint32_t>& geometrySrc);
};

class ShaderLibrary {
 public:
  void Add(const String& name, const ReferencePointer<class Shader>& shader);
  void Add(const ReferencePointer<class Shader>& shader);
  ReferencePointer<class Shader> Load(const ReferencePointer<class RenderDevice>& renderDevice, const ReferencePointer<class SwapChain>& swapChain,
                                      const ReferencePointer<class Framebuffer>& framebuffer,
                                const class BufferLayout& vertexLayout, ShaderDependencyGraph dependencyGraph, const String& name, const DynamicArray<uint32_t>& vertexSrc,
                                const DynamicArray<uint32_t>& fragmentSrc, const DynamicArray<uint32_t>& geometrySrc);

  ReferencePointer<class Shader> Get(const String& name);

  bool Exists(const String& name) const;

 private:
  std::unordered_map<String, ReferencePointer<class Shader>> m_Shaders;
};
}  // namespace Hydrogen
