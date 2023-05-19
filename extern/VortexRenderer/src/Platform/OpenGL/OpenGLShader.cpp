#include <Vortex/Platform/OpenGL/OpenGLShader.hpp>
#include <Vortex/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <array>
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>
#include <iostream>
#include <vector>

using namespace Vortex::OpenGL;

namespace Utils {
static GLenum ShaderTypeFromString(const std::string& type) {
    ZoneScoped;
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel")
        return GL_FRAGMENT_SHADER;
    if (type == "geometry")
        return GL_GEOMETRY_SHADER;

    VT_ASSERT_CHECK(false, "Invalid renderer shader type");

    return 0;
}

static const std::string StringFromShaderType(const GLenum type) {
    ZoneScoped;
    if (type == GL_VERTEX_SHADER)
        return "vertex";
    if (type == GL_FRAGMENT_SHADER)
        return "pixel";
    if (type == GL_GEOMETRY_SHADER)
        return "geometry";

    VT_ASSERT_CHECK(false, "Invalid renderer shader type");

    return 0;
}
} // namespace Utils

OpenGLShader::OpenGLShader(const std::string& filepath) {
    ZoneScoped;
    std::string source = ReadFile(filepath);
    auto shaderSources = PreProcess(source);
    Compile(shaderSources);

    // Extract name from filepath
    auto lastSlash = filepath.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = filepath.rfind('.');
    auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
    m_Name = filepath.substr(lastSlash, count);
}

OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc) : m_Name(name) {
    ZoneScoped;
    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertexSrc;
    sources[GL_FRAGMENT_SHADER] = fragmentSrc;
    if (!geometrySrc.empty())
        sources[GL_GEOMETRY_SHADER] = geometrySrc;
    Compile(sources);
}

OpenGLShader::~OpenGLShader() {
    ZoneScoped;
    glDeleteProgram(m_RendererID);
    glCheckError();
    // spdlog::trace("Deleted OpenGL shader program (ID: {})", m_RendererID); // Dont now why but this segfaults (maybe lifetime related)
}

std::string OpenGLShader::ReadFile(const std::string& filepath) {
    ZoneScoped;
    std::string result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    VT_ASSERT(in, "Failed to open shader file");
    in.seekg(0, std::ios::end);
    result.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&result[0], result.size());
    in.close();

    return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {
    ZoneScoped;
    std::unordered_map<GLenum, std::string> shaderSources;

    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);
    while (pos != std::string::npos) {
        size_t eol = source.find_first_of("\r\n", pos);
        VT_ASSERT(eol != std::string::npos, "Expected definition after #type instruction");
        size_t begin = pos + typeTokenLength + 1;
        std::string type = source.substr(begin, eol - begin);

        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        pos = source.find(typeToken, nextLinePos);
        shaderSources[::Utils::ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
    }

    return shaderSources;
}

void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
    ZoneScoped;
    GLuint program = glCreateProgram();
    spdlog::trace("Created OpenGL shader program (ID: {})", program);
    VT_ASSERT_CHECK(shaderSources.size() <= 3, "More than 3 shader types in file, but only 3 are supported (vertex, pixel, geometry)");
    std::array<GLenum, 3> glShaderIDs;
    int glShaderIDIndex = 0;
    for (auto& kv : shaderSources) {
        GLenum type = kv.first;
        const std::string& source = kv.second;

        GLuint shader = glCreateShader(type);
        spdlog::trace("Created OpenGL shader of type {} (ID: {})", ::Utils::StringFromShaderType(type), shader);

        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);

        glCompileShader(shader);
        spdlog::trace("Compiled OpenGL shader of type {} (ID: {})", ::Utils::StringFromShaderType(type), shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);
            spdlog::trace("Deleted OpenGL shader (ID: {})", shader);

            spdlog::error("OpenGL shader compilation failed:\n{}", infoLog.data());
            spdlog::error("Shader info: (type: {}, ID: {})", ::Utils::StringFromShaderType(type), shader);
            VT_ASSERT(false, "Shader compilation failed");
            break;
        }

        glAttachShader(program, shader);
        spdlog::trace("Attached OpenGL shader of type {} (ID: {}) to shader program (ID : {})", ::Utils::StringFromShaderType(type), shader, program);
        glShaderIDs[glShaderIDIndex++] = shader;
    }

    m_RendererID = program;

    // Link our program
    glLinkProgram(program);
    spdlog::trace("Linked OpenGL shader program (ID: {})", program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*) &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);

        for (auto id : glShaderIDs)
            glDeleteShader(id);

        spdlog::error("OpenGL shader program linking failed:\n{}", infoLog.data());
        spdlog::error("Shader program info: (ID: {})", program);

        VT_ASSERT(false, "Shader linking failed");
        return;
    }

    for (auto id : glShaderIDs) {
        glDetachShader(program, id);
        spdlog::trace("Detached OpenGL shader (ID: {}) from shader program (ID : {})", id, program);
    }

    for (auto id : glShaderIDs) {
        glDeleteShader(id);
        spdlog::trace("Deleted OpenGL shader (ID: {})", id);
    }
    glCheckError();
}

