#pragma once

#include <unordered_map>

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"
#include "../Math/Math.hpp"
#include "ShaderCompiler.hpp"
#include "Buffer.hpp"
#include "CommandBuffer.hpp"
#include "RenderDevice.hpp"
#include "Framebuffer.hpp"
#include "SwapChain.hpp"

namespace Hydrogen {
enum class ShaderDependencyType { UniformBuffer = 0 };

struct ShaderDependency {
  ShaderDependencyType Type;
  ShaderStage Stage;
  
  ReferencePointer<UniformBuffer> UniformBuffer;
};

struct ShaderDependencyGraph {
  ShaderDependencyGraph(const std::initializer_list<ShaderDependency>& dependencies) : Dependencies(dependencies) {}
  DynamicArray<ShaderDependency> Dependencies;
};

class Shader {
 public:
  virtual ~Shader() = default;

  virtual void Bind(const ReferencePointer<CommandBuffer>& commandBuffer) const = 0;

  virtual const String& GetName() const = 0;

  static ReferencePointer<Shader> Create(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain,
                                         const ReferencePointer<Framebuffer>& framebuffer, const BufferLayout& vertexLayout, ShaderDependencyGraph dependencyGraph,
                                         const String& name, const DynamicArray<uint32_t>& vertexSrc, const DynamicArray<uint32_t>& fragmentSrc,
                                         const DynamicArray<uint32_t>& geometrySrc);
};

class ShaderLibrary {
 public:
  void Add(const String& name, const ReferencePointer<Shader>& shader);
  void Add(const ReferencePointer<Shader>& shader);
  ReferencePointer<Shader> Load(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Framebuffer>& framebuffer,
                                const BufferLayout& vertexLayout, ShaderDependencyGraph dependencyGraph, const String& name, const DynamicArray<uint32_t>& vertexSrc,
                                const DynamicArray<uint32_t>& fragmentSrc, const DynamicArray<uint32_t>& geometrySrc);

  ReferencePointer<Shader> Get(const String& name);

  bool Exists(const String& name) const;

 private:
  std::unordered_map<String, ReferencePointer<Shader>> m_Shaders;
};
}  // namespace Hydrogen
