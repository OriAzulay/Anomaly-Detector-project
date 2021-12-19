// ID1 : 206336794

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <cmath>
#include <vector>
#include <assert.h>
#include "anomaly_detection_util.h"

using namespace std;

// ------------ DO NOT CHANGE -----------
class Circle
{
public:
    Point center;
    float radius;
    Circle(Point c, float r) : center(c), radius(r) {}
};
// --------------------------------------

// implement

// A function for distance between 2 points
float distance(const Point &a, const Point &b);

// A method to check if the point is inside the circle or in the boundery.
bool PointInCircle(const Circle &c, const Point &p);

// A method to get the circumcircle by 4 values (got help from wikipedia circumCenter equation)
Point getCenter(float ax, float ay, float bx, float by);

// Get circle by 2 points (the middle is the center and radius)
Circle CircleBy2Points(const Point &A, const Point &B);

// Get circle by 3 points (by circumcircle equation)
Circle CircleBy3Points(const Point &A, const Point &B, const Point &C);

// check if the given point is in the boundery or not
bool CircleCheck(const Circle &c, const vector<Point> &Bdr);

// check after the recursion if the boundery is 1\2\3 points.
Circle trivialCircle(vector<Point> &Bdr);

// main welzl's algoritem - check every recursive prograss if the point
// is in the boundery or inside the circle, at the end go to the trivialCircle method
Circle welzlAlg(Point **p, vector<Point> Bdr, int n);

Circle findMinCircle(Point **points, size_t size);

#endif /* MINCIRCLE_H_ */
