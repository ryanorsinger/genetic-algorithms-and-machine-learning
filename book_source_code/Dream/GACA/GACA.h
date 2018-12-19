/***
 * Excerpted from "Genetic Algorithms and Machine Learning for Programmers",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material,
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose.
 * Visit http://www.pragmaticprogrammer.com/titles/fbmach for more book information.
***/
#ifndef GACA_INCLUDED
#define GACA_INCLUDED

#include <functional>
#include <random>
#include <vector>
#include "cells.h"
#include "rule.h"

namespace GACA
{
  Row breed(Row mum, Row dad, size_t split);
  size_t fitness(const Row & cell, Row::value_type value);

  class World
  {
  public:
    World(const Rule & rule, Row row, size_t height);
    void Reset(Row row);

    bool Alive(size_t x, size_t y) const;
    size_t Height() const {  return height; }
    size_t Width() const { return row.size(); }
    Row State() const { return row; }
    Row StartState() const { return history[0]; }
  private:
    const Rule & rule;
    Row row;
    size_t height;
    std::vector<Row> history;
    void Run();
  };

  typedef std::vector<World> Population;

  class Crossover
  {
  public:
      Crossover(std::random_device::result_type seed,
        size_t population_size,
        const Rule & rule,
        size_t updates,
        bool middle,
        bool target);
      Population operator()(const Population & population);
	  const World & Crossover::tournament(const World & world1,
		  const World & world2,
		  const World & world3) const;
  private:
      std::default_random_engine gen;
      std::uniform_int_distribution<size_t> uniform_dist;
      std::uniform_int_distribution<size_t> split_dist;
      const Rule & rule;
      const size_t updates;
      const bool middle;
      const bool target;
  };

  class Mutation
  {
  public:
      Mutation(std::random_device::result_type seed, double rate);
      Row operator()(Row cell);
  private:
      std::default_random_engine gen;
      std::uniform_int_distribution<size_t> uniform_dist;
      const double rate;
  };

  const World& best(const Population & population, bool target);

  void show(const Rule & rule, const Population & population, size_t updates, bool target);
  std::ostream& operator<< (std::ostream & os, const DreamRule & rule);
}

#endif
