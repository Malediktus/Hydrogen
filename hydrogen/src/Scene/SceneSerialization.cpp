#include <Hydrogen/Scene/SceneSerialization.hpp>
#include <Hydrogen/Scene/Scene.hpp>
#include <Hydrogen/Scene/Entity.hpp>

using namespace Hydrogen;

SceneSerializer::SceneSerializer(Scene* scene) : m_Scene(scene) {}

void SceneSerializer::Serialize(std::filesystem::path filepath) {}

void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity) {}
