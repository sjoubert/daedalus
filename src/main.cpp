#include "cell.hpp"
#include "generator.hpp"
#include "maze.hpp"
#include "next_level_screen.hpp"
#include "start_screen.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include <utility>
#include <random>

static constexpr int OFFSET = 10;

void drawHUD(sf::Clock const& p_clock, sf::RenderWindow& p_window, daedalus::Maze const& p_maze)
{
  auto const timeRatio = p_clock.getElapsedTime().asSeconds() / (p_maze.getWidth() * p_maze.getHeight());
  if (timeRatio > 1)
  {
    p_window.close();
  }

  auto saveView = p_window.getView();
  p_window.setView(p_window.getDefaultView());

  constexpr int WIDTH = 25;
  auto const height = p_window.getSize().y - 2.0f * OFFSET;

  sf::RectangleShape timer({WIDTH, height});
  timer.setPosition(p_window.getSize().x - WIDTH - OFFSET, OFFSET);

  timer.setFillColor({130, 200, 50});
  p_window.draw(timer);

  if (timeRatio < 0.33)
  {
    timer.setFillColor({255, 230, 120});
  }
  else if (timeRatio < 0.66)
  {
    timer.setFillColor({200, 100, 50});
  }
  else
  {
    timer.setFillColor({240, 80, 90});
  }
  timer.setSize({WIDTH , height * timeRatio});
  p_window.draw(timer);

  sf::RectangleShape itemStatus({daedalus::Cell::PIXELS, daedalus::Cell::PIXELS});
  // Door
  itemStatus.setFillColor(p_maze.doorIsOpen() ? sf::Color::Green : sf::Color::Red);
  itemStatus.setPosition({OFFSET, OFFSET});
  p_window.draw(itemStatus);
  // Bonus
  if (p_maze.hasBonus())
  {
    itemStatus.setFillColor(sf::Color::Blue);
  }
  else
  {
    itemStatus.setFillColor(sf::Color::Transparent);
    itemStatus.setOutlineColor(sf::Color::Blue);
    itemStatus.setOutlineThickness(2);
  }
  itemStatus.move(WIDTH + OFFSET, 0);
  p_window.draw(itemStatus);


  p_window.setView(saveView);
}

int main()
{
  sf::RenderWindow window({1024, 640}, "Daedalus");
  ImGui::SFML::Init(window);
  auto& io = ImGui::GetIO();
  io.IniFilename = nullptr;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  {
    daedalus::StartScreen start(window);
    start.run();
  }

  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<std::size_t> sizeDist(10, 15);
  auto maze = daedalus::Generator{sizeDist(rng), sizeDist(rng)}.primMaze();

  auto centerView = [](sf::RenderTarget& p_target, sf::Vector2f p_center)
  {
    auto view = p_target.getView();
    view.setCenter(p_center);
    p_target.setView(view);
  };
  centerView(window, maze.getVisibleCenter());

  sf::Clock clock;
  sf::Clock deltaClock;
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
      else if (event.type == sf::Event::KeyPressed)
      {
        switch (event.key.code)
        {
          case sf::Keyboard::Up:
          {
            maze.movePlayer(daedalus::Direction::North);
            break;
          }
          case sf::Keyboard::Down:
          {
            maze.movePlayer(daedalus::Direction::South);
            break;
          }
          case sf::Keyboard::Right:
          {
            maze.movePlayer(daedalus::Direction::East);
            break;
          }
          case sf::Keyboard::Left:
          {
            maze.movePlayer(daedalus::Direction::West);
            break;
          }
          case sf::Keyboard::Space:
          {
            if (maze.hasWon())
            {
              {
                daedalus::NextLevelScreen nextLevelScreen(window);
                nextLevelScreen.run();
              }

              maze = daedalus::Generator{sizeDist(rng), sizeDist(rng)}.primMaze();
              clock.restart();
              centerView(window, maze.getVisibleCenter());
            }
            break;
          }
          default:
            break;
        }
      }
    }

    window.clear();

    auto const maxDelta = 0.01f * deltaClock.getElapsedTime().asSeconds();
    auto deltaCenter = maze.getVisibleCenter() - window.getView().getCenter();
    deltaCenter = {std::clamp(deltaCenter.x, -maxDelta, maxDelta), std::clamp(deltaCenter.y, -maxDelta, maxDelta)};
    centerView(window, window.getView().getCenter() + deltaCenter);
    window.draw(maze);

    drawHUD(clock, window, maze);

    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
