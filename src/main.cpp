#include "constants.hpp"
#include "maze.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

int main()
{
  daedalus::Maze maze(16, 9);

  sf::RenderWindow window(sf::VideoMode(maze.getWidth() * daedalus::CELL_SIZE, maze.getHeight() * daedalus::CELL_SIZE),
    "Daedalus");

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
      window.display();
    }
  }

  return 0;
}
