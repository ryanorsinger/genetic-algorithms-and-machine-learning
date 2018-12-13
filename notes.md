# Genetic Algorithms

### Vocabulary
candidate solution
stopping criteria
hyperparameters
heuristics  
fitness functions
stochastic search (random search)
fitness, objective, and cost functions
    
### Important questions to ask about ML process/results
- How did you build it? If it needs data to learn, remember: Garbage in, garbage out. Bias in, bias out.5
- How did you test it? Is it doing what you expect?
- Does it work? Have you got a solution to your problem?
- What parameters did you use? Are these good enough or will something else work better?
- Does it apply generally? Or does it only work for your current problem and data?

### Decision trees
- two forms: classification trees (categorical) and regression trees (numeric)
- Once you have a decision tree, you’ll turn it into a ruleset and prune it back to locate the paper bag...
- type of classifier
- use domain knowledge and entropy to guide the tree's growth
- classification trees (categorical)
- regression trees (numerical)
- ask a question and take a branch depending on the answer

#### Ways to build a decision tree:
- "bottom up" builds a classifier one data item at a time
- Top-Down Induction of Decision Trees (TDIDT) starts w/ all training data and divides it

`data = [['a', 0, 'good'], ['b', -1, 'bad'], ['a', 101, 'good']]`
`label = ['letter', 'number', 'class']`
`tree = {'letter': {'a': 'good', 'b': 'bad'}}`

### How the decision tree partiitons data on a feature
Quicksort uses a pivot point to divide the data into low and high values. A decision tree partitions the data using a feature instead of a pivot, but still recursively builds up a decision tree.

```python
ID3(data, features, tree = {}):
if data is (mostly) in same category:
    return leaf_node(data)
  feature = pick_one(data, features)
  tree[feature]={}
  groups = partition(data, feature)
  for group in groups:
    tree[feature][group] = ID3(group, features)
  return tree
```

### How to decide the best feature
- Since entropy doesn’t need detailed statistical knowledge, it’s a good place to start...
- Entropy tells you how homogeneous a data set is. Entropy captures the concept of randomness or chaos in a system
- A coin with heads on both sides has zero entropy.
- To find the entropy of your data set, you’ll be finding the logarithm of fractions.
```python
data = [[0, 0, False],
        [-1, 0, True],
        [1, 0, True],
        [0, -1, True],
        [0, 1, True]]
label = ['x', 'y', 'out']

```
- If you use base two, the entropy of a binary class, like heads/tails or inside/outside of the bag, is between 0 and 1 (no need to normalize if we're dealing w/ a binary classification)

- try to identify the feature with the lowest entropy
- calculate information gain and pick the variable with the highest gain. This gain is the difference between the baseline entropy and entropy if you split on one attribute.