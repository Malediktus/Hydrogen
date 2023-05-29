#include <Hydrogen/Core/Task.hpp>
#include <algorithm>

using namespace Hydrogen;

DynamicArray<ReferencePointer<Task>> TaskManager::s_Tasks;

ReferencePointer<Task> TaskManager::Activate(ReferencePointer<Task> task) {
    s_Tasks.push_back(task);
    task->OnActivate();
    return task;
}

void TaskManager::Deactivate(ReferencePointer<Task> task) {
    DynamicArray<ReferencePointer<Task>>::iterator position = std::find(s_Tasks.begin(), s_Tasks.end(), task);
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
