#include "maze.hpp"

#include "resources.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <cassert>

namespace daedalus
{

Maze::Maze(std::size_t p_width, std::size_t p_height, RunState p_state)
  : m_tiles{p_height, {p_width, Tile::Wall}}
  , m_fog{p_height, std::vector<bool>(p_width, true)}
  , m_state(p_state)
{
  m_tiles.front() = {p_width, Tile::Border};
  for (auto& row: m_tiles)
  {
    row.front() = Tile::Border;
    row.back() = Tile::Border;
  }
  m_tiles.back() = {p_width, Tile::Border};

  sf::Image tilesetsImage;
  tilesetsImage.loadFromFile(getResource("image/rpg-maker-2k-tilesets.png"));
  tilesetsImage.createMaskFromColor({255, 103, 139});
  m_tileset.loadFromImage(tilesetsImage, {480 + 4, 256 + 6, 480, 256});
  auto getTile = [&](int x, int y)
  {
    constexpr int TILE_SIZE = 16;
    constexpr auto scaleFactor = Cell::PIXELS / static_cast<float>(TILE_SIZE);
    sf::Sprite sprite(m_tileset);
    sprite.setTextureRect({TILE_SIZE * x, TILE_SIZE * y, TILE_SIZE, TILE_SIZE});
    sprite.setScale({scaleFactor, scaleFactor});
    return sprite;
  };
  m_floorSprite = getTile(7, 4);
  m_borderSprite = getTile(6, 8);
  m_wallSprite = getTile(19, 9);
  m_startSprite = getTile(0, 12);
  m_openEndSprite = getTile(21, 7);
  m_closedEndSprite = getTile(21, 6);
  m_bonusSprite = getTile(18, 11);
  m_leverOnSprite = getTile(29, 2);
  m_leverOffSprite = getTile(22, 11);

  sf::Image charactersImage;
  charactersImage.loadFromFile(getResource("image/rpg-maker-2k-characters.png"));
  charactersImage.createMaskFromColor({32, 156, 0});
  m_characters.loadFromImage(charactersImage, {2, 2, 192, 128});
  auto getCharacter  = [&](int x, int y)
  {
    constexpr int TILE_SIZE_X = 24;
    constexpr int TILE_SIZE_Y = 32;
    constexpr auto scaleFactor = Cell::PIXELS / static_cast<float>(TILE_SIZE_X);
    sf::Sprite sprite(m_characters);
    sprite.setTextureRect({TILE_SIZE_X * x, TILE_SIZE_Y * y, TILE_SIZE_X, TILE_SIZE_Y});
    sprite.setOrigin(0, TILE_SIZE_Y / 2);
    sprite.setScale({scaleFactor, scaleFactor});
    return sprite;
  };
  m_playerSprite[Direction::North] = getCharacter(1, 0);
  m_playerSprite[Direction::East] = getCharacter(1, 1);
  m_playerSprite[Direction::South] = getCharacter(1, 2);
  m_playerSprite[Direction::West] = getCharacter(1, 3);

  m_glassBreak.loadFromFile(getResource("audio/glass-breaking.wav"));
  m_leverSound.setBuffer(m_glassBreak);
  m_hiccup.loadFromFile(getResource("audio/level-up-3note2.wav"));
  m_bonusSound.setBuffer(m_hiccup);
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
    case Tile::LeverOff:
    {
      openDoor();
      m_leverSound.play();
      setTile(m_player, Tile::LeverOn);
      break;
    }
    case Tile::Bonus:
    {
      m_hasBonus = true;
      m_bonusSound.play();
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

  if (m_state.hasItem(Item::Id::Flashlight))
  {
    auto location = m_player;
    auto const deltaRow = (m_playerDirection == Direction::East || m_playerDirection == Direction::West) ? 1 : 0;
    auto const deltaCol = (m_playerDirection == Direction::North || m_playerDirection == Direction::South) ? 1 : 0;
    for (bool foundObstacle = false; not foundObstacle;)
    {
      m_fog[location.row][location.column] = false;
      m_fog[location.row - deltaRow][location.column - deltaCol] = false;
      m_fog[location.row + deltaRow][location.column + deltaCol] = false;

      auto tile = getTile(location);
      foundObstacle = tile == Tile::Wall || tile == Tile::Border;

      location = getNextCell(location, m_playerDirection);
    }
  }
}

void Maze::movePlayer(Direction p_direction)
{
  m_playerDirection = p_direction;

  auto targetTile = getTile(getNextCell(m_player, p_direction));
  if (targetTile != Tile::Border && targetTile != Tile::Wall)
  {
    m_player = getNextCell(m_player, m_playerDirection);
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

void Maze::clearFog(Cell p_cell)
{
  m_fog[p_cell.row][p_cell.column] = false;
}

void Maze::draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const
{
  p_states.transform *= getTransform();

  sf::Color const fogColor{25, 25, 25};
  p_target.clear(fogColor);

  // Tiles
  for (auto row = 0u; row < getHeight(); ++row)
  {
    for (auto col = 0u; col < getWidth(); ++col)
    {
      std::vector<sf::Sprite> sprites;
      switch (getTile({row, col}))
      {
        case Tile::Floor:
        {
          sprites = {m_floorSprite};
          break;
        }
        case Tile::Border:
        {
          sprites = {m_borderSprite};
          break;
        }
        case Tile::Wall:
        {
          sprites = {m_floorSprite, m_wallSprite};
          break;
        }
        case Tile::Start:
        {
          sprites = {m_startSprite};
          break;
        }
        case Tile::ClosedEnd:
        {
          sprites = {m_closedEndSprite};
          break;
        }
        case Tile::OpenEnd:
        {
          sprites = {m_openEndSprite};
          break;
        }
        case Tile::LeverOff:
        {
          sprites = {m_floorSprite, m_leverOffSprite};
          break;
        }
        case Tile::LeverOn:
        {
          sprites = {m_floorSprite, m_leverOnSprite};
          break;
        }
        case Tile::Bonus:
        {
          sprites = {m_floorSprite, m_bonusSprite};
          break;
        }
      }

      for (auto& sprite: sprites)
      {
        sprite.setPosition(col * Cell::PIXELS, row * Cell::PIXELS);
        p_target.draw(sprite, p_states);
      }
    }
  }

  // Player
  auto player = m_playerSprite.at(m_playerDirection);
  player.setPosition(m_player.column * Cell::PIXELS, m_player.row * Cell::PIXELS * 1.f);
  p_target.draw(player, p_states);

  // Fog of war
  sf::RectangleShape tile({Cell::PIXELS, Cell::PIXELS});
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

sf::Sprite Maze::getDoorIndicator() const
{
  return m_doorOpen ? m_openEndSprite : m_closedEndSprite;
}

sf::Sprite Maze::getBonusIndicator() const
{
  return m_hasBonus ? m_bonusSprite : sf::Sprite{};
}

}
