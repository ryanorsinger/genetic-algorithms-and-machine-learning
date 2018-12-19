/***
 * Excerpted from "Genetic Algorithms and Machine Learning for Programmers",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material,
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose.
 * Visit http://www.pragmaticprogrammer.com/titles/fbmach for more book information.
***/
#ifndef AIR_INCLUDED
#define AIR_INCLUDED

#include <random>

namespace Diffuse
{
  class Air
  {
    std::mt19937 engine;
    std::normal_distribution<float> normal_dist;
    const float step;

  public:
    Air(float step,
      unsigned int seed = 1)
      :
      step(step),
      engine(seed)
    {
    }

    float Bump()
    {
      return step * normal_dist(engine);
    }
  };
}
#endif
