#ifndef DAEDALUS_MAZE_HPP
#define DAEDALUS_MAZE_HPP

#include "cell.hpp"

#include <vector>

namespace daedalus
{

enum class Separation
{
  Empty,
  Wall,
};

enum class Direction
{
  North,
  East,
  South,
  West,
};

class Maze
{
public:
  Maze(std::size_t p_width, std::size_t p_height);

  std::size_t getWidth() const;
  std::size_t getHeight() const;

  Separation getSeparation(Cell p_cell, Direction p_direction) const;

private:
  std::vector<std::vector<Separation>> m_eastSeparations;
  std::vector<std::vector<Separation>> m_southSeparations;
};

}

#endif
