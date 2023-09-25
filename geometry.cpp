#pragma once

#include <cmath>
#include <iostream>
#include <vector>
#include <memory>

#include "lib.cpp"

struct Point {
    double x;
    double y;
};

enum class NodeType : char {
    VERTEX,
    INTERSECTION
};

/*
    This node allows for the representation of a graph of two polygons
    joined together. A node can be a vertex or an intersection.
    
    Vertex: node.next1 and node.next2 are the same for a vertex, being the
    next node of the polygon the vertex is a part of.

    Intersection: node.next1 and node.next2 respectively are the next node
    contained by the first and second polygons.
*/
struct Node {
    NodeType type;
    Point value;
    Node* next1;
    Node* next2;
};

const Point ORIGIN = { 0, 0 };

double Distance(Point p1, Point p2) {
    double delta_x = p1.x - p2.x;
    double delta_y = p1.y - p2.y;
    return std::sqrt(delta_x * delta_x + delta_y * delta_y);
}

double DotProduct(Point p1, Point p2, Point p3, Point p4) {
    return (p2.x - p1.x) * (p4.x - p3.x) + (p2.y - p1.y) * (p4.y - p3.y);
}

/*
    Searches for the intersection point of the vertical segment p1-p2 and
    non vertical segment p3-p4
*/
Option<Point> Intercept_SingleVertical(Point p1, Point p2, Point p3, Point p4) {
    Option<Point> result;
    result.some = false;
    double m = (p3.y - p4.y) / (p3.x - p4.x);
    double b = p3.y - m * p3.x;
    Point p;
    p.x = p1.x;
    p.y = m * p.x + b;
    if (p.x < p3.x && p.x < p4.x || p.x > p3.x && p.x > p4.x || p.y < p1.y && p.y < p2.y || p.y > p1.y && p.y > p2.y)
        return result;
    result.some = true;
    result.value = p;
    return result;
}

/*
    Searches for the intersection point of the segments p1-p2 and p3-p4.
*/
Option<Point> Intercept(Point p1, Point p2, Point p3, Point p4) {
    Option<Point> result;
    result.some = false;
    
    if (p1.x == p2.x && p1.y == p2.y || p3.x == p4.x && p3.y == p4.y || p1.x == p2.x && p3.x == p4.x)
        return result;
    if (p1.x == p2.x)
        return Intercept_SingleVertical(p1, p2, p3, p4);
    if (p3.x == p4.x)
        return Intercept_SingleVertical(p3, p4, p1, p2);
    
    double a = (p1.y - p2.y) / (p1.x - p2.x);
    double b = p1.y - a * p1.x;
    double c = (p3.y - p4.y) / (p3.x - p4.x);
    double d = p3.y - c * p3.x; 

    if (a == c)
        return result;
    
    Point p;
    p.x = (d - b) / (a - c);
    p.y = a * p.x + b;
    
    if (p.x < p1.x && p.x < p2.x || p.x > p1.x && p.x > p2.x || p.x < p3.x && p.x < p4.x || p.x > p3.x && p.x > p4.x)
        return result;
    
    result.some = true;
    result.value = p;
    return result;
}

/*
    Returns true if the point c is to the left of the vector a-b
*/
bool isLeft(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) > 0;
}

/*
    Calculates the cosine of the angles P1-P2-P3 and P1-P2-P4 with the
    dot product and chooses the respective point with the smallest cosine.
    It firstly checks if p3 and p4 are to the left or right of the vector
    p1-p2 (since the cosine would be agnostic to that detail) and chooses
    the correct point taking it into account.
*/
bool PickSmallestAngle(Point p1, Point p2, Point p3, Point p4) {
    double len = Distance(p1, p2);
    double cos3 = DotProduct(p1, p2, p2, p3) / (len * Distance(p2, p3));
    double cos4 = DotProduct(p1, p2, p2, p4) / (len * Distance(p2, p4));
    if (isLeft(p1, p2, p3) && isLeft(p1, p2, p4))
        return cos3 < cos4;
    if (isLeft(p1, p2, p3))
        return true;
    if (isLeft(p1, p2, p4))
        return false;
    return cos3 > cos4;
}

