#ifndef DAEDALUS_LOST_SCREEN_HPP
#define DAEDALUS_LOST_SCREEN_HPP

#include "screen.hpp"

namespace daedalus
{

class LostScreen
  : public Screen
{
public:
  LostScreen(sf::RenderWindow& p_window, int p_lostLevel);

  std::unique_ptr<Screen> run() override;

private:
  int m_lostLevel;
};

}

#endif
