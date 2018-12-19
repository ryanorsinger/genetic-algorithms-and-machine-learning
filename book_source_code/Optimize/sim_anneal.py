import math
import random

def transitionProbability(old_value, new_value, temperature): 
  if temperature <= 0:
    return 0
  return math.exp((old_value - new_value) / temperature)

def seek(x,
      step,
      f,
      temperature, 
      min_x=float('-inf'),
      max_x=float('inf')):
  best_y = f(x)
  best_x = x
  while temperature > -5:
    jump = False
    if temperature < 0: step /= 2.0 
    possible = [x - step, x + step, x + random.gauss(0, 1)]
    for new_x in [i for i in possible if min_x < i < max_x]:
      y = f(new_x)
      if y < best_y:
        x = new_x
        best_x = new_x
        best_y = y 
      elif transitionProbability(best_y, y, temperature) > random.random():
        jump = True
        x = new_x
    yield best_x, best_y, temperature, jump
    temperature -= 0.1

if __name__ == '__main__':
  temperature = 10
  gen = seek(lambda x: x**2, -1, 0.2, temperature)
  for x, y, t in gen:
    print (x, y, t)

