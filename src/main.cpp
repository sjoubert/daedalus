#include "cell.hpp"
#include "maze.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

int main()
{
  daedalus::Maze maze(16, 9);

  sf::VideoMode const videoMode(maze.getWidth() * daedalus::Cell::PIXELS, maze.getHeight() * daedalus::Cell::PIXELS);
  sf::RenderWindow window(videoMode, "Daedalus");

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
