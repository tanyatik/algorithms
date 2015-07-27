#include <vector>
#include <algorithm>
#include <assert.h>

#include "geometry.hpp"

namespace algorithms {

// polygon points should be sorted clockwise and start from leftmost (and down)
bool PointInsidePolygon(Point2D point, const std::vector<Point2D>& polygon) {

    Point2D first = *polygon.begin(), second = *(polygon.begin() + 1);

    int left = 0, right = polygon.size();
    double angle = GetAngle(second, first, point);

    std::vector<double> angles;
    for (const Point2D& point : polygon) {
        angles.push_back(GetAngle(second, first, point));
    }

    while (left < right) {
        if (left + 1 == right) {
            return CounterClockwise(polygon[left], point, (right == polygon.size()) ? first : polygon[right]);
        }

        int middle = left + (right - left)/ 2;
        assert(middle < polygon.size());

        double middleAngle = angles[middle];

        if (middleAngle < angle) {
            left = middle;
        } else {
            right = middle;
        }
    }
    assert(true);
    return false;
}

static inline double SignedSquare(Point2D a, Point2D b, Point2D c) {
    double s = WedgeProduct(a, b, c) / 2.0;
    return s;
}

double Square(const std::vector<Point2D>& points) {
    double square = 0;
    Point2D point = points.front();
    for (auto left = points.begin() + 1, right = left + 1; right != points.end(); ++left, ++right) {
        square += SignedSquare(point, *left, *right);
    }
    return fabs(square);
}


std::vector<Point2D> ConvexHull(std::vector<Point2D> points) {
    // find leftmost and rightmost points
    using std::pair;
    using std::vector;
    using std::sort;

    assert(points.size() >= 3);
    if (points.size() == 3) {
        return points;
    }

    using std::swap;
    sort(points.begin(), points.end(), [] (const Point2D& left, const Point2D& right) -> bool {
        return left.x < right.x || (left.x == right.x && left.y < right.y);
    });


    Point2D leftmost = points.front();
    Point2D rightmost = points.back();

    auto upPPrev = points.begin();
    auto downPPrev = points.begin();
    auto upPrev = points.end();
    auto downPrev = points.end();

    vector<Point2D> resultUp, resultDown;
    resultDown.push_back(leftmost);

    for (auto point = points.begin() + 1; point != points.end(); ++point) {
        if (point == points.end() - 1 || Clockwise(leftmost, *point, rightmost)) {
            if (upPrev != points.end() && Clockwise(*upPPrev, *upPrev, *point)) {
                upPPrev = upPrev;
                resultUp.push_back(*upPrev);
                upPrev = point;
            }
            upPrev = point;
        }
        if (point == points.end() - 1 || CounterClockwise(leftmost, *point, rightmost)) {
            if (downPrev != points.end() && CounterClockwise(*downPPrev, *downPrev, *point)) {
                downPPrev = downPrev;
                resultDown.push_back(*downPrev);
                downPrev = point;
            }
            downPrev = point;
        }
    }
    resultDown.push_back(rightmost);
    resultDown.insert(resultDown.end(), resultUp.rbegin(), resultUp.rend());

    return resultDown;
}


} // namespace algorithms
