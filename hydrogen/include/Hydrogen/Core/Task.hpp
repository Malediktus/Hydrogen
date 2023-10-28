#pragma once

#include <vector>

#include "Memory.hpp"

namespace Hydrogen {
class Task {
 public:
  virtual ~Task() = default;
  virtual void OnActivate() = 0;
  virtual void OnUpdate() = 0;
  virtual void OnDeactivate() = 0;
};

class TaskManager {
 public:
  static ReferencePointer<Task> Activate(ReferencePointer<Task> task);
  static void Deactivate(ReferencePointer<Task> task);

  static void Update();
  static void Shutdown();

 private:
  static DynamicArray<ReferencePointer<Task>> s_Tasks;
};
}  // namespace Hydrogen
