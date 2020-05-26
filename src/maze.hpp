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

  void draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const;

private:
  Separation const* getSeparationData(Cell p_cell, Direction p_direction) const;

  using SeparationGrid = std::vector<std::vector<Separation>>;
  SeparationGrid m_eastSeparations;
  SeparationGrid m_southSeparations;
};

}

#endif
