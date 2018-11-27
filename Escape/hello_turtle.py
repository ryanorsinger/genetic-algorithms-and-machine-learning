import turtle

def draw_bag():
  turtle.shape('turtle')
  turtle.pen(pencolor='brown', pensize=5)
  turtle.penup()
  turtle.goto(-35, 35)
  turtle.pendown()
  turtle.right(90)
  turtle.forward(70)
  turtle.left(90)
  turtle.forward(70)
  turtle.left(90)
  turtle.forward(70)

if __name__ == '__main__':
  turtle.setworldcoordinates(-70., -70., 70., 70.)
  draw_bag()
  turtle.mainloop()

def escaped(position):
  x = int(position[0])
  y = int(position[1])
  return x < -35 or x > 35 or y < -35 or y > 35


def draw_line():
  angle = 0
  step = 5
  t = turtle.Turtle()
  while not escaped(t.position()):
    t.left(angle)
    t.forward(step)


draw_line()