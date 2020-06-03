#include "run_state.hpp"

#include <algorithm>

namespace daedalus
{

RunState::RunState()
{
  initItems();
}

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

std::vector<Item> RunState::getBonus()
{
  std::shuffle(m_bonus.begin(), m_bonus.end(), m_itemRng);
  return {m_bonus.begin(), std::next(m_bonus.begin(), std::min(m_bonusCount, m_bonus.size()))};
}

void RunState::addBonus()
{
  ++m_bonusCount;
}

std::vector<Item> RunState::getMalus()
{
  std::shuffle(m_malus.begin(), m_malus.end(), m_itemRng);
  return {m_malus.front()};
}

int RunState::currentLevel() const
{
  return m_level;
}

void RunState::nextLevel(std::vector<Item> const& p_bonus, std::vector<Item> const& p_malus)
{
  ++m_level;

  for (auto& item: p_bonus)
  {
    if (item.isSelected())
    {
      item.getEffect()(*this);
      --m_bonusCount;
    }
  }
  for (auto& item: p_malus)
  {
    if (item.isSelected())
    {
      item.getEffect()(*this);
    }
  }
}

}
