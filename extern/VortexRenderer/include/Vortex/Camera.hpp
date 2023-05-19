#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <tracy/Tracy.hpp>

namespace Vortex {
class Camera {
public:
    Camera(float fov, float width, float height) {
        ZoneScoped;
        m_Projection = glm::perspective(fov / 2.0f, width / height, 0.1f, 1000.0f);
        m_View = glm::mat4(1.0f);
        m_Position = glm::vec3(0.0f);
        UpdateMatrices();
    }

    void Resize(float fov, float width, float height) {
        ZoneScoped;
        m_Projection = glm::perspective(fov / 2.0f, width / height, 0.1f, 1000.0f);
        UpdateMatrices();
    }

    virtual ~Camera() = default;

    const glm::mat4& GetViewProj() {
        return m_ViewProj;
    }

    const glm::vec3& GetPosition() {
        return m_Position;
    }

    virtual void Translate(glm::vec3 v) {
        ZoneScoped;
        m_Position += v;
        m_View = glm::translate(m_View, v * -1.0f);
        UpdateMatrices();
    }

protected:
    virtual void UpdateMatrices() {
        ZoneScoped;
        m_ViewProj = m_Projection * m_View;
    }

    glm::vec3 m_Position;
    glm::mat4 m_Projection;
    glm::mat4 m_View;
    glm::mat4 m_ViewProj;
};
} // namespace Vortex
