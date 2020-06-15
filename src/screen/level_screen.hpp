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
  static void centerView(sf::RenderTarget& p_target, sf::Vector2f p_center);
  void drawHUD(float p_timeRatio);
  sf::Texture createBackground();

  RunState m_state;
  Maze m_maze;
};

}

#endif
