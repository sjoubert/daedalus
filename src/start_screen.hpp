#ifndef DAEDALUS_STARTSCREEN_HPP
#define DAEDALUS_STARTSCREEN_HPP

#include <SFML/Graphics/RenderWindow.hpp>

namespace daedalus
{

class StartScreen
{
public:
  StartScreen(sf::RenderWindow& p_window);
  ~StartScreen();

  void run();

private:
  sf::RenderWindow& m_window;
  sf::View m_savedView;
};

}

#endif
