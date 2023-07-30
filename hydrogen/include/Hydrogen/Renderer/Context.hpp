#pragma once

#include <memory>

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"
#include "RenderWindow.hpp"

namespace Hydrogen {
struct ProjectInformation {
  String ProjectName;
  Vector3 ProjectVersion;
};

class Context {
 public:
  ~Context() = default;

  virtual void Init(ProjectInformation clientInfo, ProjectInformation engineInfo) = 0;

  static ReferencePointer<Context> Create(const ReferencePointer<RenderWindow>& mainWindow);
};
}  // namespace Hydrogen
