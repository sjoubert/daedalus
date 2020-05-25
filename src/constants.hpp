#ifndef DAEDALUS_CONSTANTS_HPP
#define DAEDALUS_CONSTANTS_HPP

namespace daedalus
{

constexpr int CELL_SIZE = 32;

enum class Separation
{
  Empty,
  Wall,
};

enum class Direction
{
  North,
  East,
  South,
  West,
};

}

#endif
