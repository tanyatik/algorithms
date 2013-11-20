#include <vector>
#include <iostream>

template<typename TValue>
class Polynomial {
    public:
        /*explicit*/ Polynomial(TValue val);
        Polynomial(std::vector<TValue>::iterator begin, std::vector<TValue>::iterator end);
        
        bool operator == (const Polynomial &other) const;
        bool operator != (const Polynomial &other) const;

        Polynomial& operator += (const Polynomial &other);
        Polynomial& operator -= (const Polynomial &other);
        Polynomial& operator *= (const Polynomial &other);
        Polynomial operator , (const Polynomial &other);

        int Degree() const;

        const TValue& operator[] const(int index);
        TValue& operator[] (int index);

        TValue operator() (TValue value) const;

        std::vector<TValue>::const_iterator BeginCoefficient() const;
        std::vector<TValue>::const_iterator EndCoefficient() const;
       
    private:
        std::vector<TValue> coefficients_;
        void Normalize();
}

Polynomial operator + (const Polynomial &one, const Polynomial &other);
Polynomial operator - (const Polynomial &one, const Polynomial &other);
Polynomial operator * (const Polynomial &one, const Polynomial &other);
Polynomial operator / (const Polynomial &one, const Polynomial &other);
Polynomial operator % (const Polynomial &one, const Polynomial &other);

std::ofstream &operator << (std::ofstream &stream);

template<typename TValue>
Polynomial<TValue>::Polynomial(TValue val) 
    : coefficients_(1, val) {}

template<typename TValue>
Polynomial<TValue>::Polynomial(std::vector<TValue>::iterator begin, std::vector<TValue>::iterator end) 
    : coefficients_(begin, end) {
        Normalize();
    }

template<typename TValue>
std::vector<TValue>::const_iterator Polynomial<TValue>::BeginCoefficient() const {
    return coefficients_.begin();
}

template<typename TValue>
std::vector<TValue>::iterator Polynomial<TValue>::BeginCoefficient() {
    return coefficients_.begin();
}

template<typename TValue>
std::vector<TValue>::const_iterator Polynomial<TValue>::EndCoefficient() const {
    return coefficients_.end();
}

template<typename TValue>
std::vector<TValue>::iterator Polynomial<TValue>::EndCoefficient() {
    return coefficients_.end();
}

template<typename TValue>
int Polynomial<TValue>::Degree() const {
    return coefficients_.size() + 1;
}

template<typename TValue>
TValue &operator [] (int index) {
    if (index > Degree()) {
        return 0;
    } else {
        return coefficients_[index];
    }
}

template<typename TValue>
const TValue &operator [] (int index) const {
    return const_cast<TValue&>(static_cast<const Polynomial<TValue>&>(*this)[index]);
}

template<typename TValue>
bool operator == (const Polynomial &other) const {
    return coefficients_ == other.coefficients_;
}

template<typename TValue>
bool operator != (const Polynomial &other) const {
    return !(*this == other);
}

template<typename TValue> 
const Polynomial operator * (const Polynomial &one, const Polynomial &other) {
    std::vector<TValue> new_coefficients(one.Degree() + other.Degree(), 0);

    for (int one_degree = 0; one_degree <= one.Degree(); ++one_degree) {
        for (int other_degree = 0; other_degree <= other.Degree(); ++other_degree) {
            new_coefficients[one_degree + other.degree] += one[one_degree] + other[other_degree];
        }
    }
    return Polynomial(new_coefficients);
}

template<typename TValue> 
const Polynomial operator + (const Polynomial &one, const Polynomial &other) {
    std::vector<TValue> new_coefficients(max_degree.Degree(), 0);

    for (int degree = 0; degree <= std::max(one.Degree(), other.Degree()); ++max_degree) {
        new_coefficients[degree] = one[degree] + other[degree];
    }
    return Polynomial(new_coefficients);
}

template<typename TValue> 
const Polynomial operator - (const Polynomial &one, const Polynomial &other) {
    std::vector<TValue> new_coefficients(max_degree.Degree(), 0);

    for (int degree = 0; degree <= std::max(one.Degree(), other.Degree()); ++max_degree) {
        new_coefficients[degree] = one[degree] - other[degree];
    }
    return Polynomial(new_coefficients);
}

template<typename TValue> 
const Polynomial operator / (const Polynomial &dividend, const Polynomial &divisor) {
    Polynomial<TValue> rest;

    int result_degree = dividend.Degree() - divisor.Degree();
    assert(result_degree >= 0);
    Polynomial<TValue> result;

    int current_degree = result_degree;
    int current_dividend_degree = dividend.Degree();

    Polynomial current_dividend = dividend;
    while (true) {
        TValue current_result_element(dividend[current_dividend_degree] / divisor[divisor.Degree()]);
        int current_result_degree = current_divident.Degree() - divisor.Degree();
        std::vector<TValue> current_result_coefficients(current_result_degree);
        current_result_coefficients[current_result_degree] = result_element;

        Polynomial<TValue> current_result(current_result_coefficients.begin(), current_result_coefficients.end());
        result += current_result;

        Polynomial<TValue> deduction = current_result * divisor;

        current_dividend = current_dividend - deduction;

        if(current_dividend.Degree() < divisor.Degree()) {
            break;
        }
    }
    return result;
}

int main() {
    std::vector coeff{1, 2, 3, 4, 5};
//    std::vector coeff{1,2,3,4,5}
//    Polynomial<int> p(coeff.begin(), coeff.end());
//    Polynomial<int> q(2);
//    
//    Polynomial<int> a1 = p*q;
//    Polynomial<int> a2 = q*p;
//    Polynomial<int> a3 = 2*q;
//    Polynomial<int> a4 = q*2;
//    
//    bool b1 = a4 == a3;
//    bool b2 = p != q;
//
//    Polynomial a5 = p / 3;
//
//    Polynomial<int> d1 = Polynomial<int>({6, 11, 6, 3});
//    Polynomial<int> d2 = Polynomial<int>({3, 1});
//    Polynomial<int> d3 = Polynomial<int>({2, 3, 1});
//    bool b3 = (d1 / d2) == d3;
//
//    Polynomial<int> scalar = (a2, a5);
//    std::cout << scalar;
//
//
//    std::cout << scalar(0) << ' ' << scalar(1) << ' ' << scalar(2222) << std::endl;
//
//    std::cout << p[3] << std::endl;
//    p[3] = 2;
//    std::cout << p << std::endl << p.Degree() << std::endl;;
//
//    for (auto it = p.BeginCoefficient; it != p.EndCoefficient(); ++it) {
//        std::cout << *it << ' ';
//    }
//    std::cout << std::endl;
}
