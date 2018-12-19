import unittest
import hill_climb
import into_bag
import sim_anneal
import math

class climb(unittest.TestCase):
  def test_flatline_goes_right(self):
    gen = hill_climb.seek(0, 0.1, lambda x: 1)
    for x, y in gen:
      self.assertEqual(x, 0.1)
      break # will keep going right

  def test_stays_at_low_point(self):
    gen = hill_climb.seek(0, 0.1, lambda x: math.fabs(x))
    count = 0
    for x, y in gen:
      self.assertEqual(x, 0)
      self.assertEqual(y, 0)
    self.assertEqual(count, 0)

  def test_goes_left_if_left_lower(self):
    gen = hill_climb.seek(0, 0.1, lambda x: x)
    for x, y in gen:
      self.assertEqual(x, -0.1)
      break

  def test_goes_right_if_right_lower(self):
    gen = hill_climb.seek(0, 0.1, lambda x: -x)
    for x, y in gen:
      self.assertEqual(x, 0.1)
      break


class anneal(unittest.TestCase):

  def test_transition_probability_is_zero_when_cooled(self):
    cool_temperature = -5.0
    self.assertEqual(sim_anneal.transitionProbability(1., 0., cool_temperature), 0)

  def test_stays_at_low_point_when_cooled(self):
    start_x = 0
    step = 0.1
    f = lambda x: math.fabs(x)
    cool_temperature = -5.0
    gen = sim_anneal.seek(start_x, step, f, cool_temperature)
    for x, y, temperature, jump in gen:
      self.assertEqual(x, start_x)
      self.assertEqual(y, f(start_x))

  def test_temperature_stable_point_when_cooled(self):
    start_x = 0
    step = 0.1
    f = lambda x: math.fabs(x)
    cool_temperature = -5.0
    gen = sim_anneal.seek(start_x, step, f, cool_temperature)
    for x, y, temperature, jump in gen:
      self.assertEqual(temperature, cool_temperature)

  def test_point_stays_within_bounds_when_bounded(self):
    max_x = 3
    min_x = -3
    start_x = 2
    step = 1.
    start_temperature = 10.0
    f = lambda x: math.fabs(x)
    gen = sim_anneal.seek(start_x, step, f, start_temperature, min_x, max_x)
    for x, y, temperature, jump in gen:
      self.assertLess(x, max_x)
      self.assertGreater(x, min_x)

  def test_finds_close_to_lowest_point(self):
    prev_x = -1.
    prev_y = 0
    step = 0.1
    f = lambda x: math.fabs(x)
    temperature = 15.0
    gen = sim_anneal.seek(prev_x, step, f, temperature)
    for x, y, temperature, jump in gen:
      prev_x = x
      prev_y = y
    self.assertLess(math.fabs(prev_x), 0.05)
    self.assertLess(math.fabs(prev_y), 0.05)

if __name__ == '__main__':
  unittest.main()

