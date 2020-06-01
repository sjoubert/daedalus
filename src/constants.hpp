#ifndef DAEDALUS_CONSTANTS_HPP
#define DAEDALUS_CONSTANTS_HPP

namespace daedalus
{

enum class Tile
{
  Floor,
  Border,
  Wall,
  Start,
  ClosedEnd,
  OpenEnd,
  Lever,
  Bonus,
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
