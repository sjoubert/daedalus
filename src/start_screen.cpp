#include "start_screen.hpp"

#include <SFML/Window/Event.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

namespace daedalus
{

StartScreen::StartScreen(sf::RenderWindow& p_window)
  : m_window(p_window)
  , m_savedView(m_window.getView())
{
  m_window.setView(m_window.getDefaultView());
}

StartScreen::~StartScreen()
{
  m_window.setView(m_savedView);
}

void StartScreen::run()
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

    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
    ImVec2 buttonWidth{200, 0};
    if (ImGui::Button("Start new game", buttonWidth))
    {
      return;
    }
    ImGui::SetItemDefaultFocus();
    if (ImGui::Button("Quit", buttonWidth))
    {
      m_window.close();
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
