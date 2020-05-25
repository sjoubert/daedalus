#ifndef DEADALUS_MAZE_HPP
#define DEADALUS_MAZE_HPP

#include <vector>

namespace deadalus
{

enum class Separation
{
  Empty,
  Wall,
};

class Maze
{
public:
  Maze(unsigned int p_width, unsigned int p_height);

private:
  std::vector<Separation> m_horizontalSeparations;
  std::vector<Separation> m_verticalSeparations;
};

}

#endif
