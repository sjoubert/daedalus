#ifndef DAEDALUS_NEXT_LEVEL_SCREEN_HPP
#define DAEDALUS_NEXT_LEVEL_SCREEN_HPP

#include "screen.hpp"
#include "run_state.hpp"

namespace daedalus
{

class NextLevelScreen
  : public Screen
{
public:
  NextLevelScreen(sf::RenderWindow& p_window, RunState p_state);

  std::unique_ptr<Screen> run() override;

private:
  RunState m_state;
};

}

#endif
