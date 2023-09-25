#include <iostream>
#include <vector>

#include "geometry.cpp"

const std::vector<Point> TRIANGLE_1 = {
    { 0, 0 },
    { 5, 10 },
    { 10, 0 }
};
const std::vector<Point> TRIANGLE_2 = {
    { 5, 5 },
    { 10, 15 },
    { 15, 5 }
};
const std::vector<Point> SQUARE_1 = {
    { 0, 0 },
    { 0, 10 },
    { 10, 10 },
    { 10, 0 }
};
const std::vector<Point> SQUARE_2 = {
    { 5, 5 },
    { 5, 15 },
    { 15, 15 },
    { 15, 5 }
};
const std::vector<Point> WEIRD_SHAPE_1 = {
    { 0, 3 },
    { 4, 6 },
    { 5, 1 },
    { 1, 0 }
};
const std::vector<Point> WEIRD_SHAPE_2 = {
    { 2, 6 },
    { 6, 5 },
    { 4, 3 },
    { 4, 0 },
    { 0, 1 }
};

void PrintPolygon(std::vector<Point> polygon) {
    for (int i = 0; i < polygon.size(); ++i)
        std::cout << "X: " << polygon[i].x << " Y: " << polygon[i].y << "\n";
    std::cout << "\n";
}

int main() {
    PrintPolygon(Polygon_Or(TRIANGLE_1, TRIANGLE_2));
    PrintPolygon(Polygon_Or(SQUARE_1, SQUARE_2));
    PrintPolygon(Polygon_Or(WEIRD_SHAPE_1, WEIRD_SHAPE_2));
}