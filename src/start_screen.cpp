#include "start_screen.hpp"

#include "generator.hpp"

#include <SFML/Window/Event.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include <random>

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
  auto size = m_window.getSize();
  auto maze = daedalus::Generator{size.x / Cell::PIXELS, size.y / Cell::PIXELS}.primMaze();
  maze.setPosition(size.x % Cell::PIXELS / 2., size.y % Cell::PIXELS / 2.);
  maze.clearFog();

  sf::Clock mazeClock;
  constexpr std::array directions{Direction::North, Direction::South, Direction::East, Direction::West};
  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<> dist(0, directions.size());

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

    if (mazeClock.getElapsedTime() > sf::seconds(0.5))
    {
      maze.movePlayer(directions[dist(rng)]);
      mazeClock.restart();
    }

    ImGui::SFML::Update(m_window, deltaClock.restart());

    ImGui::Begin("start_menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
    ImVec2 buttonWidth{200, 0};
    if (ImGui::Button("Start new game", buttonWidth))
    {
      return;
    }
    if (ImGui::Button("Quit", buttonWidth))
    {
      m_window.close();
    }
    ImGui::SetWindowPos({(m_window.getSize().x - ImGui::GetWindowWidth()) / 2,
      (m_window.getSize().y - ImGui::GetWindowHeight()) / 2.f});
    ImGui::End();

    m_window.clear(sf::Color::Black);
    m_window.draw(maze);
    ImGui::SFML::Render(m_window);
    m_window.display();
  }
}

}
