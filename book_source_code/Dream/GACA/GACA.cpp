#include "GACA.h"

#include <algorithm>
#include <bitset>
#include <iostream>
#include <stdexcept>
#include <sstream>

using namespace GACA;

World::World(const Rule & rule, Row row, size_t height) :
  rule(rule),
  row(row),
  height(height)
{
  Run();
}

void World::Run()
{
  while (history.size() < height)
  {
    history.push_back(row);
    row = rule(row);
  }
}

bool World::Alive(size_t x, size_t y) const
{
  size_t available = history.size();
  if(y < available)
  {
    return history[y][x];
  }
  return row[x];
}

void World::Reset(Row new_starting_row)
{
  row = new_starting_row;
  history.clear();
  Run();
}

Row GACA::breed(Row mum, Row dad, size_t split)
{
  Row new_row;
  auto it = std::copy(mum.begin(), mum.begin() + split, new_row.begin());
  std::copy(dad.begin() + split, dad.end(), it);
  return new_row;
}

void GACA::show(const Rule & rule, const Population & population, size_t updates, bool value)
{
  for (auto p : population)
  {
    const Row rows = p.StartState();
    for (auto i : rows)
      std::cout << i << ',';
    std::cout << fitness(rows, value) << "   giving ";
    for (auto i : p.State())
      std::cout << i << ',';
    std::cout << fitness(p.State(), value) << '\n';
  }
  std::cout << '\n';
}

Crossover::Crossover(std::random_device::result_type seed,
    size_t population_size,
    const Rule & rule,
    size_t updates,
    bool middle,
    bool target) :
  gen(seed),
  uniform_dist(0, population_size - 1),
  split_dist(0, std::tuple_size<Row>::value - 1),
  rule(rule),
  updates(updates),
  middle(middle),
  target(target)
{
}

Population Crossover::operator()(const Population & population)
{
  const size_t size = population.size();
  if (size-1 != uniform_dist.max())
  {
    std::stringstream ss;
    ss << "Expecting population size " << uniform_dist.max()
       << " got " << size;
    throw std::invalid_argument(ss.str());
  }
  Population new_population;
  auto best_world = best(population, target); 
  new_population.push_back(best_world);

  while(new_population.size() < size) 
  {
    const World & mum = tournament(population[uniform_dist(gen)],
                           population[uniform_dist(gen)],
                           population[uniform_dist(gen)]);
    const World & dad = tournament(population[uniform_dist(gen)],
                           population[uniform_dist(gen)],
                           population[uniform_dist(gen)]);

    Row new_row = breed(mum.StartState(), dad.StartState(),
                       middle ? 
                           std::tuple_size<Row>::value / 2
                        :  split_dist(gen));  
    World child(rule, new_row, updates);
    World winning_world(rule,
            tournament(child, mum, dad).StartState(), 
            updates);
    new_population.push_back(winning_world);
  }
  return new_population;
}

const World & Crossover::tournament(const World & world1,
    const World & world2,
	const World & world3) const
{
  size_t alive1 = fitness(world1.State(), target);
  size_t alive2 = fitness(world2.State(), target);
  size_t alive3 = fitness(world3.State(), target);
  if(alive1 < alive2)
  {
    if(alive1 < alive3)
      return alive2 < alive3 ? world3 : world2;
    return world2;
  }
  if(alive2 < alive3)
    return alive1 < alive3 ? world3 : world1;
  return world1;
}

Mutation::Mutation(std::random_device::result_type seed, double rate) :
  gen(seed),
  uniform_dist(0, std::tuple_size<Row>::value-1),
  rate(rate)
{
}

Row Mutation::operator()(Row row)
{
  auto maybe = uniform_dist(gen);
  if (maybe < rate*row.size())
  {
    auto index = uniform_dist(gen);
    row[index] = !row[index];
  }
  return row;
}

size_t GACA::fitness(const Row & row, Row::value_type target)
{
  return std::count(row.begin(), row.end(), target);
}

const World& GACA::best(const Population & population, bool target)
{
  return *std::max_element(population.cbegin(), population.cend(),
    [&](const World & lhs, const World & rhs)
    {
      return fitness(lhs.State(), target)
		    < fitness(rhs.State(), target);
    });
}
