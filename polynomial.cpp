#include <vector>
#include <iostream>
#include <complex>
#include <assert.h>

template<typename TValue>
class Polynomial;

template<typename TValue>
const Polynomial<TValue> doSum(const Polynomial<TValue>& one, const Polynomial<TValue>& other);
template<typename TValue>
const Polynomial<TValue> doSubtract(const Polynomial<TValue>& one, const Polynomial<TValue>& other);
template<typename TValue>
const Polynomial<TValue> doMultiply(const Polynomial<TValue>& one, const Polynomial<TValue>& other);
template<typename TValue>
const Polynomial<TValue> doDivide(const Polynomial<TValue>& one, const Polynomial<TValue>& other);
template<typename TValue>
const Polynomial<TValue> doModulus(const Polynomial<TValue>& one, const Polynomial<TValue>& other);
template<typename TValue>
const Polynomial<TValue> doGreatestCommonDivisor(const Polynomial<TValue>& one, 
                                                 const Polynomial<TValue>& other);

// Class representing a polynomial where coefficients are of type TValue
// Assuming TValue has operators +, -, *, /
// Assuming TValue has default ctor which value is taken as null
// Assuming TValue has operator << for ofstream overloaded
template<typename TValue>
class Polynomial {
    public:
        /*explicit*/ Polynomial(TValue val);
        Polynomial(typename std::vector<TValue>::iterator begin_iter, 
                   typename std::vector<TValue>::iterator end_iter);
        
        int maxDegree() const;
        int degree() const;
        bool isNull() const;

        // Count value of the polynomial in specified point
        const TValue operator() (TValue point) const;
        // Get specified coefficient of polynomial
        const TValue &operator[] (int index) const;
        // Get with possibility of change of polynomial
        TValue &operator[] (int index);

        typename std::vector<TValue>::const_iterator beginCoefficient() const;
        typename std::vector<TValue>::const_iterator endCoefficient() const;
       
        friend Polynomial &operator += (Polynomial &one, const Polynomial &other) {
            one = doSum(one, other);
            return one;
        }
        friend Polynomial &operator -= (Polynomial &one, const Polynomial &other) {
            one = doSubtract(one, other);
            return one;
        }
        friend Polynomial &operator *= (Polynomial &one, const Polynomial &other) {
            one = doMultiply(one, other);
            return one;
        }
        friend Polynomial &operator /= (Polynomial &one, const Polynomial &other) {
            one = doDivide(one, other);
            return one;
        }
        friend Polynomial &operator %= (Polynomial &one, const Polynomial &other) {
            one = doModulus(one, other);
            return one;
        }

        friend const Polynomial operator + (const Polynomial &one, const Polynomial &other) {
            return doSum(one, other);
        }
        friend const Polynomial operator - (const Polynomial &one, const Polynomial &other) {
            return doSubtract(one, other);
        }
        friend const Polynomial operator * (const Polynomial &one, const Polynomial &other) {
            return doMultiply(one, other);
        }
        friend const Polynomial operator / (const Polynomial &one, const Polynomial &other) {
            return doDivide(one, other);
        }
        friend const Polynomial operator % (const Polynomial &one, const Polynomial &other) {
            return doModulus(one, other);
        }

        // Get greatest common divisor of two arguments
        friend const Polynomial operator , (const Polynomial &one, const Polynomial &other) {
            return doGreatestCommonDivisor(one, other);
        }
        friend bool operator == (const Polynomial &one, const Polynomial &other) {
            return one.equals(other);
        }
        friend bool operator != (const Polynomial &one, const Polynomial &other) {
            return !one.equals(other);
        }

    private:
        std::vector<TValue> coefficients_;
        void removeLeadingZeros();
        bool equals(const Polynomial &other) const;
};

template<typename TValue>
std::ofstream &operator << (std::ofstream &stream, const Polynomial<TValue> &p);

template<typename TValue>
Polynomial<TValue>::Polynomial(TValue val) 
    : coefficients_(1, val) {}

