#include <gtest/gtest.h>

#include "../smart_pointer.hpp"

class SimpleClass {
private:
    int value_;
public:
    SimpleClass(int value) :
        value_(value) {
            std::cout << "SimpleClass ctor" << std::endl;
        }

    ~SimpleClass() {
        std::cout << "SimpleClass dtor" << std::endl;
    }
};

TEST(smart_pointer, default_smart_pointer) {
    using tanyatik::SmartPointer;
    
    SmartPointer<int> p(new int(5));
    SmartPointer<int> q(p);

    SmartPointer<SimpleClass> s1(new SimpleClass(5));
    SmartPointer<SimpleClass> s2(s1);
    SmartPointer<SimpleClass> s3 = s2;
}
