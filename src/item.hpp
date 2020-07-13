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
  enum class Id
  {
    Compass,
    DowserWand,
    Flashlight,
    Hourglass,
    Metronome,
    Mirror,
    Panacea,
    TreasureMap,
    Unzip,
    UpsideDown,
    Zip,
  };

  Item(Id p_id, int p_uses, std::string p_name, std::string p_text);
  Item(Item const&) = default;

  Id getId() const;
  std::string const& getName() const;
  std::string const& getText() const;

  bool& getSelected();
  bool isSelected() const;

  /**
   * Mark item as used
   * @return Uses left
   */
  int use();

private:
  Id m_id;
  std::string m_name;
  std::string m_text;
  bool m_selected{false};
  int m_uses;
};

}

#endif
