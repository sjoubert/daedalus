#ifndef DAEDALUS_STARTSCREEN_HPP
#define DAEDALUS_STARTSCREEN_HPP

#include "screen.hpp"

namespace daedalus
{

class StartScreen
  : public Screen
{
public:
  using Screen::Screen;

  std::unique_ptr<Screen> run() override;
};

}

#endif
