#pragma once

#include <unordered_map>

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"
#include "../Math/Math.hpp"
#include "RenderDevice.hpp"
#include "RenderPass.hpp"
#include "SwapChain.hpp"

namespace Hydrogen {
class Shader {
 public:
  virtual ~Shader() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual const String& GetName() const = 0;

  static ReferencePointer<Shader> Create(ReferencePointer<RenderDevice> renderDevice, const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<RenderPass>& renderPass,
                                         const String& name, const DynamicArray<uint32_t>& vertexSrc, const DynamicArray<uint32_t>& fragmentSrc,
                                         const DynamicArray<uint32_t>& geometrySrc);
};

class ShaderLibrary {
 public:
  void Add(const String& name, const ReferencePointer<Shader>& shader);
  void Add(const ReferencePointer<Shader>& shader);
  ReferencePointer<Shader> Load(ReferencePointer<RenderDevice> renderDevice, const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<RenderPass>& renderPass,
                                const String& name, const DynamicArray<uint32_t>& vertexSrc, const DynamicArray<uint32_t>& fragmentSrc, const DynamicArray<uint32_t>& geometrySrc);

  ReferencePointer<Shader> Get(const String& name);

  bool Exists(const String& name) const;

 private:
  std::unordered_map<String, ReferencePointer<Shader>> m_Shaders;
};
}  // namespace Hydrogen
