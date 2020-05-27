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
              centerOnPlayer(window, maze.getPlayer());
            }
            break;
          }
          default:
            break;
        }
      }

      window.clear(sf::Color::Black);
      window.draw(maze);
      window.display();
    }
  }

  return 0;
}
