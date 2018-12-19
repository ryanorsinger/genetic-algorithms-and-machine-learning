/***
 * Excerpted from "Genetic Algorithms and Machine Learning for Programmers",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material,
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose.
 * Visit http://www.pragmaticprogrammer.com/titles/fbmach for more book information.
***/
#ifndef PARTICLE_INCLUDED
#define PARTICLE_INCLUDED

#include <cmath>
#include <functional>
#include <limits>
#include <random>

namespace Diffuse
{
  class Particle
  {
  public:
    Particle(float x = 0, float y = 0, float edge = 0,
        float max_x = std::numeric_limits<float>::max(),
        float max_y = std::numeric_limits<float>::max(),
        bool breakout = false) 
    :
      x(x), y(y), edge(edge),
      max_x(max_x), max_y(max_y),
      done(false),
      breakout(breakout)
      {
      }

    void Move(float x_step, float y_step)
    {
      if (done) return;

      x += x_step;
      y += y_step;

      if (y < edge / 4)
      {
        done = true;
        return;
      }
      if (y > max_y) y = max_y;

      if (!breakout)
      {
        if (x < edge / 2) x = edge / 2;
        if (x > max_x) x = max_x;
      }
    }

    float X() const { return x; }
    float Y() const { return y; }

  private:
    float x;
    float y;
    const float edge;
    const float max_x;
    const float max_y;
    bool done;
    const bool breakout;
  };
}

#endif
