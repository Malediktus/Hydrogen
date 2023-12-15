#include <Hydrogen/Assets/SpriteAsset.hpp>
#include <Hydrogen/Renderer/RenderDevice.hpp>
#include <Hydrogen/Renderer/Texture.hpp>

using namespace Hydrogen;

void SpriteAsset::Load(const std::filesystem::path& filepath) {
  HY_ASSERT(!filepath.empty(),
            "Parameter 'filepath' of type 'const String&' in function "
            "SpriteAsset::Load(const String& filepath) is an empty string!");
  HY_LOG_INFO("Loading sprite asset '{}'!", filepath.string());
  m_Pixels = stbi_load(filepath.string().c_str(), (int*)&m_Width, (int*)&m_Height, (int*)&m_Channels, STBI_rgb_alpha);
  HY_ASSERT(m_Pixels, "Failed to load sprite {}", filepath.string());
  HY_LOG_INFO("Finished loading sprite asset '{}'!", filepath.string());

  m_Texture = Texture2D::Create(m_Width, m_Height, m_Pixels);
}
