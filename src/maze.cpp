#include "maze.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

#include <cassert>

namespace daedalus
{

Maze::Maze(std::size_t p_width, std::size_t p_height)
  : m_eastSeparations{p_height, {p_width - 1, Separation::Wall}}
  , m_southSeparations{p_height - 1, {p_width, Separation::Wall}}
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

void Maze::draw(sf::RenderWindow& p_window) const
{
  sf::RectangleShape wall(sf::Vector2f(Cell::PIXELS, 4));
  wall.setFillColor(sf::Color::Black);

  // Horizontal
  wall.setRotation(0);
  wall.setOrigin(0, wall.getSize().y / 2);
  // Borders
  for (auto col = 0u; col < getWidth(); ++col)
  {
    wall.setPosition(col * Cell::PIXELS, 0);
    p_window.draw(wall);
    wall.setPosition(col * Cell::PIXELS, getHeight() * Cell::PIXELS);
    p_window.draw(wall);
  }
  // Separations
  for (auto row = 1u; row < getHeight(); ++row)
  {
    for (auto col = 0u; col < getWidth(); ++col)
    {
      if (getSeparation({row, col}, daedalus::Direction::North) != daedalus::Separation::Empty)
      {
        wall.setPosition(col * Cell::PIXELS, row * Cell::PIXELS);
        p_window.draw(wall);
      }
    }
  }

  // Vertical
  wall.setRotation(90);
  // Borders
  for (auto row = 0u; row < getHeight(); ++row)
  {
    wall.setPosition(0, row * Cell::PIXELS);
    p_window.draw(wall);
    wall.setPosition(getWidth() * Cell::PIXELS, row * Cell::PIXELS);
    p_window.draw(wall);
  }
  // separations
  for (auto row = 0u; row < getHeight(); ++row)
  {
    for (auto col = 1u; col < getWidth(); ++col)
    {
      if (getSeparation({row, col}, daedalus::Direction::West) != daedalus::Separation::Empty)
      {
        wall.setPosition(col * Cell::PIXELS, row * Cell::PIXELS);
        p_window.draw(wall);
      }
    }
  }
}

}
