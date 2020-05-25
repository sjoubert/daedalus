#ifndef DAEDALUS_MAZE_HPP
#define DAEDALUS_MAZE_HPP

#include <vector>

namespace daedalus
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
