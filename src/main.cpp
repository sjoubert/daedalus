#include "cell.hpp"
#include "generator.hpp"
#include "maze.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <utility>
#include <random>

int main()
{
  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<std::size_t> sizeDist(10, 15);

  auto maze = daedalus::Generator{sizeDist(rng), sizeDist(rng)}.primMaze();

  sf::RenderWindow window({1024, 640}, "Daedalus");
  auto defaultView = window.getDefaultView();
  defaultView.setCenter(maze.getWidth() * daedalus::Cell::PIXELS / 2, maze.getHeight() * daedalus::Cell::PIXELS / 2);
  window.setView(defaultView);

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
            auto view = window.getDefaultView();
            view.setCenter(maze.getWidth() * daedalus::Cell::PIXELS / 2, maze.getHeight() * daedalus::Cell::PIXELS / 2);
            window.setView(view);
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
          default:
            break;
        }
      }

      window.clear(sf::Color::Black);
      window.draw(maze);
      window.display();

      if (maze.hasWon())
      {
        maze = daedalus::Generator{sizeDist(rng), sizeDist(rng)}.primMaze();
        auto view = window.getView();
        view.setCenter(maze.getWidth() * daedalus::Cell::PIXELS / 2, maze.getHeight() * daedalus::Cell::PIXELS / 2);
        window.setView(view);
      }
    }
  }

  return 0;
}
