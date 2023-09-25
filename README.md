# geometry-logic
## Description
This repository contains a function Polygon_Or which takes 2 polygons
which are represented as vectors of points and joins them.
## How it works
This algorithm works by creating a graph of the polygon, where each
node is either a vertex or intersection of the two polygons, and
navigating its outer edge until it finds the starting point.

- The starting point is searched, which is just the farthest
vertex from either polygon from the origin. This point will always
be part of the result.
- The intersection points are found and added to the graph.
- The graph is navigated from the starting point found previously
with the following logic: if the current point is a vertex, move to
the next point; if the current point is an intersection, move to the
point which closes the angle between the segment previous-current and
current-next the most to the left.
<img src="https://github.com/DuarteBarbosaRibeiro/geometry-logic/blob/main/img/intersection.png?raw=true" alt="intersection-example" width="200">
The vector BC makes a smaller angle with the vector AB but BD is
chosen because the points are assumed to be ordered clockwise,
therefore, to the right of the vector AB is the inside of one
of the polygons, so point C doesn't border the union of the two
polygons.

## Examples
### 2 Triangles
Triangle 1: (0, 0), (5, 10), (10, 0)\
Triangle 2: (5, 5), (10, 15), (15, 5)\
Result: (7.5, 5), (10, 0), (0, 0), (5, 10), (6.25, 7.5), (10, 15), (15, 5)\
<img src="https://github.com/DuarteBarbosaRibeiro/geometry-logic/blob/main/img/triangle.png?raw=true" alt="2-triangles-result" width="200">

### 2 Squares
Square 1: (0, 0), (0, 10), (10, 10), (10, 0)\
Square 2: (5, 5), (5, 15), (15, 15), (15, 5)\
Result: (10, 5), (10, 0), (0, 0), (0, 10), (5, 10), (5, 15), (15, 15), (15, 5)\
<img src="https://github.com/DuarteBarbosaRibeiro/geometry-logic/blob/main/img/square.png?raw=true" alt="2-squares-result" width="200">

### "Weird shapes"
Weird Shape 1: (0, 3), (4, 6), (5, 1), (1, 0)\
Weird Shape 2: (2, 6), (6, 5), (4, 3), (4, 0), (0, 1)\
Result: (5, 1), (4, 0.75), (4, 0), (2.5, 0.375), (1, 0), (0.727273, 0.818182), (0, 1), (0.363636, 1.90909), (0, 3), (1.14286, 3.85714), (2, 6), (3.5, 5.625), (4, 6), (4.10526, 5.47368), (6, 5), (4.5, 3.5)\
<img src="https://github.com/DuarteBarbosaRibeiro/geometry-logic/blob/main/img/weird_shape.png?raw=true" alt="weird-shapes-result" width="200">