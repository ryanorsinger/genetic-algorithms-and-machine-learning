/***
 * Excerpted from "Genetic Algorithms and Machine Learning for Programmers",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material,
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose.
 * Visit http://www.pragmaticprogrammer.com/titles/fbmach for more book information.
***/
#ifndef GoL_INCLUDED
#define GoL_INCLUDED

#include <algorithm>
#include <functional>
#include <set>
#include <stdexcept>
#include <vector>

namespace GoL
{
  void walkNeighbors(size_t x, size_t y, size_t max_x, size_t max_y,
          std::function<void(size_t, size_t)> action);
  void walkNeighborsWithWrapping(size_t x, size_t y, size_t max_x, size_t max_y,
          std::function<void(size_t, size_t)> action);

  ///
  //There are many known shapes that become stable, in an unbounded GoLWorld
  //See http://www.conwaylife.com/wiki/Glider for details
  ///
  class World;
  void glider(World & world,
         size_t corner_x,
        size_t corner_y);
  void reflected_glider(World & world,
              size_t corner_x,
              size_t corner_y);
  void inverted_glider(World & world,
              size_t corner_x,
              size_t corner_y);

  World random_setup(size_t x_start_max,
            size_t y_start_max,
            size_t GoLWorld_x,
            size_t GoLWorld_y,
            size_t number,
            bool wrap = false);

  class World
  {
  public:
    World(size_t max_x, size_t max_y, bool wrap);
    World(size_t max_x, size_t max_y, bool wrap,
          size_t start_width, size_t start_height,
          size_t number);

    size_t Width() const { return max_x; }
    size_t Height() const { return max_y; }
    size_t Alive() const;

    bool Alive(size_t x, size_t y) const
    {
      return state[y*max_x + x];
    }

    void Spark(size_t x, size_t y)
    {
      if(Alive(x,y))
        throw std::invalid_argument("Cell already alive");
      state[y*max_x + x] = true;
    }

    void Update();

  private:
    const size_t max_x;
    const size_t max_y;
    std::vector<bool> state;//evil
    const bool wrap;

    bool StayAlive(size_t x, size_t y) const;
  };
}

#endif

