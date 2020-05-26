#include "cell.hpp"
#include "generator.hpp"
#include "maze.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>

#include <utility>
#include <random>

int main()
{
  std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<std::size_t> sizeDist(10, 15);

  auto maze = daedalus::Generator{sizeDist(rng), sizeDist(rng)}.primMaze();

  sf::RenderWindow window({1024, 640}, "Daedalus");
  auto view = window.getDefaultView();
  view.setCenter(maze.getWidth() * daedalus::Cell::PIXELS / 2, maze.getHeight() * daedalus::Cell::PIXELS / 2);
  window.setView(view);

  auto const playerRadius = daedalus::Cell::PIXELS / 2 * 0.7;
  sf::CircleShape player(playerRadius);
  player.setOrigin(playerRadius, playerRadius);
  player.setFillColor(sf::Color::Blue);

  auto playerPos = maze.getStart();

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
            playerPos = maze.getStart();
            break;
          }
          case sf::Keyboard::Up:
          {
            if (maze.getSeparation({playerPos.row, playerPos.column}, daedalus::Direction::North) == daedalus::Separation::Empty)
            {
              --playerPos.row;
            }
            break;
          }
          case sf::Keyboard::Down:
          {
            if (maze.getSeparation({playerPos.row, playerPos.column}, daedalus::Direction::South) == daedalus::Separation::Empty)
            {
              ++playerPos.row;
            }
            break;
          }
          case sf::Keyboard::Right:
          {
            if (maze.getSeparation({playerPos.row, playerPos.column}, daedalus::Direction::East) == daedalus::Separation::Empty)
            {
              ++playerPos.column;
            }
            break;
          }
          case sf::Keyboard::Left:
          {
            if (maze.getSeparation({playerPos.row, playerPos.column}, daedalus::Direction::West) == daedalus::Separation::Empty)
            {
              --playerPos.column;
            }
            break;
          }
          default:
            break;
        }
      }

      window.clear(sf::Color::Black);

      window.draw(maze);
      player.setPosition((playerPos.column + 0.5) * daedalus::Cell::PIXELS, (playerPos.row + 0.5) * daedalus::Cell::PIXELS);
      window.draw(player);

      window.display();

      if (playerPos == maze.getEnd())
      {
        maze = daedalus::Generator{sizeDist(rng), sizeDist(rng)}.primMaze();
        auto view = window.getView();
        view.setCenter(maze.getWidth() * daedalus::Cell::PIXELS / 2, maze.getHeight() * daedalus::Cell::PIXELS / 2);
        window.setView(view);
        playerPos = maze.getStart();
      }
    }
  }

  return 0;
}
