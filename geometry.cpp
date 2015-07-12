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
            return clockwise(*left, point, *right);
        }

        auto middle = left + (std::distance(left, right))/ 2;
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


std::vector<Point2D> convexHull(std::vector<Point2D> points, double* s) {
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

    vector<Point2D> result;

    Point2D leftmost = points.front();
    Point2D rightmost = points.back();

    auto upPPrev = points.begin();
    auto downPPrev = points.begin();
    auto upPrev = points.end();
    auto downPrev = points.end();

    if (s) {
        *s = 0;
    }
    result.push_back(leftmost);

    for (auto point = points.begin() + 1; point != points.end(); ++point) {
        if (point == points.end() - 1 || clockwise(leftmost, *point, rightmost)) {
            if (upPrev == points.end()) {
                upPrev = point;
            } else if (clockwise(*upPPrev, *upPrev, *point)) {
                if (s) {
                    *s += square(*upPPrev, *upPrev, *point);
                }
                upPPrev = upPrev;
                result.push_back(*upPrev);
                upPrev = point;
            } else {
                upPrev = point;
            }
        }
        if (point == points.end() - 1 || !clockwise(leftmost, *point, rightmost)) {
            if (downPrev == points.end()) {
                downPrev = point;
            } else if (!clockwise(*downPPrev, *downPrev, *point)) {
                if (s) {
                    *s += square(*downPPrev, *downPrev, *point);
                }
                downPPrev = downPrev;
                result.push_back(*downPrev);
                downPrev = point;
            } else {
                downPrev = point;
            }
        }
    }
    result.push_back(rightmost);

    return result;
}



} // namespace algorithms
