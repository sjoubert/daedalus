#ifndef DAEDALUS_GENERATOR_HPP
#define DAEDALUS_GENERATOR_HPP

#include "maze.hpp"

#include <random>
#include <cstddef>

namespace daedalus
{

class Generator
{
public:
  Generator(std::size_t p_width, std::size_t p_height);

  Maze primMaze();

private:
  Cell randomCell();
  Cell randomEmptyCell();

  std::size_t m_width;
  std::size_t m_height;

  std::mt19937 m_rng;
  std::uniform_int_distribution<std::size_t> m_rowDist;
  std::uniform_int_distribution<std::size_t> m_colDist;
};

}

#endif