template<typename TValue>
Polynomial<TValue>::Polynomial(typename std::vector<TValue>::iterator begin, 
                               typename std::vector<TValue>::iterator end) 
    : coefficients_(begin, end) {
    removeLeadingZeros();
}

template<typename TValue>
bool Polynomial<TValue>::isNull() const {
    return ((coefficients_.size() == 1) && (coefficients_[0] == (TValue())));
}

template<typename TValue>
void Polynomial<TValue>::removeLeadingZeros() {
    int index = coefficients_.size() - 1;
    while (index > 0 && coefficients_[index] == (TValue())) {
        --index;
    }
    ++index;
    coefficients_.erase(coefficients_.begin() + index, coefficients_.end());
}

template<typename TValue>
typename std::vector<TValue>::const_iterator Polynomial<TValue>::beginCoefficient() const {
    return coefficients_.begin();
}

template<typename TValue>
typename std::vector<TValue>::const_iterator Polynomial<TValue>::endCoefficient() const {
    return coefficients_.end();
}

template<typename TValue>
int Polynomial<TValue>::maxDegree() const {
    return coefficients_.size() - 1;
}

template<typename TValue>
int Polynomial<TValue>::degree() const {
    if (isNull()) {
        return -1;
    }
    return coefficients_.size() - 1;
}

template<typename TValue>
const TValue &Polynomial<TValue>::operator[] (int index) const {
    assert(index <= maxDegree());
    return coefficients_[index];
}

template<typename TValue>
TValue &Polynomial<TValue>::operator[] (int index) {
    if (index > maxDegree()) {
        coefficients_.resize(index + 1);
    }
    return coefficients_[index];
    // there will be a problem if user sets coefficient[maxDegree()] to null...
}

template<typename TValue>
const TValue Polynomial<TValue>::operator() (TValue value) const {
    TValue answer = coefficients_[maxDegree()];
    for (int i = maxDegree() - 1; i >= 0; --i) {
        answer = answer * value + coefficients_[i];
    }
    return answer;
}

template<typename TValue>
bool Polynomial<TValue>::equals (const Polynomial<TValue> &other) const {
    return coefficients_ == other.coefficients_;
}

template<typename TValue> 
const Polynomial<TValue> doMultiply (const Polynomial<TValue> &one, const Polynomial<TValue> &other) {
    std::vector<TValue> new_coefficients(one.maxDegree() + other.maxDegree() + 1, (TValue()));

    for (int one_degree = 0; one_degree <= one.maxDegree(); ++one_degree) {
        for (int other_degree = 0; other_degree <= other.maxDegree(); ++other_degree) {
            new_coefficients[one_degree + other_degree] += one[one_degree] * other[other_degree];
        }
    }
    return Polynomial<TValue>(new_coefficients.begin(), new_coefficients.end());
}

template<typename TValue> 
const Polynomial<TValue> doSum (const Polynomial<TValue> &one, const Polynomial<TValue> &other) {
    const Polynomial<TValue> &ref_max = (one.maxDegree() < other.maxDegree() ? other : one);
    const Polynomial<TValue> &ref_min = (one.maxDegree() < other.maxDegree() ? one : other);

    std::vector<TValue> new_coefficients(ref_max.maxDegree() + 1);
    int degree = 0;
    for (; degree <= ref_min.maxDegree(); ++degree) {
        new_coefficients[degree] = ref_min[degree] + ref_max[degree];
    }
    for (; degree <= ref_max.maxDegree(); ++degree) {
        new_coefficients[degree] = ref_max[degree]; 
    }
    return Polynomial<TValue>(new_coefficients.begin(), new_coefficients.end());
}

template<typename TValue> 
const Polynomial<TValue> doSubtract (const Polynomial<TValue> &one, const Polynomial<TValue> &other) {
    std::vector<TValue> new_coefficients(std::max(one.maxDegree(), other.maxDegree()) + 1);
    for (int degree = 0; degree <= one.maxDegree(); ++degree) {
        new_coefficients[degree] = one[degree];
    }
    for (int degree = 0; degree <= other.maxDegree(); ++degree) {
        new_coefficients[degree] -= other[degree]; 
    }
    return Polynomial<TValue>(new_coefficients.begin(), new_coefficients.end());
}

