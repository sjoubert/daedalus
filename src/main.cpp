#include "screen/start_screen.hpp"
#include "resources.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio/Music.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include <memory>

int main(int, char** p_argv)
{
  daedalus::initResourceDir(p_argv[0]);

  sf::RenderWindow window({1366, 768}, "Daedalus");
  window.setVerticalSyncEnabled(true);
  ImGui::SFML::Init(window);
  auto& io = ImGui::GetIO();
  io.IniFilename = nullptr;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  sf::Music bgMusic;
  bgMusic.openFromFile(daedalus::getResource("audio/chill-tune-for-a-game.wav"));
  bgMusic.setLoop(true);
  bgMusic.play();

  std::unique_ptr<daedalus::Screen> screen = std::make_unique<daedalus::StartScreen>(window);
  while (screen)
  {
    screen = screen->run();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
