#include "screen/start_screen.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include <memory>

int main()
{
  sf::RenderWindow window({1366, 768}, "Daedalus");
  ImGui::SFML::Init(window);
  auto& io = ImGui::GetIO();
  io.IniFilename = nullptr;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  std::unique_ptr<daedalus::Screen> screen = std::make_unique<daedalus::StartScreen>(window);
  while (screen)
  {
    screen = screen->run();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
