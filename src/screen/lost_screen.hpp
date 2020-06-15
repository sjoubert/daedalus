#ifndef DAEDALUS_LOST_SCREEN_HPP
#define DAEDALUS_LOST_SCREEN_HPP

#include "screen.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace daedalus
{

class LostScreen
  : public Screen
{
public:
  LostScreen(sf::RenderWindow& p_window, sf::Texture p_background, int p_lostLevel);

  std::unique_ptr<Screen> run() override;

private:
  sf::Texture m_background;
  sf::Sprite m_backgroundSprite;
  int m_lostLevel;
};

}

#endif
