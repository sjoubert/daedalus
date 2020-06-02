#include "maze.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <cassert>

namespace daedalus
{

Maze::Maze(std::size_t p_width, std::size_t p_height)
  : m_tiles{p_height, {p_width, Tile::Wall}}
  , m_fog{p_height, std::vector<bool>(p_width, true)}
{
  m_tiles.front() = {p_width, Tile::Border};
  for (auto& row: m_tiles)
  {
    row.front() = Tile::Border;
    row.back() = Tile::Border;
  }
  m_tiles.back() = {p_width, Tile::Border};
}

std::size_t Maze::getWidth() const
{
  return m_tiles.front().size();
}

std::size_t Maze::getHeight() const
{
  return m_tiles.size();
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

Maze::Tile Maze::getTile(Cell p_cell) const
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
    case Tile::Lever:
    {
      openDoor();
      setTile(m_player, Tile::Floor);
      break;
    }
    case Tile::Bonus:
    {
      m_hasBonus = true;
      setTile(m_player, Tile::Floor);
      break;
    }
    default:
      break;
  }

  for (auto deltaRow: {-1, 0, 1})
  {
    for (auto deltaCol: {-1, 0, 1})
    {
      m_fog[m_player.row + deltaRow][m_player.column + deltaCol] = false;
    }
  }
}

void Maze::movePlayer(Direction p_direction)
{
  auto targetTile = getTile(getNextCell(m_player, p_direction));
  if (targetTile == Tile::Border || targetTile == Tile::Wall)
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

bool Maze::hasBonus() const
{
  return m_hasBonus;
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

  // Tiles
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
        case Tile::Border:
        {
          tile.setFillColor(sf::Color::Black);
          break;
        }
        case Tile::Wall:
        {
          tile.setFillColor({160, 100, 40});
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
        case Tile::Lever:
        {
          tile.setFillColor(sf::Color::Blue);
          break;
        }
        case Tile::Bonus:
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
  auto const playerRadius = Cell::PIXELS / 2 * 0.7;
  sf::CircleShape player(playerRadius);
  player.setOrigin(playerRadius, playerRadius);
  player.setFillColor(sf::Color::Cyan);
  player.setPosition((m_player.column + 0.5) * Cell::PIXELS, (m_player.row + 0.5) * Cell::PIXELS);
  p_target.draw(player, p_states);

  // Fog of war
  tile.setFillColor(fogColor);
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

sf::Vector2f Maze::getVisibleCenter() const
{
  std::pair<std::size_t, std::size_t> rowBounds{getWidth(), 0};
  std::pair<std::size_t, std::size_t> colBounds{getHeight(), 0};
  for (std::size_t row = 0; row < getHeight(); ++row)
  {
    for (std::size_t col = 0; col < getWidth(); ++col)
    {
      if (not m_fog[row][col])
      {
        rowBounds.first = std::min(rowBounds.first, row);
        rowBounds.second = std::max(rowBounds.second, row);
        colBounds.first = std::min(colBounds.first, col);
        colBounds.second = std::max(colBounds.second, col);
      }
    }
  }

  sf::Vector2f center(colBounds.first, rowBounds.first);
  center += {(colBounds.second - colBounds.first) / 2.f, (rowBounds.second - rowBounds.first) / 2.f};
  center += {0.5f, 0.5f};
  center *= static_cast<float>(Cell::PIXELS);
  return getTransform().transformPoint(center);
}

}
