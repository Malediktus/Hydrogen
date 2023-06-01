#pragma once

#include "../../Renderer/Context.hpp"

namespace Hydrogen::Vulkan {
class VulkanContext : public Context {
public:
    VulkanContext(const ReferencePointer<RenderWindow>& window, ProjectInformation) : m_Window(window) {
    }
    virtual ~VulkanContext() = default;

    virtual void Init() override;
    virtual void Destroy() override {
    }

    virtual ReferencePointer<RenderWindow> GetWindow() override {
        return m_Window;
    }

private:
    ReferencePointer<RenderWindow> m_Window;
};
} // namespace Hydrogen::Vulkan
