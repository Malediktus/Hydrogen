#pragma once

#include <filesystem>
#include <yaml-cpp/yaml.h>

namespace Hydrogen {
class Scene;

class SceneSerializer {
 public:
  SceneSerializer(Scene* scene);
  ~SceneSerializer() = default;

  void Serialize(std::filesystem::path filepath);

 private:
  void SerializeEntity(YAML::Emitter& out, class Entity entity);

  Scene* m_Scene;
};

class SceneDeserializer {};
}  // namespace Hydrogen
