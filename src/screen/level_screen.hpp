#ifndef DAEDALUS_LEVEL_SCREEN_HPP
#define DAEDALUS_LEVEL_SCREEN_HPP

#include "screen.hpp"
#include "maze.hpp"

namespace daedalus
{

class LevelScreen
  : public Screen
{
public:
  LevelScreen(sf::RenderWindow& p_window, float p_allowedTime, std::pair<std::size_t, std::size_t> p_size);

  std::unique_ptr<Screen> run() override;

private:
  void drawHUD(float p_timeRatio);

  Maze m_maze;
  float m_allowedTime;
  std::pair<std::size_t, std::size_t> const m_size;
};

}

#endif
