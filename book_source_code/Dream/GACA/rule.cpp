#include "rule.h"

using namespace GACA;


Row DreamRule::operator()(const Row & cells) const
{
  auto it = lookup.find(cells);
  if (it != lookup.end())
    return it->second;
  Row return_cell = gen.generate();
  lookup[cells] = return_cell;
  return return_cell;
}
