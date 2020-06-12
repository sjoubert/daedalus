#include "screen/start_screen.hpp"
#include "resources.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include <memory>

int main()
{
  sf::RenderWindow window({1366, 768}, "Daedalus");
  window.setVerticalSyncEnabled(true);
  ImGui::SFML::Init(window);
  auto& io = ImGui::GetIO();
  io.IniFilename = nullptr;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  sf::SoundBuffer bgSoundBuffer;
  bgSoundBuffer.loadFromFile(daedalus::getResource("audio/chill-tune-for-a-game.wav"));
  sf::Sound bgSound(bgSoundBuffer);
  bgSound.setLoop(true);
  bgSound.play();

  std::unique_ptr<daedalus::Screen> screen = std::make_unique<daedalus::StartScreen>(window);
  while (screen)
  {
    screen = screen->run();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
