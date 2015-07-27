#include <iostream>
#include <vector>
#include <complex>
#include <math.h>
#include <assert.h>

namespace algorithms {
namespace internal {

// internal function
void DoDiscreteFourierTransformation(bool inverse, std::vector<std::complex<double>> *coefficients) {
    int dimension = coefficients->size();

    if (dimension == 1) {
        return;
    }

    std::vector<std::complex<double>> even(dimension / 2);
    std::vector<std::complex<double>> odd(dimension / 2);

    for (int i = 0; i < dimension / 2; ++i) {
        even[i] = (*coefficients)[2 * i];
        odd[i] = (*coefficients)[2 * i + 1];
    }

    DoDiscreteFourierTransformation(inverse, &even);
    DoDiscreteFourierTransformation(inverse, &odd);

    double phi = 2.0 * M_PI / dimension;
    if (inverse) {
        phi *= -1;
    }

    std::complex<double> eps(1);
    std::complex<double> diff(cos(phi), sin(phi));

    for (int i = 0; i < dimension / 2; ++i) {
        (*coefficients)[i]                 = even[i] + eps * odd[i];
        (*coefficients)[i + dimension / 2] = even[i] - eps * odd[i];

        eps *= diff;
    }
}


size_t ResizeForFFT(std::vector<std::complex<double>> * const coefficients) {
    size_t n = 1;
    while (n < coefficients->size()) {
        n <<= 1;
    }
    n <<= 1;
    coefficients->resize(n);
    return n;
}

bool IsPowerOf2(size_t size) {
    return (size & (size - 1)) == 0;
}


} // namespace internal

// api functions
void DiscreteFourierTransformation(std::vector<std::complex<double>> * const coefficients) {
    // internal::ResizeForFFT(coefficients);
    assert(internal::IsPowerOf2(coefficients->size()));
    return internal::DoDiscreteFourierTransformation(false, coefficients);
}

void InverseDiscreteFourierTransformation(std::vector<std::complex<double>> * const coefficients) {
    size_t n = coefficients->size();
    assert(internal::IsPowerOf2(n));
    // size_t n = internal::ResizeForFFT(coefficients);
    internal::DoDiscreteFourierTransformation(true, coefficients);

    for (auto& c : *coefficients) {
        c /= coefficients->size();
    }
}

std::vector<int> PolynomialMultiply(const std::vector<int>& one, const std::vector<int>& two) {
    std::vector<std::complex<double>> one_c(one.begin(), one.end()), two_c(two.begin(), two.end());

    internal::ResizeForFFT(&one_c);
    internal::ResizeForFFT(&two_c);
    size_t size = one_c.size();
    if (one_c.size() < two_c.size()) {
        one_c.resize(two_c.size());
        size = two_c.size();
    } else if (one_c.size() > two_c.size()) {
        two_c.resize(one_c.size());
    }

    DiscreteFourierTransformation(&one_c);
    DiscreteFourierTransformation(&two_c);

    for (size_t i = 0; i < size; ++i) {
        one_c[i] *= two_c[i];
    }

    InverseDiscreteFourierTransformation(&one_c);

    std::vector<int> result(size);
    for (size_t i = 0; i < size; ++i) {
        result[i] = std::lround(one_c[i].real());
    }

    return result;
}

} // namespace algorithms
