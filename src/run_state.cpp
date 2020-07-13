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
  auto const sizeDelta = countItem(Item::Id::Unzip) - countItem(Item::Id::Zip);
  std::uniform_int_distribution<std::size_t> sizeDist{10 + sizeDelta, 16 + sizeDelta};
  return sizeDist(m_sizeRng);
}

std::size_t RunState::newHeight()
{
  auto const sizeDelta = countItem(Item::Id::Unzip) - countItem(Item::Id::Zip);
  std::uniform_int_distribution<std::size_t> sizeDist{5 + sizeDelta, 9 + sizeDelta};
  return sizeDist(m_sizeRng);
}

float RunState::getTimeFactor() const
{
  return 0.5 + 0.1 * (countItem(Item::Id::Hourglass) - countItem(Item::Id::Metronome));
}

void RunState::addBonus()
{
  ++m_bonusCount;
}

std::size_t RunState::getBonusCount() const
{
  return m_bonusCount;
}

std::vector<Item> RunState::getBonus()
{
  auto count = m_bonusCount;
  if (hasItem(Item::Id::Panacea))
  {
    ++count;
  }
  return getItems(m_bonus, count);
}

std::vector<Item> RunState::getMalus()
{
  auto count = 1;
  if (hasItem(Item::Id::Panacea))
  {
    ++count;
  }
  return getItems(m_malus, count);
}

std::vector<Item> RunState::getItems(std::vector<Item>& p_itemPool, std::size_t p_count)
{
  std::ranges::shuffle(p_itemPool, m_itemRng);
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
      addItem(item.getId());
      --m_bonusCount;
    }
  }
  for (auto& item: p_malus)
  {
    if (item.isSelected())
    {
      addItem(item.getId());
    }
  }
}

void RunState::addItem(Item::Id p_id)
{
  m_items.push_back(p_id);
}

bool RunState::hasItem(Item::Id p_id) const
{
  return std::ranges::find(m_items, p_id) != m_items.end();
}

std::size_t RunState::countItem(Item::Id p_id) const
{
  return std::ranges::count(m_items, p_id);
}

}
