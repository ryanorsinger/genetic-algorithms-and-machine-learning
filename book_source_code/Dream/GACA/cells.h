/***
 * Excerpted from "Genetic Algorithms and Machine Learning for Programmers",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material,
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose.
 * Visit http://www.pragmaticprogrammer.com/titles/fbmach for more book information.
***/
#ifndef CELLS_INCLUDED
#define CELLS_INCLUDED

#include <array>
#include <random>

namespace GACA
{
  typedef std::array<bool, 32> Row;

  class RowGenerator
  {
  public:
    RowGenerator(std::random_device::result_type seed) :
      gen(seed),
      uniform_dist(0, 1)
    {
    }
    Row generate();
  private:
    std::default_random_engine gen;
    std::uniform_int_distribution<size_t> uniform_dist;
  };
}

#endif