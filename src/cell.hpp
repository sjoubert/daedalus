#ifndef DEADALUS_CELL_HPP
#define DEADALUS_CELL_HPP

#include <cstddef>

namespace daedalus
{

struct Cell
{
  static constexpr int PIXELS = 32;

  std::size_t row;
  std::size_t column;
};

}

#endif
