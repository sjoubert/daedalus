#include "next_level_screen.hpp"

#include <SFML/Window/Event.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

namespace daedalus
{

NextLevelScreen::NextLevelScreen(sf::RenderWindow& p_window)
  : Screen(p_window)
{
}

void NextLevelScreen::run()
{
  sf::Clock deltaClock;
  while (getWindow().isOpen())
  {
    sf::Event event;
    while (getWindow().pollEvent(event))
    {
      ImGui::SFML::ProcessEvent(event);
      if (event.type == sf::Event::Closed)
      {
        getWindow().close();
      }
    }

    ImGui::SFML::Update(getWindow(), deltaClock.restart());

    ImGui::Begin("next_level_menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
    if (ImGui::Button("Next Level"))
    {
      return;
    }
    ImGui::SetWindowPos({(getWindow().getSize().x - ImGui::GetWindowWidth()) / 2,
      (getWindow().getSize().y - ImGui::GetWindowHeight()) / 2.f});
    ImGui::End();

    getWindow().clear(sf::Color::Black);
    ImGui::SFML::Render(getWindow());
    getWindow().display();
  }
}

}
