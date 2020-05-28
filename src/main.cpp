#include "cell.hpp"
#include "generator.hpp"
#include "maze.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

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

  timer.setFillColor({200, 100, 50});
  timer.setSize({WIDTH , height * timeRatio});
  p_window.draw(timer);

  if (p_maze.hasKey())
  {
    sf::RectangleShape key({daedalus::Cell::PIXELS, daedalus::Cell::PIXELS});
    key.setFillColor(sf::Color::Yellow);
    key.setPosition({OFFSET, OFFSET});
    p_window.draw(key);
  }

  p_window.setView(saveView);
}

int main()
{
  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<std::size_t> sizeDist(10, 15);
  auto maze = daedalus::Generator{sizeDist(rng), sizeDist(rng)}.primMaze();

  sf::Clock clock;

  auto centerOnPlayer = [](sf::RenderTarget& p_target, daedalus::Cell p_player)
  {
    auto view = p_target.getView();
    view.setCenter(p_player.column * daedalus::Cell::PIXELS, p_player.row * daedalus::Cell::PIXELS);
    p_target.setView(view);
  };

  sf::RenderWindow window({1024, 640}, "Daedalus");
  centerOnPlayer(window, maze.getPlayer());
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
      else if (event.type == sf::Event::MouseWheelScrolled)
      {
        auto view = window.getView();
        view.zoom(event.mouseWheelScroll.delta < 0 ? 1.1 : 0.9);
        window.setView(view);
      }
      else if (event.type == sf::Event::KeyPressed)
      {
        switch (event.key.code)
        {
          case sf::Keyboard::Num0:
          {
            centerOnPlayer(window, maze.getPlayer());
            break;
          }
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
          case sf::Keyboard::Enter:
          {
            if (maze.hasWon())
            {
              maze = daedalus::Generator{sizeDist(rng), sizeDist(rng)}.primMaze();
              clock.restart();
              centerOnPlayer(window, maze.getPlayer());
            }
            break;
          }
          default:
            break;
        }
      }
    }

    window.clear();
    window.draw(maze);
    drawHUD(clock, window, maze);
    window.display();
  }

  return 0;
}
