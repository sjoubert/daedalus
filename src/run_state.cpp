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

void RunState::addBonus()
{
  ++m_bonusCount;
}

std::vector<Item> RunState::getBonus()
{
  return getItems(m_bonus, m_bonusCount);
}

std::vector<Item> RunState::getMalus()
{
  return getItems(m_malus, 1);
}

std::vector<Item> RunState::getItems(std::vector<Item>& p_itemPool, std::size_t p_count)
{
  std::shuffle(p_itemPool.begin(), p_itemPool.end(), m_itemRng);
  auto count = std::min(p_count, p_itemPool.size());

  // Remove used items
  auto itBound = std::prev(p_itemPool.end(), count);
  std::vector<Item> items(itBound, p_itemPool.end());
  p_itemPool.erase(itBound, p_itemPool.end());

  // Replace items with left uses
  for (auto& item: items)
  {
    if (item.use() != 0)
    {
      p_itemPool.push_back(item);
    }
  }

  return items;
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

void RunState::addItem(Item::Id p_id)
{
  m_items.push_back(p_id);
}

bool RunState::hasItem(Item::Id p_id) const
{
  return std::find(m_items.begin(), m_items.end(), p_id) != m_items.end();
}

}
