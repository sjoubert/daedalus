#ifndef DAEDALUS_SCREEN_HPP
#define DAEDALUS_SCREEN_HPP

#include <SFML/Graphics/RenderWindow.hpp>

namespace daedalus
{

class Screen
{
public:
  Screen(sf::RenderWindow& p_window);
  ~Screen();

  virtual void run() = 0;

protected:
  sf::RenderWindow& getWindow();

private:
  sf::RenderWindow& m_window;
  sf::View m_savedView;
};

}

#endif
