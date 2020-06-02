#ifndef DAEDALUS_SCREEN_HPP
#define DAEDALUS_SCREEN_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

namespace daedalus
{

class Screen
{
public:
  Screen(sf::RenderWindow& p_window);
  virtual ~Screen() = default;

  virtual std::unique_ptr<Screen> run() = 0;

protected:
  sf::RenderWindow& getWindow();

private:
  sf::RenderWindow& m_window;
};

}

#endif
