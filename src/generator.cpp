#include "generator.hpp"

#include <algorithm>

namespace daedalus
{

Generator::Generator(std::size_t p_width, std::size_t p_height)
  : m_width(p_width)
  , m_height(p_height)
  , m_rng(std::random_device{}())
{
}

Maze Generator::primMaze()
{
  Maze maze{m_width, m_height};

  std::vector cells = {Cell{0, 0}};

  std::vector<std::pair<Cell, Cell>> walls;
  auto pushBackWalls = [&](Cell const& p_cell)
  {
    for (auto dir: {Direction::North, Direction::South, Direction::East, Direction::West})
    {
      if (maze.getSeparation(p_cell, dir) == Separation::Wall)
      {
        walls.emplace_back(p_cell, maze.getNextCell(p_cell, dir));
      }
    }
  };
  pushBackWalls(cells.back());

  while (not walls.empty())
  {
    auto wallIt = std::next(walls.begin(), std::uniform_int_distribution<>(0, walls.size()-1)(m_rng));
    auto wall = *wallIt;
    walls.erase(wallIt);

    if (std::find(cells.begin(), cells.end(), wall.second) == cells.end())
    {
      maze.setSeparation(wall.first, wall.second, Separation::Empty);
      cells.push_back(wall.second);
      pushBackWalls(wall.second);
    }
  }

  return maze;
}

}