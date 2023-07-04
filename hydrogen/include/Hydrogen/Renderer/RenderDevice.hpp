#pragma once

#include <memory>

#include "../Core/Assert.hpp"
#include "../Core/Base.hpp"
#include "../Core/Memory.hpp"
#include "RenderWindow.hpp"

namespace Hydrogen {
enum class RenderDeviceType {
  Unknown = 0,
  IntegratedGPU = 1,
  DiscreteGPU = 2,
  VirtualGPU = 3,
  CPU = 4
};

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
  ~RenderDevice() = default;

  static ReferencePointer<RenderDevice> Create(
      std::function<std::size_t(const RenderDeviceProperties&)>
          deviceRateFunction);
};
}  // namespace Hydrogen
