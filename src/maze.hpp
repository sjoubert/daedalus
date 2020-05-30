#ifndef DAEDALUS_MAZE_HPP
#define DAEDALUS_MAZE_HPP

#include "constants.hpp"
#include "cell.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <vector>

namespace daedalus
{

class Maze
  : public sf::Drawable
  , public sf::Transformable
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
  bool doorIsOpen() const;
  bool hasBonus() const;

  void clearFog();

  void draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const override;
  sf::Vector2f getVisibleCenter() const;

private:
  Separation const* getSeparationData(Cell p_cell, Direction p_direction) const;
  void openDoor();

  using SeparationGrid = std::vector<std::vector<Separation>>;
  SeparationGrid m_eastSeparations;
  SeparationGrid m_southSeparations;

  std::vector<std::vector<Tile>> m_tiles;
  std::vector<std::vector<bool>> m_fog;

  Cell m_player{};
  bool m_doorOpen = false;
  bool m_hasBonus = false;
};

}

#endif
