#include "maze.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>

int main()
{
  constexpr int CELL_SIZE = 32;

  daedalus::Maze maze(16, 9);

  sf::RenderWindow window(sf::VideoMode(maze.getWidth() * CELL_SIZE, maze.getHeight() * CELL_SIZE), "Daedalus");
  sf::RectangleShape wall(sf::Vector2f(CELL_SIZE, 4));
  wall.setFillColor(sf::Color::Black);

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

      // Row separations
      wall.setRotation(0);
      for (auto row = 1u; row < maze.getHeight(); ++row)
      {
        for (auto col = 0u; col < maze.getWidth(); ++col)
        {
          if (maze.getSeparation({row, col}, daedalus::Direction::North) == daedalus::Separation::Wall)
          {
            wall.setPosition({static_cast<float>(col) * CELL_SIZE, static_cast<float>(row) * CELL_SIZE});
            window.draw(wall);
          }
        }
      }
      // Column separations
      wall.setRotation(90);
      for (auto row = 0u; row < maze.getHeight(); ++row)
      {
        for (auto col = 1u; col < maze.getWidth(); ++col)
        {
          if (maze.getSeparation({row, col}, daedalus::Direction::West) == daedalus::Separation::Wall)
          {
            wall.setPosition({static_cast<float>(col) * CELL_SIZE, static_cast<float>(row) * CELL_SIZE});
            window.draw(wall);
          }
        }
      }

      window.display();
    }
  }

  return 0;
}
