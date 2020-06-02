#include "screen.hpp"
#include <iostream>
namespace daedalus
{

Screen::Screen(sf::RenderWindow& p_window)
  : m_window(p_window)
{
  m_window.setView(m_window.getDefaultView());
}

sf::RenderWindow& Screen::getWindow()
{
  return m_window;
}

}
