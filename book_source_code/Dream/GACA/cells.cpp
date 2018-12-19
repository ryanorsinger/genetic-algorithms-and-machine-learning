#include "cells.h"

using namespace GACA;

Row GACA::RowGenerator::generate()
{
  Row cells;
  for (size_t i = 0; i < cells.size(); ++i)
  {
    cells[i] = (uniform_dist(gen) == 1);
  }
  return cells;
}
