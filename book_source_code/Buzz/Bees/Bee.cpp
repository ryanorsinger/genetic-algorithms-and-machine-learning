#include "Bee.h"

#include <algorithm>
#include <iostream>
#include <numeric>

using namespace BeeColony;

Hive::Hive(int number_workers,
    int number_inactive,
    int number_scout,
    Coordinate start_pos, Coordinate food, float buzz, int steps)
    : bees(bees), steps(steps), step(0u),
    engine(std::random_device()()),
    uniform_dist(0, number_workers+ number_inactive+ number_scout - 1)
{
  for (int i = 0; i < number_inactive; ++i)
  {
    bees.emplace_back(Role::Inactive, start_pos, food, buzz);
  }
  for(int i=0; i < number_workers; ++i)
  {
    bees.emplace_back(Role::Worker, start_pos, food, buzz);
  }
  for(int i=0; i < number_scout; ++i)
  {
    bees.emplace_back(Role::Scout, start_pos, food, buzz);
  }
}

void Bee::waggle(double jiggle) 
{
  if (get_role() == Role::Inactive)
    return;
  
  position.x += jiggle;
}

void Bee::communicate(Role new_role, Coordinate new_food)
{
  role = new_role;
  if (quality(new_food) > quality(food))
  {
    food = new_food;
  }
}

void Bee::scout(double x_move, double y_move)
{
  Coordinate new_pos{position.x + buzz * x_move, position.y + buzz * y_move};
  double new_quality = quality(new_pos);
  if (new_quality > quality(position))
  {
    food = new_pos;
    position = new_pos;
  }
}

void BeeColony::move(Coordinate & from, const Coordinate to, double step)
{
  if (from.y > to.y)
    from.y -= step;
  if (from.y < to.y)
    from.y += step;
  if (from.x < to.x)
    from.x += step;
  if (from.x > to.x)
    from.x -= step;
}

void Bee::work(double x_move, double y_move)
{
  move(position, food, buzz/2.0);
  double new_quality =
    quality({ position.x + x_move, position.y + y_move });
  if (new_quality >= quality(position))
  {
    position.x += x_move;
    position.y += y_move;
  }
}

void Bee::go_home()
{
  if (!is_home())
  {
    move(position, home, buzz);
  }
}

void Hive::waggle_dance()
{
  for (auto & bee : bees)
  {
    const size_t choice = uniform_dist(engine);
    const auto new_role = bees[choice].get_role();
    const auto new_food = bees[choice].get_food();
    bees[choice].communicate(bee.get_role(), bee.get_food());
    bee.communicate(new_role, new_food);
  }
  step = 0;
}

void Hive::explore()
{
  for (auto & bee : bees)
  {
    switch (bee.get_role())
    {
    case Role::Worker:
      bee.work(normal_dist(engine), normal_dist(engine));
      break;
    case Role::Scout:
      bee.scout(normal_dist(engine), normal_dist(engine));
      break;
    }
  }
}


void Hive::update_bees()
{
  //either moving or waggling then updating
  if (++step < steps) 
  {
    explore();
  }
  else if(!all_home())
  {
    for(auto & bee : bees)
    {
      if (!bee.is_home())
        bee.go_home();
      else
        bee.waggle(normal_dist(engine));
    }
  }
  else 
  {
    waggle_dance();
  }
}

bool Hive::all_home()
{
  return std::all_of(bees.cbegin(), bees.cend(),
      [](const Bee &individual) { return individual.is_home(); }
  );
}

void Hive::swarm()
{
  double best_x = -1.0, best_y = -1.0;
  for(const auto & bee : bees)
  {
    if(quality(bee.get_food()) > best_y)
    {
      best_y = bee.get_food().y;
      best_x = bee.get_food().x;
    }
  }
  for(auto & bee : bees)
  {
    bee.move_home({ best_x, best_y });
  }
  step = steps;
}

bool BeeColony::should_swarm(
    const std::vector<BeeColony::Bee> & bees,
    double target)
{
  return bees.end() == std::find_if(bees.begin(), bees.end(), 
    [target](const Bee & bee) {
      return quality(bee.get_food()) < target;
  });
}
