from decision_tree import *
import unittest

class TestLeafNode(unittest.TestCase):
  def test_that_pure_node_is_pure(self):
    d = [[1], [1], [1]]
    category, count = potential_leaf_node(d)
    self.assertEqual(count, len(d))

class TestClassify(unittest.TestCase):
  def test_that_binary_split_follows_correct_branch(self):
    tree = {'a': {0: 1, 1: 2}}
    self.assertEqual(classify(tree, ['a'], [0]), 1)
    self.assertEqual(classify(tree, ['a'], [1]), 2)
    self.assertEqual(classify(tree, ['a'], [2]), None)


  def test_that_ternary_split_follows_correct_branch(self):
    tree = {'a': {0: 1, 1: 2, 2: 3}}
    self.assertEqual(classify(tree, ['a'], [0]), 1)
    self.assertEqual(classify(tree, ['a'], [1]), 2)
    self.assertEqual(classify(tree, ['a'], [2]), 3)
    self.assertEqual(classify(tree, ['a'], [3]), None)

  def test_that_tree_followed_to_leaf(self):
    tree = {'a': {0: {'b': {0: 1, 1: 2}}, 1: {'c': {10: 1, 11: 2}}}}
    self.assertEqual(classify(tree, ['a', 'b', 'c'], [0, 0, 11]), 1)


class TestTreeAsRuleStr(unittest.TestCase):
  def test_that_binary_split_states_both_branches(self):
    tree = {'a': {0: 1, 1: 2}}
    self.assertEqual(as_rule_str(tree, ['a']), 'if a = 0 then 1.\nif a = 1 then 2.\n\n')

  def test_that_each_part_of_subtree_stated_in_rule(self):
    tree = {'x': {0: {'y': {0: False, -1: False, -2: True, -3: True}},
          -1: {'y': {0 : False, -1: False, -2: True, -3: True}}, -2: True, -3: True}}
    expected = '''if x = 0:
  if y = 0 then False, if y = -2 then True, if y = -1 then False, if y = -3 then True
if x = -2 then True.
if x = -1:
  if y = 0 then False, if y = -2 then True, if y = -1 then False, if y = -3 then True
if x = -3 then True.

'''
    self.assertEqual(as_rule_str(tree, ['x', 'y', 'out']), expected)

class TestEntropy(unittest.TestCase):
  def test_entropy_two_different_classes_distribution_is_one(self):
    self.assertEqual(entropy([[1], [2]]), 1.)

  def test_entropy_three_different_classes_distribution_is_at_least_one(self):
    #https://stats.stackexchange.com/questions/95261/why-am-i-getting-information-entropy-greater-than-1
    self.assertGreaterEqual(entropy([[1], [2], [3]]), 1.) #1.584962500721156

  def test_entropy_equal_distribution_is_zero(self):
    self.assertEqual(entropy([[1], [1], [1]]), 0.) 
    self.assertEqual(entropy([[1], [1]]), 0.)

class TestBestFeatureForSplit(unittest.TestCase):
  def test_pure_feature_found(self):
    data = [['a', 0, 1], ['b', -1, 2], ['a', 101, 1]]
    self.assertEqual(best_feature_for_split(data), 0)

class test_find_edges(unittest.TestCase):
  def test_finds_largest_left_and_bottom_points(self):
    data = [[0, 0, False], [-1, 0, False], [-2, 0, True], [-3, 0, True],
      [0, -1, False], [-1, -1, False], [-2, -1, True], [-3, -1, True],
      [0, -2, True], [-1, -2, True], [-2, -2, True], [-3, -2, True],
      [0, -3, True], [-1, -3, True], [-2, -3, True], [-3, -3, True]]
    label = ['x', 'y', 'out']
    tree = create_tree(data, label)
    X = [-3, -2, -1, 0]
    Y = [-3, -2, -1, 0]
    (min_x, min_y), (max_x, max_y) = find_edges(tree, label, X, Y)
    self.assertEqual(min_x, -1)
    self.assertEqual(min_y, -1)

  def test_finds_smallest_right_and_top_point(self):
    data = [[x, y, -1 < x < 1 and -1 < y < 1] for x in range(-2, 3) for y in range(-2, 3)]
    label = ['x', 'y', 'out']
    tree = create_tree(data, label)
    X = [-3, -2, -1, 0]
    Y = [-3, -2, -1, 0]
    (min_x, min_y), (max_x, max_y) = find_edges(tree, label, X, Y)
    self.assertEqual(max_x, -1)
    self.assertEqual(max_y, -1)

  def test_fails_if_bag_not_centered_on_origin(self):
    data = [[0, 0, True], [-1, 0, True], [-2, 0, True], [-3, 0, True],
      [0, -1, True], [-1, -1, True], [-2, -1, True], [-3, -1, True],
      [0, -2, False], [-1, -2, False], [-2, -2, True], [-3, -2, True],
      [0, -3, False], [-1, -3, False], [-2, -3, True], [-3, -3, True]]
    label = ['x', 'y', 'out']
    tree = create_tree(data, label)
    X = [-3, -2, -1, 0]
    Y = [-3, -2, -1, 0]
    self.assertRaises(ValueError, find_edges, tree, label, X, Y)

if __name__  == '__main__':
  unittest.main()
