from escape import *
import unittest

class FakeTurtle:
  def __init__(self, pos):
    self.pos = pos

  def position(self):
    return self.pos

class TestEscaped(unittest.TestCase):
  def test_that_35_35_is_not_out(self):
    self.assertFalse(escaped( (35, 35) ))

  def test_that_minus_35_minus_35_is_not_out(self):
    self.assertFalse(escaped( (-35, -35) ))

  def test_that_36_36_is_out(self):
    self.assertTrue(escaped( (36, 36) ))

  def test_that_minus_36_minus_36_is_out(self):
    self.assertTrue(escaped( (-36, -36) ))

class TestPositionData(unittest.TestCase):
  def test_that_storing_point_increases_list_size(self):
    L = []
    t = FakeTurtle((0, 0))
    store_position_data(L, t)
    self.assertEqual(len(L), 1)

  def test_that_storing_point_inside_bag_marks_it_as_false(self):
    L = []
    t = FakeTurtle((0, 0))
    store_position_data(L, t)
    self.assertFalse(L[0][2])

  def test_that_storing_point_outside_bag_marks_it_as_true(self):
    L = []
    t = FakeTurtle((50, 50))
    store_position_data(L, t)
    self.assertTrue(L[0][2])

if __name__  == '__main__':
  unittest.main()
