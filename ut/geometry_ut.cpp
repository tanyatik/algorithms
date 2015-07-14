#include <gtest/gtest.h>

#include "../geometry.hpp"

using algorithms::Point2D;

TEST(geometry, convex_hull) {
    // square
    std::vector<Point2D> square;

    square.push_back(Point2D{ -1, -1 });
    square.push_back(Point2D{ -1, 1 });
    square.push_back(Point2D{ 1, -1 });
    square.push_back(Point2D{ 1, 1 });

    double s;
    std::vector<Point2D> convex = convexHull(square, &s);
    EXPECT_EQ(convex.size(), square.size());
    EXPECT_EQ(convex[0], square[0]);
    EXPECT_EQ(convex[1], square[1]);
    EXPECT_EQ(convex[2], square[2]);
    EXPECT_EQ(convex[3], square[3]);
    EXPECT_NEAR(4, s, 0.01);

    // strange non-convex figure
    std::vector<Point2D> nonConvexFigure;
    nonConvexFigure.push_back(Point2D{ -1, -1 });
    nonConvexFigure.push_back(Point2D{ -1, 1 });
    nonConvexFigure.push_back(Point2D{ 0.5, 0.5 });
    nonConvexFigure.push_back(Point2D{ 1, -1 });
    nonConvexFigure.push_back(Point2D{ 1, 1 });

    // should become same square
    s = 0;
    convex = convexHull(nonConvexFigure, &s);

    EXPECT_EQ(convex.size(), square.size());
    EXPECT_EQ(convex[0], square[0]);
    EXPECT_EQ(convex[1], square[1]);
    EXPECT_EQ(convex[2], square[2]);
    EXPECT_EQ(convex[3], square[3]);
    EXPECT_NEAR(4, s, 0.01);
}

TEST(geometry, point_inside) {
    // square
    std::vector<Point2D> square;
    square.push_back(Point2D{ -1, -1 });
    square.push_back(Point2D{ -1, 1 });
    square.push_back(Point2D{ 1, 1 });
    square.push_back(Point2D{ 1, -1 });

    EXPECT_TRUE(pointInsidePolygon(Point2D{0, 0}, square));
    EXPECT_TRUE(pointInsidePolygon(Point2D{0.5, 0.3}, square));
    EXPECT_TRUE(pointInsidePolygon(Point2D{-0.5, -0.3}, square));
    EXPECT_TRUE(pointInsidePolygon(Point2D{-0.5, 0.3}, square));
    EXPECT_TRUE(pointInsidePolygon(Point2D{0.5, -0.3}, square));

    EXPECT_FALSE(pointInsidePolygon(Point2D{2, 0}, square));
    EXPECT_FALSE(pointInsidePolygon(Point2D{2.5, 0.3}, square));

    EXPECT_FALSE(pointInsidePolygon(Point2D{-0.5, -2.3}, square));
    EXPECT_FALSE(pointInsidePolygon(Point2D{-0.5, 1.3}, square));
    EXPECT_FALSE(pointInsidePolygon(Point2D{5.5, -0.3}, square));
}