void OpenGLShader::Bind() const {
    ZoneScoped;
    glUseProgram(m_RendererID);
    glCheckError();
    spdlog::trace("Bound OpenGL shader program (ID: {})", m_RendererID);
}

void OpenGLShader::Unbind() const {
    ZoneScoped;
    glUseProgram(0);
    glCheckError();
    spdlog::trace("Unbound OpenGL shader program (ID: {})", m_RendererID);
}

void OpenGLShader::SetInt(const std::string& name, int value) {
    ZoneScoped;
    m_UniformInts[name] = value;
}

void OpenGLShader::SetFloat(const std::string& name, float value) {
    ZoneScoped;
    m_UniformFloats[name] = value;
}

void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) {
    ZoneScoped;
    m_UniformFloat2s[name] = value;
}

void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
    ZoneScoped;
    m_UniformFloat3s[name] = value;
}

void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
    ZoneScoped;
    m_UniformFloat4s[name] = value;
}

void OpenGLShader::SetMatrix3(const std::string& name, const glm::mat3& matrix) {
    ZoneScoped;
    m_UniformMatrix3s[name] = matrix;
}

void OpenGLShader::SetMatrix4(const std::string& name, const glm::mat4& matrix) {
    ZoneScoped;
    m_UniformMatrix4s[name] = matrix;
}

void OpenGLShader::UploadInt(const std::string& name, int value) {
    ZoneScoped;
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, value);
    glCheckError();
    spdlog::trace("Uploaded int to OpenGL shader program (name: {}, value: {}, ID: {})", name, value, m_RendererID);
}

void OpenGLShader::UploadFloat(const std::string& name, float value) {
    ZoneScoped;
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
    glCheckError();
    spdlog::trace("Uploaded float to OpenGL shader program (name: {}, value: {}, ID: {})", name, value, m_RendererID);
}

void OpenGLShader::UploadFloat2(const std::string& name, const glm::vec2& value) {
    ZoneScoped;
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2f(location, value.x, value.y);
    glCheckError();
    spdlog::trace("Uploaded vector2 to OpenGL shader program (name: {}, value: ({}, {}), ID: {})", name, value.x, value.y, m_RendererID);
}

void OpenGLShader::UploadFloat3(const std::string& name, const glm::vec3& value) {
    ZoneScoped;
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
    glCheckError();
    spdlog::trace("Uploaded vector3 to OpenGL shader program (name: {}, value: ({}, {}, {}), ID: {})", name, value.x, value.y, value.z, m_RendererID);
}

void OpenGLShader::UploadFloat4(const std::string& name, const glm::vec4& value) {
    ZoneScoped;
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
    glCheckError();
    spdlog::trace("Uploaded vector4 to OpenGL shader program (name: {}, value: ({}, {}, {}, {}), ID: {})", name, value.x, value.y, value.z, value.w, m_RendererID);
}

void OpenGLShader::UploadMatrix3(const std::string& name, const glm::mat3& matrix) {
    ZoneScoped;
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    glCheckError();
    spdlog::trace("Uploaded matrix3x3 to OpenGL shader program (name: {}, ID: {})", name, m_RendererID);
}

void OpenGLShader::UploadMatrix4(const std::string& name, const glm::mat4& matrix) {
    ZoneScoped;
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    glCheckError();
    spdlog::trace("Uploaded matrix4x4 to OpenGL shader program (name: {}, ID: {})", name, m_RendererID);
}

void OpenGLShader::UploadUniformQueues() {
    ZoneScoped;
    for (auto& [name, value] : m_UniformInts) {
        UploadInt(name, value);
    }
    for (auto& [name, value] : m_UniformFloats) {
        UploadFloat(name, value);
    }
    for (auto& [name, value] : m_UniformFloat2s) {
        UploadFloat2(name, value);
    }
    for (auto& [name, value] : m_UniformFloat3s) {
        UploadFloat3(name, value);
    }
    for (auto& [name, value] : m_UniformFloat4s) {
        UploadFloat4(name, value);
    }
    for (auto& [name, value] : m_UniformMatrix3s) {
        UploadMatrix3(name, value);
    }
    for (auto& [name, value] : m_UniformMatrix4s) {
        UploadMatrix4(name, value);
    }
}
