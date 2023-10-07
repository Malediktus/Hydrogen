#pragma once

#include "../Core/Memory.hpp"
#include "../Math/Math.hpp"

namespace Hydrogen {
struct ProjectInformation {
  String ProjectName;
  Vector3 ProjectVersion;
};

class Context {
 public:
  ~Context() = default;

  virtual void Init(ProjectInformation clientInfo, ProjectInformation engineInfo) = 0;

  static ReferencePointer<Context> Create(const ReferencePointer<class RenderWindow>& mainWindow);
};
}  // namespace Hydrogen
