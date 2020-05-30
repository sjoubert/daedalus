#include "next_level_screen.hpp"

#include <SFML/Window/Event.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

namespace daedalus
{

NextLevelScreen::NextLevelScreen(sf::RenderWindow& p_window)
  : m_window(p_window)
  , m_savedView(m_window.getView())
{
  m_window.setView(m_window.getDefaultView());
}

NextLevelScreen::~NextLevelScreen()
{
  m_window.setView(m_savedView);
}

void NextLevelScreen::run()
{
  sf::Clock deltaClock;
  while (m_window.isOpen())
  {
    sf::Event event;
    while (m_window.pollEvent(event))
    {
      ImGui::SFML::ProcessEvent(event);
      if (event.type == sf::Event::Closed)
      {
        m_window.close();
      }
    }

    ImGui::SFML::Update(m_window, deltaClock.restart());

    ImGui::Begin("next_level_menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
    if (ImGui::Button("Next Level"))
    {
      return;
    }
    ImGui::SetWindowPos({(m_window.getSize().x - ImGui::GetWindowWidth()) / 2,
      (m_window.getSize().y - ImGui::GetWindowHeight()) / 2.f});
    ImGui::End();

    m_window.clear(sf::Color::Black);
    ImGui::SFML::Render(m_window);
    m_window.display();
  }
}

}
