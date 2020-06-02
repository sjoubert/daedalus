#include "start_screen.hpp"

#include "generator.hpp"
#include "level_screen.hpp"

#include <SFML/Window/Event.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include <random>

namespace daedalus
{

StartScreen::StartScreen(sf::RenderWindow& p_window)
  : Screen(p_window)
{
}

std::unique_ptr<Screen> StartScreen::run()
{
  auto size = getWindow().getSize();
  auto maze = Generator{size.x / Cell::PIXELS, size.y / Cell::PIXELS}.primMaze();
  maze.setPosition((size.x - maze.getWidth() * Cell::PIXELS) / 2., (size.y - maze.getHeight() * Cell::PIXELS) / 2.);
  maze.clearFog();

  sf::Clock mazeClock;
  constexpr std::array directions =
    {Maze::Direction::North, Maze::Direction::South, Maze::Direction::East, Maze::Direction::West};
  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<> dist(0, directions.size());

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

    if (mazeClock.getElapsedTime() > sf::seconds(0.5))
    {
      maze.movePlayer(directions[dist(rng)]);
      mazeClock.restart();
    }

    ImGui::SFML::Update(getWindow(), deltaClock.restart());

    ImGui::Begin("start_menu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
    ImVec2 buttonWidth{200, 0};
    if (ImGui::Button("Start new game", buttonWidth))
    {
      return std::make_unique<LevelScreen>(getWindow(), 0.5f, std::pair<std::size_t, std::size_t>{10, 15});
    }
    if (ImGui::Button("Quit", buttonWidth))
    {
      getWindow().close();
    }
    ImGui::SetWindowPos({(getWindow().getSize().x - ImGui::GetWindowWidth()) / 2,
      (getWindow().getSize().y - ImGui::GetWindowHeight()) / 2.f});
    ImGui::End();

    getWindow().clear(sf::Color::Black);
    getWindow().draw(maze);
    ImGui::SFML::Render(getWindow());
    getWindow().display();
  }

  return {};
}

}
