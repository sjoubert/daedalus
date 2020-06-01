#include "next_level_screen.hpp"

#include <SFML/Window/Event.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

namespace daedalus
{

NextLevelScreen::NextLevelScreen(sf::RenderWindow& p_window, bool p_withBonus)
  : Screen(p_window)
  , m_withBonus(p_withBonus)
{
}

void NextLevelScreen::run()
{
  float nextLevelButtonSpacing{};

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

    ImGui::SetNextWindowSize({getWindow().getSize().x / 2.f, -1});
    ImGui::Begin("next_level_menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

    ImGui::Columns(2, nullptr, false);

    if (m_withBonus)
    {
      ImGui::Text("Bonus:");
      ImGui::Bullet();
      ImGui::Selectable("Allowed time increase", true);
    }
    else
    {
      ImGui::Text("No bonus found!");
    }

    ImGui::NextColumn();

    ImGui::Text("Malus:");
    ImGui::Bullet();
    ImGui::Selectable("Maze size increase", true);

    ImGui::Columns(1);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Dummy({nextLevelButtonSpacing, -1});
    ImGui::SameLine();
    if (ImGui::Button("Next Level"))
    {
      return;
    }
    nextLevelButtonSpacing = (ImGui::GetWindowContentRegionWidth() - ImGui::GetItemRectSize().x) / 2.f;

    ImGui::SetWindowPos({(getWindow().getSize().x - ImGui::GetWindowWidth()) / 2,
      (getWindow().getSize().y - ImGui::GetWindowHeight()) / 2.f});
    ImGui::End();

    getWindow().clear(sf::Color::Black);
    ImGui::SFML::Render(getWindow());
    getWindow().display();
  }
}

}
