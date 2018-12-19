/***
 * Excerpted from "Genetic Algorithms and Machine Learning for Programmers",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material,
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose.
 * Visit http://www.pragmaticprogrammer.com/titles/fbmach for more book information.
***/
#ifndef RULE_INCLUDED
#define RULE_INCLUDED

#include <bitset>
#include <functional>
#include <map>//TODO
#include <random>
#include "cells.h"

namespace GACA
{
  class Rule
  {
  public:
      virtual Row operator()(const Row & cells) const = 0;
  };

  class StaticRule : public Rule
  {
  public:
    virtual Row operator()(const Row & cells) const
    {
      return cells;
    }
  };

  class ECARule : public Rule
  {
  public:
    explicit ECARule(size_t rule) : rule(rule)
    {
    }
    virtual Row operator()(const Row & cells) const
    {
      Row next;
      next.fill(false);
      for (size_t i = 0; i<std::tuple_size<Row>::value; ++i)
      {
        std::bitset<3> state = 0;
        if (i>0)
            state[2] = cells[i - 1];
        state[1] = cells[i];
        if (i<std::tuple_size<Row>::value - 1)
            state[0] = cells[i + 1];

        next[i] = rule[state.to_ulong()];
      }
      return next;
    }
  private:
    const std::bitset<8> rule;
  };

  class DreamRule : public Rule
  {
  public:
    explicit DreamRule(std::random_device::result_type seed) :
        gen(seed)
    {
    }
    virtual Row operator()(const Row & cells) const;
  private:
    mutable std::map<Row, Row> lookup;
    mutable RowGenerator gen;
   };

}

#endif
