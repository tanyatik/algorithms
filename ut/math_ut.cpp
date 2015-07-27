#include <gtest/gtest.h>

#include "math/geometry.hpp"
#include "math/fft.hpp"

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

TEST(geometry, point_inside_square) {
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

TEST(geometry, point_inside_polygon) {
    // square
    std::vector<Point2D> polygon;
    polygon.push_back(Point2D{ -2, 1 });
    polygon.push_back(Point2D{ 0, 3 });
    polygon.push_back(Point2D{ 3, 3 });
    polygon.push_back(Point2D{ 4, 1 });
    polygon.push_back(Point2D{ 3, -2 });
    polygon.push_back(Point2D{ 2, -2 });

    EXPECT_TRUE(PointInsidePolygon(Point2D{0, 1}, polygon));
    EXPECT_TRUE(PointInsidePolygon(Point2D{2, 2}, polygon));
    EXPECT_TRUE(PointInsidePolygon(Point2D{3, 1}, polygon));
    EXPECT_TRUE(PointInsidePolygon(Point2D{1, 0}, polygon));
}

TEST(geometry, wedge_product) {
    EXPECT_FALSE(Clockwise(Point2D{-1, -3}, {-1, -2}, {-1, -1}));
    EXPECT_FALSE(CounterClockwise(Point2D{-1, -3}, {-1, -2}, {-1, -1}));
    EXPECT_FALSE(Clockwise(Point2D{-1, -3}, {-1, -2}, {-1, 0}));
    EXPECT_FALSE(CounterClockwise(Point2D{-1, -3}, {-1, -2}, {-1, 0}));
}


TEST(gcd, gcd) {
    EXPECT_EQ(gcd(1, 1), 1);
    EXPECT_EQ(gcd(2, 1), 1);
    EXPECT_EQ(gcd(1, 2), 1);
    EXPECT_EQ(gcd(2, 3), 1);
    EXPECT_EQ(gcd(3, 5), 1);
    EXPECT_EQ(gcd(2, 4), 2);
    EXPECT_EQ(gcd(12, 4), 4);
    EXPECT_EQ(gcd(12, 15), 3);
    EXPECT_EQ(gcd(6, 10), 2);
    EXPECT_EQ(gcd(15, 10), 5);
}
TEST(polynomial_multiply, fft_polynomial_multiply_eq_sizes) {
    std::vector<int> vec1({int(1), int(2), int(3)}); // polynomial coefficients
    std::vector<int> vec2({int(3), int(2), int(1)}); // polynomial coefficients

    auto res = algorithms::PolynomialMultiply(vec1, vec2);

    ASSERT_EQ(3, res[0]);
    ASSERT_EQ(8, res[1]);
    ASSERT_EQ(14, res[2]);
    ASSERT_EQ(8, res[3]);
    ASSERT_EQ(3, res[4]);
    ASSERT_EQ(0, res[5]);
    ASSERT_EQ(0, res[6]);
    ASSERT_EQ(0, res[7]);
}


TEST(polynomial_multiply, fft_polynomial_multiply_neq_sizes) {
    std::vector<int> vec1({1, 1, 2}); // polynomial coefficients
    std::vector<int> vec2({1, 2}); // polynomial coefficients

    auto res = algorithms::PolynomialMultiply(vec1, vec2);

    ASSERT_EQ(1, res[0]);
    ASSERT_EQ(3, res[1]);
    ASSERT_EQ(4, res[2]);
    ASSERT_EQ(4, res[3]);
}


TEST(polynomial_multiply, fft_polynomial_multiply_zeros) {
    std::vector<int> vec1({0, 1, 2}); // polynomial coefficients
    std::vector<int> vec2({0, 2}); // polynomial coefficients

    auto res = algorithms::PolynomialMultiply(vec1, vec2);

    ASSERT_EQ(0, res[0]);
    ASSERT_EQ(0, res[1]);
    ASSERT_EQ(2, res[2]);
    ASSERT_EQ(4, res[3]);
}

TEST(polynomial_multiply, fft_polynomial_multiply_neq_sizes_zeros) {
    std::vector<int> vec1({0, 1, 1, 0, 3}); // polynomial coefficients
    std::vector<int> vec2({1, 2}); // polynomial coefficients

    auto res = algorithms::PolynomialMultiply(vec1, vec2);

    ASSERT_EQ(0, res[0]);
    ASSERT_EQ(1, res[1]);
    ASSERT_EQ(3, res[2]);
    ASSERT_EQ(2, res[3]);
    ASSERT_EQ(3, res[4]);
    ASSERT_EQ(6, res[5]);
    ASSERT_EQ(0, res[6]);
    ASSERT_EQ(0, res[7]);
}
