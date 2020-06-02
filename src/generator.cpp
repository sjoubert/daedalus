#include "generator.hpp"

#include <algorithm>

namespace daedalus
{

Generator::Generator(std::size_t p_width, std::size_t p_height)
  : m_width((p_width % 2 == 0) ? p_width - 1 : p_width)
  , m_height((p_height % 2 == 0) ? p_height - 1 : p_height)
  , m_rng(std::random_device{}())
  , m_rowDist(1, m_height - 2)
  , m_colDist(1, p_width - 2)
{
}

Maze Generator::primMaze()
{
  Maze maze{m_width, m_height};

  // Walls
  std::vector cells = {Cell{1, 1}};
  maze.setTile({1, 1}, Maze::Tile::Floor);

  std::vector<std::pair<Cell, Cell>> walls;
  auto pushBackWalls = [&](Cell const& p_cell)
  {
    for (auto dir: {Maze::Direction::North, Maze::Direction::South, Maze::Direction::East, Maze::Direction::West})
    {
      auto wallCell = maze.getNextCell(p_cell, dir);
      auto behindCell = maze.getNextCell(wallCell, dir);
      if (maze.getTile(wallCell) == Maze::Tile::Wall && maze.getTile(behindCell) != Maze::Tile::Border)
      {
        walls.emplace_back(wallCell, maze.getNextCell(wallCell, dir));
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
      maze.setTile(wall.first, Maze::Tile::Floor);
      maze.setTile(wall.second, Maze::Tile::Floor);
      cells.push_back(wall.first);
      cells.push_back(wall.second);
      pushBackWalls(wall.second);
    }
  }

  // Cells
  auto startCell = randomEmptyCell(maze);
  maze.setTile(startCell, Maze::Tile::Start);
  maze.setPlayer(startCell);
  maze.setTile(randomEmptyCell(maze), Maze::Tile::ClosedEnd);
  maze.setTile(randomEmptyCell(maze), Maze::Tile::Lever);
  maze.setTile(randomEmptyCell(maze), Maze::Tile::Bonus);

  return maze;
}

Cell Generator::randomCell()
{
  return {m_rowDist(m_rng), m_colDist(m_rng)};
}

Cell Generator::randomEmptyCell(Maze const& p_maze)
{
  auto cell = randomCell();
  while (p_maze.getTile(cell) != Maze::Tile::Floor)
  {
    cell = randomCell();
  }
  return cell;
}

}