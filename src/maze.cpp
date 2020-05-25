#include "maze.hpp"

namespace deadalus
{

Maze::Maze(unsigned int p_width, unsigned int p_height)
  : m_horizontalSeparations{p_width, Separation::Empty}
  , m_verticalSeparations{p_height, Separation::Wall}
{
}

}
