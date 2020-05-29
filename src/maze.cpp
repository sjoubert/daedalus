#include "maze.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <cassert>

namespace daedalus
{

Maze::Maze(std::size_t p_width, std::size_t p_height)
  : m_eastSeparations{p_height, {p_width - 1, Separation::Wall}}
  , m_southSeparations{p_height - 1, {p_width, Separation::Wall}}
  , m_tiles{p_height, {p_width, Tile::Floor}}
  , m_fog{p_height, std::vector<bool>(p_width, true)}
{
}

std::size_t Maze::getWidth() const
{
  return m_southSeparations.front().size();
}

std::size_t Maze::getHeight() const
{
  return m_eastSeparations.size();
}

Separation Maze::getSeparation(Cell p_cell, Direction p_direction) const
{
  if (auto id = getSeparationData(p_cell, p_direction); id)
  {
    return *id;
  }
  return Separation::Border;
}

Separation const* Maze::getSeparationData(Cell p_cell, Direction p_direction) const
{
  if ((p_direction == Direction::North && p_cell.row == 0)
    || (p_direction == Direction::South && p_cell.row == getHeight() - 1)
    || (p_direction == Direction::West && p_cell.column == 0)
    || (p_direction == Direction::East && p_cell.column == getWidth() - 1))
  {
    return nullptr;
  }

  auto row = p_cell.row;
  if (p_direction == Direction::North)
  {
    --row;
  }

  auto col = p_cell.column;
  if (p_direction == Direction::West)
  {
    --col;
  }

  if (p_direction == Direction::North || p_direction == Direction::South)
  {
    return &m_southSeparations[row][col];
  }
  else
  {
    return &m_eastSeparations[row][col];
  }
}

Cell Maze::getNextCell(Cell p_cell, Direction p_direction) const
{
  auto row = p_cell.row;
  auto col = p_cell.column;

  switch (p_direction)
  {
    case Direction::North:
    {
      --row;
      break;
    }
    case Direction::South:
    {
      ++row;
      break;
    }
    case Direction::East:
    {
      ++col;
      break;
    }
    case Direction::West:
    {
      --col;
      break;
    }
  }

  return {row, col};
}

void Maze::setSeparation(Cell p_first, Cell p_second, Separation p_separation)
{
  assert(p_first.row == p_second.row || p_first.column == p_second.column);
  assert(p_first.row < getHeight() && p_first.column < getWidth()
    && p_second.row < getHeight() && p_second.column < getWidth());

  if (p_first.row == p_second.row)
  {
    if (p_first.column - 1 == p_second.column)
    {
      m_eastSeparations[p_first.row][p_second.column] = p_separation;
    }
    else if (p_first.column + 1 == p_second.column)
    {
      m_eastSeparations[p_first.row][p_first.column] = p_separation;
    }
  }
  else if (p_first.column == p_second.column)
  {
    if (p_first.row - 1 == p_second.row)
    {
      m_southSeparations[p_second.row][p_first.column] = p_separation;
    }
    else if (p_first.row + 1 == p_second.row)
    {
      m_southSeparations[p_first.row][p_first.column] = p_separation;
    }
  }
}

Tile Maze::getTile(Cell p_cell) const
{
  return m_tiles[p_cell.row][p_cell.column];
}

void Maze::setTile(Cell p_cell, Tile p_tile)
{
  m_tiles[p_cell.row][p_cell.column] = p_tile;
}

void Maze::setPlayer(Cell p_cell)
{
  m_player = p_cell;

  switch (getTile(m_player))
  {
    case Tile::Key:
    {
      openDoor();
      setTile(m_player, Tile::Floor);
      break;
    }
    default:
      break;
  }

  m_fog[m_player.row][m_player.column] = false;
  for (auto dir: {Direction::North, Direction::South, Direction::East, Direction::West})
  {
    if (getSeparation(m_player, dir) == Separation::Empty)
    {
      auto nextCell = getNextCell(m_player, dir);
      m_fog[nextCell.row][nextCell.column] = false;
    }
  }
}

Cell Maze::getPlayer() const
{
  return m_player;
}

void Maze::movePlayer(Direction p_direction)
{
  if (getSeparation(m_player, p_direction) != daedalus::Separation::Empty)
  {
    return;
  }

  switch (p_direction)
  {
    case Direction::North:
    {
      --m_player.row;
      break;
    }
    case Direction::South:
    {
      ++m_player.row;
      break;
    }
    case Direction::East:
    {
      ++m_player.column;
      break;
    }
    case Direction::West:
    {
      --m_player.column;
      break;
    }
  }
  setPlayer(m_player);
}

bool Maze::hasWon() const
{
  return getTile(m_player) == Tile::OpenEnd;
}

void Maze::openDoor()
{
  m_doorOpen = true;
  for (auto& row: m_tiles)
  {
    for (auto& tile: row)
    {
      if (tile == Tile::ClosedEnd)
      {
        tile = Tile::OpenEnd;
      }
    }
  }
}

bool Maze::doorIsOpen() const
{
  return m_doorOpen;
}

void Maze::clearFog()
{
  for (auto& row: m_fog)
  {
    std::fill(row.begin(), row.end(), false);
  }
}

void Maze::draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const
{
  p_states.transform *= getTransform();

  sf::Color const fogColor{25, 25, 25};
  p_target.clear(fogColor);

  // Ground
  sf::RectangleShape tile({Cell::PIXELS, Cell::PIXELS});
  for (auto row = 0u; row < getHeight(); ++row)
  {
    for (auto col = 0u; col < getWidth(); ++col)
    {
      switch (getTile({row, col}))
      {
        case Tile::Floor:
        {
          tile.setFillColor({100, 100, 100});
          break;
        }
        case Tile::Start:
        {
          tile.setFillColor(sf::Color::White);
          break;
        }
        case Tile::ClosedEnd:
        {
          tile.setFillColor(sf::Color::Red);
          break;
        }
        case Tile::OpenEnd:
        {
          tile.setFillColor(sf::Color::Green);
          break;
        }
        case Tile::Key:
        {
          tile.setFillColor(sf::Color::Yellow);
          break;
        }
      }

      tile.setPosition(col * Cell::PIXELS, row * Cell::PIXELS);
      p_target.draw(tile, p_states);
    }
  }

  // Player
  auto const playerRadius = daedalus::Cell::PIXELS / 2 * 0.7;
  sf::CircleShape player(playerRadius);
  player.setOrigin(playerRadius, playerRadius);
  player.setFillColor(sf::Color::Blue);
  player.setPosition((m_player.column + 0.5) * daedalus::Cell::PIXELS, (m_player.row + 0.5) * daedalus::Cell::PIXELS);
  p_target.draw(player, p_states);

  sf::RectangleShape wall(sf::Vector2f(Cell::PIXELS, 4));
  wall.setFillColor(sf::Color::Black);

  // Horizontal
  wall.setRotation(0);
  wall.setOrigin(0, wall.getSize().y / 2);
  // Borders
  for (auto col = 0u; col < getWidth(); ++col)
  {
    wall.setPosition(col * Cell::PIXELS, 0);
    p_target.draw(wall, p_states);
    wall.setPosition(col * Cell::PIXELS, getHeight() * Cell::PIXELS);
    p_target.draw(wall, p_states);
  }
  // Separations
  for (auto row = 1u; row < getHeight(); ++row)
  {
    for (auto col = 0u; col < getWidth(); ++col)
    {
      if (getSeparation({row, col}, daedalus::Direction::North) != daedalus::Separation::Empty)
      {
        wall.setPosition(col * Cell::PIXELS, row * Cell::PIXELS);
        p_target.draw(wall, p_states);
      }
    }
  }

  // Vertical
  wall.setRotation(90);
  // Borders
  for (auto row = 0u; row < getHeight(); ++row)
  {
    wall.setPosition(0, row * Cell::PIXELS);
    p_target.draw(wall, p_states);
    wall.setPosition(getWidth() * Cell::PIXELS, row * Cell::PIXELS);
    p_target.draw(wall, p_states);
  }
  // separations
  for (auto row = 0u; row < getHeight(); ++row)
  {
    for (auto col = 1u; col < getWidth(); ++col)
    {
      if (getSeparation({row, col}, daedalus::Direction::West) != daedalus::Separation::Empty)
      {
        wall.setPosition(col * Cell::PIXELS, row * Cell::PIXELS);
        p_target.draw(wall, p_states);
      }
    }
  }

  // Fog of war
  tile.setFillColor(fogColor);
  for (auto row = -1; row <= static_cast<decltype(row)>(getHeight()); ++row)
  {
    tile.setPosition(- Cell::PIXELS, row * Cell::PIXELS);
    p_target.draw(tile, p_states);
    tile.setPosition(getWidth() * Cell::PIXELS, row * Cell::PIXELS);
    p_target.draw(tile, p_states);
  }
  for (auto col = -1; col <= static_cast<decltype(col)>(getWidth()); ++col)
  {
    tile.setPosition(col * Cell::PIXELS, - Cell::PIXELS);
    p_target.draw(tile, p_states);
    tile.setPosition(col * Cell::PIXELS, getHeight() * Cell::PIXELS);
    p_target.draw(tile, p_states);
  }
  for (auto row = 0u; row < getHeight(); ++row)
  {
    for (auto col = 0u; col < getWidth(); ++col)
    {
      if (m_fog[row][col])
      {
        tile.setPosition(col * Cell::PIXELS, row * Cell::PIXELS);
        p_target.draw(tile, p_states);
      }
    }
  }
}

}