std::vector<Node*> PolygonToNodeVector(std::vector<Point> poly) {
    std::vector<Node*> result;
    result.resize(poly.size());
    for (int i = 0; i < result.size(); ++i)
        result[i] = ((Node*) std::malloc(sizeof(Node)));
    for (int i = 0; i < result.size(); ++i) {
        result[i]->type = NodeType::VERTEX;
        result[i]->value = poly[i];
        result[i]->next1 = result[(i + 1) % result.size()];
        result[i]->next2 = result[(i + 1) % result.size()];
    }
    return result;
}

std::vector<Point> Polygon_Or(std::vector<Point> poly1, std::vector<Point> poly2) {
    std::vector<Point> result;
    
    std::vector<Node*> nodes1 = PolygonToNodeVector(poly1);
    std::vector<Node*> nodes2 = PolygonToNodeVector(poly2);
    std::vector<Node*> intersepts;
    Node* head1 = nodes1[0];
    Node* head2 = nodes2[0];

    Node* start;
    double max_distance = 0;
    for (int i = 0; i < nodes1.size(); ++i) {
        double distance = Distance(nodes1[i]->value, ORIGIN);
        if (distance > max_distance) {
            start = nodes1[i];
            max_distance = distance;
        }
    }
    for (int i = 0; i < nodes2.size(); ++i) {
        double distance = Distance(nodes2[i]->value, ORIGIN);
        if (distance > max_distance) {
            start = nodes2[i];
            max_distance = distance;
        }
    }

    /*
        For every combination of edges (2 consecutive points of the polygon),
        check for an intersection and add it if it exists.
    */
    for (int i = 0; i < nodes1.size(); ++i) {
        for (int j = 0; j < nodes2.size(); ++j) {
            Node* start1 = nodes1[i];
            Node* end1 = nodes1[(i + 1) % nodes1.size()];
            Node* start2 = nodes2[j];
            Node* end2 = nodes2[(j + 1) % nodes2.size()];
            auto intersept_result = Intercept(start1->value, end1->value, start2->value, end2->value);
            if (!intersept_result.some)
                continue;
                    
            Node* intersept = (Node*) std::malloc(sizeof(Node));
            intersept->type = NodeType::INTERSECTION;
            intersept->value = intersept_result.value;
            intersept->next1 = end1;
            intersept->next2 = end2;

            Node* temp;
            double distance;

            /*
                start1 and start2 might be followed up by an intersection node, so you
                cannot simply set next1 to intersept for both nodes. Both nodes are
                travelled until it isn't an intersection node or intersept should be
                placed after it (due to the order of 2 or more intersection points) on
                the same edge, it is then added.
            */
            temp = start1;
            distance = Distance(start1->value, intersept->value);
            while (temp->next1->type == NodeType::INTERSECTION && Distance(start1->value, temp->next1->value) < distance)
                temp = temp->next1;
            switch (temp->type) {
                case NodeType::VERTEX: temp->next1 = intersept; temp->next2 = intersept; break;
                case NodeType::INTERSECTION: temp->next1 = intersept;
            }

            temp = start2;
            distance = Distance(start2->value, intersept->value);
            while (temp->next2->type == NodeType::INTERSECTION && Distance(start2->value, temp->next2->value) < distance)
                temp = temp->next2;
            switch (temp->type) {
                case NodeType::VERTEX: temp->next1 = intersept; temp->next2 = intersept; break;
                case NodeType::INTERSECTION: temp->next2 = intersept;
            }

            intersepts.push_back(intersept);
        }
    }

    /*
        Navigate through the graph from start, choosing the node.next1 if the
        node is a vertex and choosing the best next point with the
        PickSmallestAngle function if the node is an intersection.
    */
    Node* previous = start;
    Node* end = start->next1;
    do {
        switch (end->type) {
            case NodeType::VERTEX: {
                previous = end;
                end = end->next1;
                break;
            }
            case NodeType::INTERSECTION: {
                Node* temp = end;
                end = PickSmallestAngle(previous->value, end->value, end->next1->value, end->next2->value) ? end->next1 : end->next2;
                previous = temp;
                break;
            }
        }
        result.push_back(end->value);
    } while (end != start->next1);

    /*
        Free nodes
    */
    for (int i = 0; i < nodes1.size(); ++i)
        std::free(nodes1[i]);
    for (int i = 0; i < nodes2.size(); ++i)
        std::free(nodes2[i]);
    for (int i = 0; i < intersepts.size(); ++i)
        std::free(intersepts[i]);

    return result;
}