#include <vector>
#include <algorithm>
#include <assert.h>

#include "geometry.hpp"

namespace algorithms {

bool pointInsidePolygon(Point2D point, const std::vector<Point2D>& polygon) {
    // polygon points should be sorted by angle

    Point2D first = *polygon.begin(), second = *(polygon.begin() + 1);

    auto left = polygon.begin(), right = polygon.end();
    double angle = getAngle(point, first, second);

    while (left < right) {
        if (left + 1 == right) {
            // found angle
            return (right == polygon.end()) ? false : !clockwise(*left, point, *right);
        }

        auto middle = left + (std::distance(left, right))/ 2;
        assert(middle < polygon.end());

        double middleAngle = getAngle(*middle, first, second);

        if (middleAngle <= angle) {
            left = middle;
        } else {
            right = middle;
        }
    }
    assert(true);
    return false;
}

static inline double signedSquare(Point2D a, Point2D b, Point2D c) {
    double s = wedgeProduct(a, b, c) / 2.0;
    return s;
}

double square(const std::vector<Point2D>& points) {
    double square = 0;
    Point2D point = points.front();
    for (auto left = points.begin() + 1, right = left + 1; right != points.end(); ++left, ++right) {
        square += signedSquare(point, *left, *right);
    }
    return fabs(square);
}


std::vector<Point2D> convexHull(std::vector<Point2D> points) {
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
    resultUp.push_back(leftmost);

    for (auto point = points.begin() + 1; point != points.end(); ++point) {
        if (point == points.end() - 1 || clockwise(leftmost, *point, rightmost)) {
            if (upPrev != points.end() && clockwise(*upPPrev, *upPrev, *point)) {
                upPPrev = upPrev;
                resultUp.push_back(*upPrev);
                upPrev = point;
            }
            upPrev = point;
        } else if (point == points.end() - 1 || !clockwise(leftmost, *point, rightmost)) {
            if (downPrev != points.end() && !clockwise(*downPPrev, *downPrev, *point)) {
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
