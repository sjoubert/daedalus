#include "item.hpp"

namespace daedalus
{

Item::Item(std::string p_name, std::string p_text)
  : m_name(p_name)
  , m_text(p_text)
{
}

std::string const& Item::getName() const
{
  return m_name;
}

std::string const& Item::getText() const
{
  return m_text;
}

std::function<void(RunState&)>& Item::getEffect()
{
  return m_effect;
}

std::function<void(RunState&)> const& Item::getEffect() const
{
  return m_effect;
}

bool& Item::getSelected()
{
  return m_selected;
}

bool Item::isSelected() const
{
  return m_selected;
}

}
