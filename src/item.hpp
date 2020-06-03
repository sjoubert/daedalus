#ifndef DAEDALUS_ITEM_HPP
#define DAEDALUS_ITEM_HPP

#include <functional>
#include <string>
#include <utility>

namespace daedalus
{
class RunState;

class Item
{
public:
  Item(std::string p_name, std::string p_text);
  Item(Item const&) = default;

  std::string const& getName() const;
  std::string const& getText() const;
  std::function<void(RunState&)>& getEffect();
  std::function<void(RunState&)> const& getEffect() const;

  bool& getSelected();
  bool isSelected() const;

private:
  std::string m_name;
  std::string m_text;
  std::function<void(RunState&)> m_effect;
  bool m_selected{false};
};

}

#endif
