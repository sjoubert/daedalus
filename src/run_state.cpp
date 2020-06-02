#include "run_state.hpp"

namespace daedalus
{

std::size_t RunState::newWidth()
{
  return m_sizeDist(m_rng);
}

std::size_t RunState::newHeight()
{
  return m_sizeDist(m_rng);
}

void RunState::increaseSize()
{
  m_sizeDist = std::uniform_int_distribution<std::size_t>{m_sizeDist.min() + 1, m_sizeDist.max() + 1};
}

float RunState::getTimeFactor() const
{
  return m_timeFactor;
}

void RunState::increaseTimeFactor()
{
  m_timeFactor += 0.1;
}

bool RunState::hasBonus() const
{
  return m_bonus;
}

void RunState::setBonus(bool p_bonus)
{
  m_bonus = p_bonus;
}

int RunState::currentLevel() const
{
  return m_level;
}

void RunState::nextLevel()
{
  ++m_level;
}

}
