#pragma once

#include "Memory.hpp"
#include "UUID.hpp"
#include <vector>

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
    static Reference<Task> Activate(Reference<Task> task);
    static void Deactivate(Reference<Task> task);

    static void Update();
    static void Shutdown();

private:
    static std::vector<Reference<Task>> s_Tasks;
};
} // namespace Hydrogen
