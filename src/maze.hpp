#ifndef DAEDALUS_MAZE_HPP
#define DAEDALUS_MAZE_HPP

#include "constants.hpp"
#include "cell.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <vector>

namespace daedalus
{

class Maze
  : public sf::Drawable
{
public:
  Maze(std::size_t p_width, std::size_t p_height);

  std::size_t getWidth() const;
  std::size_t getHeight() const;

  Separation getSeparation(Cell p_cell, Direction p_direction) const;
  Cell getNextCell(Cell p_cell, Direction p_direction) const;
  void setSeparation(Cell p_first, Cell p_second, Separation p_separation);

  Tile getTile(Cell p_cell) const;
  void setTile(Cell p_cell, Tile p_tile);

  void setPlayer(Cell p_cell);
  void movePlayer(Direction p_direction);

  bool hasWon() const;

  void draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const override;

private:
  Separation const* getSeparationData(Cell p_cell, Direction p_direction) const;

  using SeparationGrid = std::vector<std::vector<Separation>>;
  SeparationGrid m_eastSeparations;
  SeparationGrid m_southSeparations;

  std::vector<std::vector<Tile>> m_tiles;

  Cell m_player;
};

}

#endif
