#include <math.h>

namespace algorithms {

// Finds M such that
// |f(M) - x| < P, M in [L, R], f is monotonic
// Function parameters:
// monotonic_function: f
// searched: x
// left_point: L
// right_point: R
// precision: P
template<typename F, typename T, typename C = std::less<T>>
double SolveMonotonicEquation(F monotonic_function, 
        T searched_value, 
        double left_point, 
        double right_point, 
        double precision) {
    double middle_point = left_point + (right_point - left_point) / 2;
    while (fabs(right_point - left_point) >= precision) {
        middle_point = left_point + (right_point - left_point) / 2;
        T middle_value = monotonic_function(middle_point);
        if (C()(middle_value, searched_value)) {
            left_point = middle_point;
        } else {
            right_point = middle_point;
        }
    }
    return middle_point;
}

} // namespace algorithms
