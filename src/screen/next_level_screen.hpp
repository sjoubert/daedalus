#ifndef DAEDALUS_NEXT_LEVEL_SCREEN_HPP
#define DAEDALUS_NEXT_LEVEL_SCREEN_HPP

#include "screen.hpp"

namespace daedalus
{

class NextLevelScreen
  : public Screen
{
public:
  NextLevelScreen(sf::RenderWindow& p_window);

  void run();
};

}

#endif
