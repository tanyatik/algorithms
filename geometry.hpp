#pragma once

#include <vector>
#include <math.h>
#include <iostream>

namespace algorithms {

struct Point2D {
    double x;
    double y;
};

static inline double sqr(double x) {
    return x * x;
}

static inline bool operator == (const Point2D& one, const Point2D& other) {
    return (one.x == other.x) && (one.y == other.y);
}

static inline double wedgeProduct(Point2D a, Point2D b, Point2D c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static inline double square(Point2D a, Point2D b, Point2D c) {
    double s = fabs(wedgeProduct(a, b, c)) / 2.0;
    return s;
}

static inline bool clockwise(Point2D a, Point2D b, Point2D c) {
    std::cerr << "cloclwise " << a.x << " " << a.y << " " <<
                            b.x << " " << b.y << " " <<
                            c.x << " " << c.y << "\n";
    return wedgeProduct(a, b, c) < 0;
}

static inline bool counterClockwise(Point2D a, Point2D b, Point2D c) {
    return wedgeProduct(a, b, c) < 0;
}

static inline double getAngle(Point2D a, Point2D b, Point2D c) {
    double a_len2 = sqr(b.x - c.x) + sqr(b.y - c.y);
    double b_len2 = sqr(a.x - c.x) + sqr(a.y - c.y);
    double c_len2 = sqr(b.x - a.x) + sqr(b.y - a.y);

    double cos_alpha = (c_len2 + a_len2 - b_len2) / (2 * sqrt(c_len2) * sqrt(a_len2));

    return acos(cos_alpha);
}

// Returns convex hull of polygom
// Graham-Andrew
// Returns iterator of new sequence end (like std::unique does)
std::vector<Point2D> convexHull(std::vector<Point2D> points);

bool pointInsidePolygon(Point2D point, const std::vector<Point2D>& polygon);
} // namespace algorithms
