#pragma once

#include "../Core/Assert.hpp"
#include "../Core/Memory.hpp"

namespace Hydrogen {
class CommandBuffer;
class RenderDevice;

enum class ShaderDataType { None = 0, Float = 1, Float2 = 2, Float3 = 3, Float4 = 4, Mat3 = 5, Mat4 = 6, Int = 7, Int2 = 8, Int3 = 9, Int4 = 10, Bool = 11 };

namespace Utils {
static uint32_t ShaderDataTypeSize(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::Float:
      return 4;
    case ShaderDataType::Float2:
      return 4 * 2;
    case ShaderDataType::Float3:
      return 4 * 3;
    case ShaderDataType::Float4:
      return 4 * 4;
    case ShaderDataType::Mat3:
      return 4 * 3 * 3;
    case ShaderDataType::Mat4:
      return 4 * 4 * 4;
    case ShaderDataType::Int:
      return 4;
    case ShaderDataType::Int2:
      return 4 * 2;
    case ShaderDataType::Int3:
      return 4 * 3;
    case ShaderDataType::Int4:
      return 4 * 4;
    case ShaderDataType::Bool:
      return 1;
    default:
      HY_ASSERT_CHECK(false, "Invalid shader data type");
  }
  return 0;
}
}  // namespace Utils

struct BufferElement {
  String Name;
  ShaderDataType Type;
  uint32_t Size;
  uint64_t Offset;
  bool Normalized;

  BufferElement() : Type(ShaderDataType::Float), Size(Utils::ShaderDataTypeSize(ShaderDataType::Float)), Offset(0), Normalized(false) {}

  BufferElement(ShaderDataType type, const String& name, bool normalized = false)
      : Name(name), Type(type), Size(Utils::ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

  uint32_t GetComponentCount() const {
    switch (Type) {
      case ShaderDataType::Float:
        return 1;
      case ShaderDataType::Float2:
        return 2;
      case ShaderDataType::Float3:
        return 3;
      case ShaderDataType::Float4:
        return 4;
      case ShaderDataType::Mat3:
        return 3 * 3;
      case ShaderDataType::Mat4:
        return 4 * 4;
      case ShaderDataType::Int:
        return 1;
      case ShaderDataType::Int2:
        return 2;
      case ShaderDataType::Int3:
        return 3;
      case ShaderDataType::Int4:
        return 4;
      case ShaderDataType::Bool:
        return 1;
      default:
        HY_ASSERT_CHECK(false, "Invalid shader data type");
    }

    return 0;
  }
};

class BufferLayout {
 public:
  BufferLayout() {}

  BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements) { CalculateOffsetsAndStride(); }

  inline uint32_t GetStride() const { return m_Stride; }
  inline const DynamicArray<BufferElement>& GetElements() const { return m_Elements; }

  DynamicArray<BufferElement>::iterator begin() { return m_Elements.begin(); }
  DynamicArray<BufferElement>::iterator end() { return m_Elements.end(); }
  DynamicArray<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
  DynamicArray<BufferElement>::const_iterator end() const { return m_Elements.end(); }

 private:
  void CalculateOffsetsAndStride() {
    uint64_t offset = 0;
    m_Stride = 0;
    for (auto& element : m_Elements) {
      element.Offset = offset;
      offset += element.Size;
      m_Stride += element.Size;
    }
  }

  DynamicArray<BufferElement> m_Elements;
  uint32_t m_Stride = 0;
};

class VertexBuffer {
 public:
  virtual ~VertexBuffer() = default;

  virtual void Bind(const ReferencePointer<CommandBuffer>& commandBuffer) const = 0;

  virtual const BufferLayout& GetLayout() const = 0;
  virtual void SetLayout(const BufferLayout& layout) = 0;

  static ReferencePointer<VertexBuffer> Create(const ReferencePointer<RenderDevice>& device, float* vertices, size_t size);
};

class IndexBuffer {
 public:
  virtual ~IndexBuffer() = default;

  virtual void Bind(const ReferencePointer<CommandBuffer>& commandBuffer) const = 0;
  virtual size_t GetCount() const = 0;

  static ReferencePointer<IndexBuffer> Create(const ReferencePointer<RenderDevice>& device, uint32_t* indices, size_t size);
};

class UniformBuffer {
 public:
  virtual ~UniformBuffer() = default;

  virtual void SetData(void* data) = 0;

  static ReferencePointer<UniformBuffer> Create(const ReferencePointer<RenderDevice>& device, size_t size);
};
}  // namespace Hydrogen
