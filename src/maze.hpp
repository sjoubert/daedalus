#ifndef DAEDALUS_MAZE_HPP
#define DAEDALUS_MAZE_HPP

#include "constants.hpp"
#include "cell.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <vector>

namespace daedalus
{

class Maze
{
public:
  Maze(std::size_t p_width, std::size_t p_height);

  std::size_t getWidth() const;
  std::size_t getHeight() const;

  void draw(sf::RenderWindow& p_window) const;

private:
  Separation getSeparation(Cell p_cell, Direction p_direction) const;

  std::vector<std::vector<Separation>> m_eastSeparations;
  std::vector<std::vector<Separation>> m_southSeparations;
};

}

#endif
