#ifndef DAEDALUS_MAZE_HPP
#define DAEDALUS_MAZE_HPP

#include "cell.hpp"
#include "run_state.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <vector>
#include <unordered_map>

namespace daedalus
{

class Maze
  : public sf::Drawable
  , public sf::Transformable
{
public:
  enum class Tile
  {
    Floor,
    Border,
    Wall,
    Start,
    ClosedEnd,
    OpenEnd,
    LeverOff,
    LeverOn,
    Bonus,
  };

  enum class Direction
  {
    North,
    East,
    South,
    West,
  };

  Maze(std::size_t p_width, std::size_t p_height, RunState p_state);

  std::size_t getWidth() const;
  std::size_t getHeight() const;

  Cell getNextCell(Cell p_cell, Direction p_direction) const;

  Tile getTile(Cell p_cell) const;
  void setTile(Cell p_cell, Tile p_tile);

  void setPlayer(Cell p_cell);
  void movePlayer(Direction p_direction);

  bool hasWon() const;
  bool hasBonus() const;

  void clearFog();
  void clearFog(Cell p_cell);

  void draw(sf::RenderTarget& p_target, sf::RenderStates p_states) const override;
  sf::Vector2f getVisibleCenter() const;
  sf::Sprite getDoorIndicator() const;
  sf::Sprite getBonusIndicator() const;

private:
  void openDoor();

  std::vector<std::vector<Tile>> m_tiles;
  std::vector<std::vector<bool>> m_fog;

  Cell m_player{};
  Direction m_playerDirection = Direction::South;

  bool m_doorOpen = false;
  bool m_hasBonus = false;

  RunState m_state;

  sf::Texture m_tileset;
  sf::Sprite m_floorSprite;
  sf::Sprite m_borderSprite;
  sf::Sprite m_wallSprite;
  sf::Sprite m_startSprite;
  sf::Sprite m_openEndSprite;
  sf::Sprite m_closedEndSprite;
  sf::Sprite m_bonusSprite;
  sf::Sprite m_leverOffSprite;
  sf::Sprite m_leverOnSprite;

  sf::Texture m_characters;
  std::unordered_map<Direction, sf::Sprite> m_playerSprite;

  sf::SoundBuffer m_glassBreak;
  sf::Sound m_leverSound;
  sf::SoundBuffer m_hiccup;
  sf::Sound m_bonusSound;
};

}

#endif
