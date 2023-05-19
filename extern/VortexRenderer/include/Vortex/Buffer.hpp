#pragma once

#include "Core.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace Vortex {
/**
 * @brief This enum represents data types used in shaders. This is useful for specifying the vertex layout.
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
enum class ShaderDataType { None = 0, Float = 1, Float2 = 2, Float3 = 3, Float4 = 4, Mat3 = 5, Mat4 = 6, Int = 7, Int2 = 8, Int3 = 9, Int4 = 10, Bool = 11 };

namespace Utils {
/**
 * @brief This is a helper functions for retrieving the sizes for shader data types
 *
 * @param type
 * @return uint32_t
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
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
        VT_ASSERT_CHECK(false, "Invalid shader data type");
    }
    return 0;
}
} // namespace Utils

/**
 * @brief A BufferElement specifies how an element in the vertex layout looks.
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
struct BufferElement {
    std::string Name;
    ShaderDataType Type;
    uint32_t Size;
    uint64_t Offset;
    bool Normalized;

    BufferElement()
        : Type(ShaderDataType::Float), Size(Utils::ShaderDataTypeSize(ShaderDataType::Float)), Offset(0), Normalized(false) {
    }

    BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
        : Name(name), Type(type), Size(Utils::ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {
    }

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
            VT_ASSERT_CHECK(false, "Invalid shader data type");
        }

        return 0;
    }
};

/**
 * @brief The BufferLayout is used for setting the vertex layout
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
class BufferLayout {
public:
    VT_API BufferLayout() {
    }

    VT_API BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements) {
        CalculateOffsetsAndStride();
    }

    VT_API inline uint32_t GetStride() const {
        return m_Stride;
    }
    VT_API inline const std::vector<BufferElement>& GetElements() const {
        return m_Elements;
    }

    VT_API std::vector<BufferElement>::iterator begin() {
        return m_Elements.begin();
    }
    VT_API std::vector<BufferElement>::iterator end() {
        return m_Elements.end();
    }
    VT_API std::vector<BufferElement>::const_iterator begin() const {
        return m_Elements.begin();
    }
    VT_API std::vector<BufferElement>::const_iterator end() const {
        return m_Elements.end();
    }

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

    std::vector<BufferElement> m_Elements;
    uint32_t m_Stride = 0;
};

/**
 * @brief The VertexBuffer class holds vertex data and can be submitted to a VertexArray and then get rendered. A VertexBuffer
 * also own a BufferLayout (with getter and setter) which describes its layout
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
class VertexBuffer {
public:
    virtual VT_API ~VertexBuffer() {
    }

    virtual VT_API void Bind() const = 0;
    virtual VT_API void Unbind() const = 0;

    virtual void SetData(const void* data, uint32_t size) = 0;

    virtual VT_API const BufferLayout& GetLayout() const = 0;
    virtual VT_API void SetLayout(const BufferLayout& layout) = 0;
};

/**
 * @brief The IndexBuffer holds index data (which is just an unsigned int array) and can get submitted to a VertexArray along with
 * a VertexBuffer.
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
class IndexBuffer {
public:
    virtual ~IndexBuffer() {
    }

    virtual VT_API void Bind() const = 0;
    virtual VT_API void Unbind() const = 0;

    virtual uint32_t GetCount() const = 0;
};

/**
 * @brief This function is the only way that should be used for creating a VertexBuffer from vertex data.
 *
 * @param vertices
 * @param size
 * @return std::shared_ptr<VertexBuffer>
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
VT_API std::shared_ptr<VertexBuffer> VertexBufferCreate(float* vertices, uint32_t size);
/**
 * @brief This function is the only way that should be used for creating a dynamic VertexBuffer.
 *
 * @param size
 * @return std::shared_ptr<VertexBuffer>
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-04-01
 * @copyright Copyright (c) 2023
 */
VT_API std::shared_ptr<VertexBuffer> VertexBufferCreate(uint32_t size);
/**
 * @brief This function is the only way that should be used for creating an IndexBuffer.
 *
 * @param indices
 * @param size
 * @return std::shared_ptr<IndexBuffer>
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
VT_API std::shared_ptr<IndexBuffer> IndexBufferCreate(uint32_t* indices, uint32_t size);
} // namespace Vortex
