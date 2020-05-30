#include "screen.hpp"

namespace daedalus
{

Screen::Screen(sf::RenderWindow& p_window)
  : m_window(p_window)
  , m_savedView(m_window.getView())
{
  m_window.setView(m_window.getDefaultView());
}

Screen::~Screen()
{
  m_window.setView(m_savedView);
}

sf::RenderWindow& Screen::getWindow()
{
  return m_window;
}

}
