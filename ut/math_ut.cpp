#include <gtest/gtest.h>

#include "math/geometry.hpp"

using namespace algorithms;

TEST(convex_hull, small_square) {
    // square
    std::vector<Point2D> square;

    square.push_back(Point2D{ -1, -1 });
    square.push_back(Point2D{ 1, -1 });
    square.push_back(Point2D{ 1, 1 });
    square.push_back(Point2D{ -1, 1 });

    std::vector<Point2D> convex = ConvexHull(square);
    EXPECT_EQ(square.size(), convex.size());
    EXPECT_EQ(convex[0], square[0]);
    EXPECT_EQ(convex[1], square[1]);
    EXPECT_EQ(convex[2], square[2]);
    EXPECT_EQ(convex[3], square[3]);
}

TEST(convex_hull, non_convex_figure) {
    // strange non-convex figure
    std::vector<Point2D> nonConvexFigure;
    nonConvexFigure.push_back(Point2D{ -1, -1 });
    nonConvexFigure.push_back(Point2D{ -1, 1 });
    nonConvexFigure.push_back(Point2D{ 0.5, 0.5 });
    nonConvexFigure.push_back(Point2D{ 1, -1 });
    nonConvexFigure.push_back(Point2D{ 1, 1 });

    std::vector<Point2D> square;

    square.push_back(Point2D{ -1, -1 });
    square.push_back(Point2D{ 1, -1 });
    square.push_back(Point2D{ 1, 1 });
    square.push_back(Point2D{ -1, 1 });

    std::vector<Point2D> convex = ConvexHull(nonConvexFigure);

    EXPECT_EQ(square.size(), convex.size());
    EXPECT_EQ(convex[0], square[0]);
    EXPECT_EQ(convex[1], square[1]);
    EXPECT_EQ(convex[2], square[2]);
    EXPECT_EQ(convex[3], square[3]);
}

TEST(convex_hull, repeating_points) {
    // square
    std::vector<Point2D> square;

    square.push_back(Point2D{ 2, 0 });
    square.push_back(Point2D{ 0, 0 });
    square.push_back(Point2D{ 2, 1 });
    square.push_back(Point2D{ 0, 1 });
    square.push_back(Point2D{ 0, 0.5});
    square.push_back(Point2D{ 1, 2 });
    square.push_back(Point2D{ 1, 0 });
    square.push_back(Point2D{1.5, 2});
    square.push_back(Point2D{ 2, 2 });
    square.push_back(Point2D{ 0, 2 });

    std::vector<Point2D> convex = ConvexHull(square);
    EXPECT_EQ(4, convex.size());
    EXPECT_EQ((Point2D{ 0, 0 }), convex[0]);
    EXPECT_EQ((Point2D{ 2, 0 }), convex[1]);
    EXPECT_EQ((Point2D{ 2, 2 }), convex[2]);
    EXPECT_EQ((Point2D{ 0, 2 }), convex[3]);
}

TEST(geometry, point_inside) {
    // square
    std::vector<Point2D> square;
    square.push_back(Point2D{ -1, -1 });
    square.push_back(Point2D{ -1, 1 });
    square.push_back(Point2D{ 1, 1 });
    square.push_back(Point2D{ 1, -1 });

    EXPECT_TRUE(PointInsidePolygon(Point2D{0, 0}, square));
    EXPECT_TRUE(PointInsidePolygon(Point2D{0.5, 0.3}, square));
    EXPECT_TRUE(PointInsidePolygon(Point2D{-0.5, -0.3}, square));
    EXPECT_TRUE(PointInsidePolygon(Point2D{-0.5, 0.3}, square));
    EXPECT_TRUE(PointInsidePolygon(Point2D{0.5, -0.3}, square));

    EXPECT_FALSE(PointInsidePolygon(Point2D{2, 0}, square));
    EXPECT_FALSE(PointInsidePolygon(Point2D{2.5, 0.3}, square));

    EXPECT_FALSE(PointInsidePolygon(Point2D{-0.5, -2.3}, square));
    EXPECT_FALSE(PointInsidePolygon(Point2D{-0.5, 1.3}, square));
    EXPECT_FALSE(PointInsidePolygon(Point2D{5.5, -0.3}, square));
}
