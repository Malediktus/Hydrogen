#include <Hydrogen/Core/Task.hpp>
#include <algorithm>

using namespace Hydrogen;

std::vector<Reference<Task>> TaskManager::s_Tasks;

Reference<Task> TaskManager::Activate(Reference<Task> task) {
    s_Tasks.push_back(task);
    task->OnActivate();
    return task;
}

void TaskManager::Deactivate(Reference<Task> task) {
    std::vector<Reference<Task>>::iterator position = std::find(s_Tasks.begin(), s_Tasks.end(), task);
    if (position != s_Tasks.end()) {
        task->OnDeactivate();
        s_Tasks.erase(position);
    }
}

void TaskManager::Update() {
    for (auto task : s_Tasks) {
        task->OnUpdate();
    }
}

void TaskManager::Shutdown() {
    for (auto task : s_Tasks) {
        task->OnDeactivate();
    }
}
