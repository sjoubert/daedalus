#include "cell.hpp"
#include "maze.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>

#include <utility>

int main()
{
  daedalus::Maze maze(16, 9);

  sf::VideoMode const videoMode(maze.getWidth() * daedalus::Cell::PIXELS, maze.getHeight() * daedalus::Cell::PIXELS);
  sf::RenderWindow window(videoMode, "Daedalus");
  auto const playerRadius = - daedalus::Cell::PIXELS / 2 * 0.7;
  sf::CircleShape player(playerRadius);
  player.setOrigin(playerRadius, playerRadius);
  player.setFillColor(sf::Color::Blue);

  auto playerInitPosition = []() -> std::pair<std::size_t, std::size_t>
  {
    return {0, 0};
  };
  auto [playerRow, playerCol] = playerInitPosition();

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
      else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num0)
      {
        window.setView(window.getDefaultView());
      }

      window.clear(sf::Color(100, 100, 100));

      maze.draw(window);
      player.setPosition((playerCol + 0.5) * daedalus::Cell::PIXELS, (playerRow + 0.5) * daedalus::Cell::PIXELS);
      window.draw(player);

      window.display();
    }
  }

  return 0;
}