template<typename TValue>
void divideByModulus(const Polynomial<TValue> &dividend, const Polynomial<TValue> &divisor, Polynomial<TValue> *result_quotient, Polynomial<TValue> *result_rest) {
    int quotient_degree = dividend.maxDegree() - divisor.maxDegree();
    assert(quotient_degree >= 0);
    Polynomial<TValue> quotient((TValue()));

    Polynomial<TValue> current_dividend = dividend;
    while (true) {
        int current_quotient_degree = current_dividend.maxDegree() - divisor.maxDegree();
        assert(current_quotient_degree >= 0);
        std::vector<TValue> temp_coef(current_quotient_degree + 1);
        temp_coef[current_quotient_degree] 
            = current_dividend[current_dividend.maxDegree()] / divisor[divisor.maxDegree()];

        Polynomial<TValue> current_quotient(temp_coef.begin(), temp_coef.end());
        quotient = quotient + current_quotient;

        Polynomial<TValue> deduction = current_quotient * divisor;
        current_dividend = current_dividend - deduction;

        if (current_dividend.maxDegree() < divisor.maxDegree() || current_quotient.isNull()) {
            break;
        }
    }

    *result_quotient = quotient;
    *result_rest = current_dividend;
}

template<typename TValue> 
const Polynomial<TValue> doDivide (const Polynomial<TValue> &dividend, 
                                   const Polynomial<TValue> &divisor) {
    Polynomial<TValue> quotient((TValue()));
    Polynomial<TValue> rest((TValue()));

    divideByModulus(dividend, divisor, &quotient, &rest);
    return quotient;
}

template<typename TValue> 
const Polynomial<TValue> doModulus (const Polynomial<TValue> &dividend, 
                                    const Polynomial<TValue> &divisor) {
    Polynomial<TValue> quotient((TValue()));
    Polynomial<TValue> rest((TValue()));

    divideByModulus(dividend, divisor, &quotient, &rest);
    return rest;
}

template<typename TValue> 
const Polynomial<TValue> doGreatestCommonDivisor (const Polynomial<TValue> &one, 
                                                  const Polynomial<TValue> &other) {
    const Polynomial<TValue> &ref_max = (one.maxDegree() < other.maxDegree() ? other : one);
    const Polynomial<TValue> &ref_min = (one.maxDegree() < other.maxDegree() ? one : other);

    if (ref_min.maxDegree() == 0) {
        return ref_max;
    }
    if (ref_max.maxDegree() == 0) {
        return ref_min;
    }
    return doGreatestCommonDivisor(ref_min, ref_max % ref_min);
}

template<typename TValue>
std::ostream &operator << (std::ostream &stream, const Polynomial<TValue> &p) {
    for (int i = p.maxDegree(); i >= 0; --i) {
        if (p[i] != (TValue()) || (p.maxDegree() == 0 && i == 0)) {
            if (i < p.maxDegree()) {
                stream << " + ";
            }
            stream << p[i];
            if (i > 0) {
                stream << "*x^" << i;
            }
        }
    }
    return stream;
}

