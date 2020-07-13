#include "item.hpp"

namespace daedalus
{

Item::Item(Id p_id, int p_uses, std::string p_name, std::string p_text)
  : m_id(p_id)
  , m_name(p_name)
  , m_text(p_text)
  , m_uses(p_uses)
{
}

Item::Id Item::getId() const
{
  return m_id;
}

std::string const& Item::getName() const
{
  return m_name;
}

std::string const& Item::getText() const
{
  return m_text;
}

bool& Item::getSelected()
{
  return m_selected;
}

bool Item::isSelected() const
{
  return m_selected;
}

int Item::use()
{
  return --m_uses;
}

}
