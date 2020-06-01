#ifndef DAEDALUS_STARTSCREEN_HPP
#define DAEDALUS_STARTSCREEN_HPP

#include "screen.hpp"

namespace daedalus
{

class StartScreen
  : public Screen
{
public:
  StartScreen(sf::RenderWindow& p_window);

  std::unique_ptr<Screen> run() override;
};

}

#endif
