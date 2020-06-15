#ifndef DAEDALUS_NEXT_LEVEL_SCREEN_HPP
#define DAEDALUS_NEXT_LEVEL_SCREEN_HPP

#include "screen.hpp"
#include "run_state.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace daedalus
{

class NextLevelScreen
  : public Screen
{
public:
  NextLevelScreen(sf::RenderWindow& p_window, sf::Texture p_background, RunState p_state);

  std::unique_ptr<Screen> run() override;

private:
  sf::Texture m_background;
  sf::Sprite m_backgroundSprite;
  RunState m_state;
};

}

#endif
