#pragma once

#include <vector>
#include <math.h>
#include <iostream>

namespace algorithms {

struct Point2D {
    double x;
    double y;
};

static inline double Sqr(double x) {
    return x * x;
}

static inline bool operator == (const Point2D& one, const Point2D& other) {
    return (one.x == other.x) && (one.y == other.y);
}

static inline std::ostream& operator << (std::ostream& stream, const Point2D& point) {
    stream << " (" << point.x << "," << point.y << ") ";
    return stream;
}

static inline double WedgeProduct(Point2D a, Point2D b, Point2D c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static inline double Square(Point2D a, Point2D b, Point2D c) {
    double s = fabs(WedgeProduct(a, b, c)) / 2.0;
    return s;
}

static inline bool Clockwise(Point2D a, Point2D b, Point2D c) {
    return WedgeProduct(a, b, c) < 0;
}

static inline bool CounterClockwise(Point2D a, Point2D b, Point2D c) {
    return WedgeProduct(a, b, c) > 0;
}

static inline double GetAngle(Point2D a, Point2D b, Point2D c) {
    double a_len2 = Sqr(b.x - c.x) + Sqr(b.y - c.y);
    double b_len2 = Sqr(a.x - c.x) + Sqr(a.y - c.y);
    double c_len2 = Sqr(b.x - a.x) + Sqr(b.y - a.y);

    double cos_alpha = (c_len2 + a_len2 - b_len2) / (2 * sqrt(c_len2) * sqrt(a_len2));

    return acos(cos_alpha);
}

// Returns convex hull of polygom
// Graham-Andrew
// Returns iterator of new sequence end (like std::unique does)
std::vector<Point2D> ConvexHull(std::vector<Point2D> points);

bool PointInsidePolygon(Point2D point, const std::vector<Point2D>& polygon);

double Square(const std::vector<Point2D>& points);
} // namespace algorithms
