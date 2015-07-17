#pragma once

#include <algorithm>

namespace algorithms {

template<typename T>
T Gcd(T a, T b) {
    using std::swap;
    // inv: a >= b;
    if (a < b) {
        swap(a, b);
    }

    if (b == 0) {
        return 0;
    }

    while (b != 0) {
        a = a % b;
        swap(a, b);
    }

    return a;
}

} // namespace algorithms
