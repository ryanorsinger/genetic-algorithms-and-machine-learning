#include "GoL.h"

#include <algorithm>
#include <random>
#include <vector>

namespace GoL
{
void walkNeighbors(size_t x, size_t y, size_t max_x, size_t max_y,
        std::function<void(size_t, size_t)> action)
{
  if(y>0)
  {
    if(x>0) action(x-1,y-1);
    action(x,y-1);
    if(x<max_x-1) action(x+1,y-1);
  }
  if(x>0) action(x-1,y);
  if(x<max_x-1) action(x+1,y);
  if(y<max_y-1) 
  {
    if(x>0) action(x-1,y+1);
    action(x,y+1);
    if(x<max_x-1) action(x+1,y+1);
  }
}

void walkNeighborsWithWrapping(size_t x, size_t y,
        size_t max_x, size_t max_y,
        std::function<void(size_t, size_t)> action)
{
  size_t row = y>0? y-1 : max_y -1;
  action(x>0? x-1 : max_x - 1, row);
  action(                   x, row);
  action(x<max_x-1? x + 1 : 0, row);
  row = y;
  action(x>0? x-1 : max_x - 1, row);
  action(x<max_x-1? x + 1 : 0, row);
  row = y<max_y-1? y+1 : 0;
  action(x>0? x-1 : max_x - 1, row);
  action(                   x, row);
  action(x<max_x-1? x + 1 : 0, row);
}

  void glider(World & world, size_t corner_x, size_t corner_y)
  {
    world.Spark(corner_x,     corner_y);
    world.Spark(corner_x + 1, corner_y);
    world.Spark(corner_x + 2, corner_y);
    world.Spark(corner_x + 2, corner_y + 1);
    world.Spark(corner_x + 1, corner_y + 2);
  }

  void reflected_glider(World & world, size_t corner_x, size_t corner_y)
  {
    world.Spark(corner_x,     corner_y);
    world.Spark(corner_x + 1, corner_y);
    world.Spark(corner_x + 2, corner_y);
    world.Spark(corner_x,     corner_y + 1);
    world.Spark(corner_x + 1, corner_y + 2);
  }

  void inverted_glider(World & world, size_t corner_x, size_t corner_y)
  {
    world.Spark(corner_x,     corner_y + 2);
    world.Spark(corner_x + 1, corner_y + 2);
    world.Spark(corner_x + 2, corner_y + 2);
    world.Spark(corner_x,     corner_y + 1);
    world.Spark(corner_x + 1, corner_y);
  }

  World random_setup(size_t x_start_max,
            size_t y_start_max,
            size_t GoLWorld_x,
            size_t GoLWorld_y,
            size_t number,
            bool wrap)

  {
    if(GoLWorld_x< x_start_max)
      throw std::invalid_argument("GoLWorld width too small");
    if(GoLWorld_y< y_start_max)
      throw std::invalid_argument("GoLWorld height too small");
    return World(GoLWorld_x, GoLWorld_y, wrap, x_start_max, y_start_max, number);
  }

  World::World(size_t max_x, size_t max_y, bool wrap) :
    World(max_x, max_y, wrap, max_x, max_y, 0)
  {
  }

  World::World(size_t max_x, size_t max_y,
               bool wrap,
               size_t start_width, size_t start_height,
               size_t number) :
    max_x(max_x),
    max_y(max_y),
    state(max_x*max_y, false),
    wrap(wrap)
  {
    if (number > start_width*start_height)
      throw std::invalid_argument("Start rectangle too small");
    if (number)
    {
      std::fill_n(state.begin(), number, true);
      std::random_device rd;
      std::mt19937 gen(rd());
      std::shuffle(state.begin(),
                   state.begin() + start_width*start_height,
                   gen);
    }
  }

  size_t World::Alive() const 
  {
    size_t alive = 0;
    for(size_t y=0; y<Height(); ++y)
    {
      for(size_t x=0; x<Width(); ++x)
      {
        if(Alive(x, y))
          ++alive;
      }
    }
    return alive;
  }

  void World::Update()
  {
    std::vector<bool> new_state(max_x*max_y, false);
    for (size_t y = 0; y<max_y; ++y)
    {
      for (size_t x = 0; x<max_x; ++x)
      {
        new_state[y*max_x + x] = StayAlive(x, y);
      }
    }
    state.swap(new_state);
  }

  bool World::StayAlive(size_t x, size_t y) const
  {
    size_t countAlive = 0;
    if (wrap)
      walkNeighborsWithWrapping(x, y, max_x, max_y,
        [&](size_t xi, size_t yi)
        {
          countAlive += Alive(xi, yi);
        }
      );
    else
      walkNeighbors(x, y, max_x, max_y,
        [&](size_t xi, size_t yi)
        {
          countAlive += Alive(xi, yi);
        });
    if (Alive(x, y))
    {
      return countAlive == 2 || countAlive == 3;
    }
    else
      return countAlive == 3;
  }

}
