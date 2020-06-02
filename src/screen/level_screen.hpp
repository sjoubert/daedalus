#ifndef DAEDALUS_LEVEL_SCREEN_HPP
#define DAEDALUS_LEVEL_SCREEN_HPP

#include "screen.hpp"
#include "maze.hpp"
#include "run_state.hpp"

namespace daedalus
{

class LevelScreen
  : public Screen
{
public:
  LevelScreen(sf::RenderWindow& p_window, RunState p_state = {});

  std::unique_ptr<Screen> run() override;

private:
  void drawHUD(float p_timeRatio);

  RunState m_state;
  Maze m_maze;
};

}

#endif
