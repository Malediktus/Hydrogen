#pragma once

#include <functional>

#include "../Core/Memory.hpp"

namespace Hydrogen {
enum class RenderDeviceType { Unknown = 0, IntegratedGPU = 1, DiscreteGPU = 2, VirtualGPU = 3, CPU = 4 };

struct RenderDeviceHeap {
  std::size_t MemorySize;
};

struct RenderDeviceProperties {
  uint32_t VendorID;
  uint32_t DeviceID;
  RenderDeviceType DeviceType;
  String DeviceName;
  DynamicArray<RenderDeviceHeap> MemoryHeaps;
  bool GeometryShaderSupport;
  bool TesslationShaderSupport;
};

class RenderDevice {
 public:
  virtual ~RenderDevice() = default;

  virtual bool ScreenSupported(const ReferencePointer<class RenderWindow>& window) = 0;
  virtual void WaitForIdle() = 0;

  static ReferencePointer<RenderDevice> Create(std::function<std::size_t(const RenderDeviceProperties&)> deviceRateFunction);
};
}  // namespace Hydrogen
