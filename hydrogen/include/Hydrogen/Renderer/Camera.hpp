#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <tracy/Tracy.hpp>

#include "../Math/Math.hpp"

namespace Hydrogen {
class Camera {
 public:
  Camera(float fov, float width, float height) {
    ZoneScoped;
    m_Projection = glm::perspective(fov / 2.0f, width / height, 0.1f, 1000.0f);
    m_View = Matrix4(1.0f);
    m_Position = Vector3(0.0f);
    UpdateMatrices();
  }

  void Resize(float fov, float width, float height) {
    ZoneScoped;
    m_Projection = glm::perspective(fov / 2.0f, width / height, 0.1f, 1000.0f);
    UpdateMatrices();
  }

  virtual ~Camera() = default;

  const Matrix4& GetViewProj() { return m_ViewProj; }

  const Vector3& GetPosition() { return m_Position; }

  virtual void Translate(Vector3 v) {
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

  Vector3 m_Position;
  Matrix4 m_Projection;
  Matrix4 m_View;
  Matrix4 m_ViewProj;
};
}  // namespace Hydrogen
