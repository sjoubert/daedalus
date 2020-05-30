#ifndef DAEDALUS_NEXT_LEVEL_SCREEN_HPP
#define DAEDALUS_NEXT_LEVEL_SCREEN_HPP

#include <SFML/Graphics/RenderWindow.hpp>

namespace daedalus
{

class NextLevelScreen
{
public:
  NextLevelScreen(sf::RenderWindow& p_window);
  ~NextLevelScreen();

  void run();

private:
  sf::RenderWindow& m_window;
  sf::View m_savedView;
};

}

#endif