template<typename TValue>
void checkArithmetics(Polynomial<TValue> first, 
                      Polynomial<TValue> second,
                      TValue scalar) {
    std::cout << "first : " << first << std::endl;
    std::cout << "second : " << second << std::endl;
    std::cout << "(first != second) is " << (first != second ? "true" : "false") << std::endl;

    Polynomial<TValue> sum = first + second;
    std::cout << "(" << first << ") + (" << second << ") = " << sum << std::endl;
    Polynomial<TValue> difference = first - second;
    std::cout << "(" << first << ") - (" << second << ") = " << difference << std::endl;
    Polynomial<TValue> factor = first * second;
    std::cout << "(" << first << ") * (" << second << ") = " << factor << std::endl;
    Polynomial<TValue> quotient = first / second;
    std::cout << "(" << first << ") / (" << second << ") = " << quotient << std::endl;
    Polynomial<TValue> rest = first % second;
    std::cout << "(" << first << ") % (" << second << ") = " << rest << std::endl;
 
    bool identity = (first == quotient * second + rest);
    std::cout << '(' << quotient << ") * (" << second << ") + " << rest 
        << " == " << first << " is "  << (identity ? "true" : "false") << std::endl;

    first += second;
    std::cout << "first += second = " << first << std::endl;
    first -= second;
    std::cout << "first -= second = " << first << std::endl;
    first *= second;
    std::cout << "first *= second = " << first << std::endl;
    first /= second;
    std::cout << "first /= second = " << first << std::endl;
    first %= second;
    std::cout << "first %= second = " << first << std::endl;
    std::cout << "first now : " << first << std::endl;

    std::cout << "scalar types : " << std::endl;
    factor = scalar * first;
    std::cout << scalar << " * first = " << factor << std::endl;
    factor = first * scalar;
    std::cout << "first * " << scalar << " = " << factor << std::endl;
    first += scalar;
    std::cout << "first += " << scalar << " = " << first << std::endl; 
    identity = (scalar == first);
    std::cout << scalar << " == first is " << (identity ? "true" : "false") << std::endl;
    identity = (first == scalar);
    std::cout << "first == " << scalar << " is " << (identity ? "true" : "false") << std::endl;
}

int main() {
    std::vector<int> vec1 = {6, 11, 6, 1, 0, 0, 0};
    std::vector<int> vec2 = {1, 3, 1, 0};

    Polynomial<int> polly1 = Polynomial<int>(vec1.begin(), vec1.end());
    Polynomial<int> polly2 = Polynomial<int>(vec2.begin(), vec2.end());

    checkArithmetics(polly1, polly2, 3);
    std::cout << std::endl;

    std::vector<int> vec3 = {1, 0, 1, 0, 1};
    Polynomial<int> polly3 = Polynomial<int>(vec3.begin(), vec3.end());
    checkArithmetics<int>(polly3, polly1, -1);
    std::cout << std::endl;

    std::vector<double> vec_double_1 = {-1.89, -3.45, -9.00};
    std::vector<double> vec_double_2 = {1.5, 3.2};

    Polynomial<double> polly4 = Polynomial<double>(vec_double_1.begin(), vec_double_1.end());
    Polynomial<double> polly5 = Polynomial<double>(vec_double_2.begin(), vec_double_2.end());
    checkArithmetics(polly4, polly5, 3.1415);
    std::cout << std::endl;

    std::vector<int> small_vec1 = {1, 3};
    std::vector<int> small_vec2 = {0, 0, 3};

    Polynomial<int> polly6 = polly1 * Polynomial<int>(small_vec1.begin(), small_vec1.end());
    Polynomial<int> polly7 = polly1 * Polynomial<int>(small_vec2.begin(), small_vec2.end());

    Polynomial<int> gcd = (polly6 , polly7); // greatest common divisor
    std::cout << "gcd (" << polly6 << ", " << polly7 << ") = " << gcd << std::endl;

    for (std::vector<int>::const_iterator it = gcd.beginCoefficient(); it != gcd.endCoefficient(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
    
    std::cout << "(" << polly7 << ")[7] = 7 : ";
    polly7[7] = 7;
    std::cout << polly7 << std::endl;
    std::cout << "degree : " << polly7.degree() << std::endl;


    std::cout << polly2 << " at x = 2: " << polly2(2) << std::endl;
    std::cout << polly1 << " at x = -3: " << polly1(-3) << std::endl;
    std::cout << polly4 << " at x = -0.75: " << polly4(-0.75) << std::endl;

    std::vector<std::complex<double>> complex_vec = {{0, 1}, {1, 0}, {-1, -1}};
    Polynomial<std::complex<double>> complex_polly(complex_vec.begin(), complex_vec.end());
    std::cout << "complex polly : " << complex_polly << std::endl;
    std::cout << complex_polly << " at x = i : " << complex_polly(std::complex<double>(0, 1)) << std::endl;
}
