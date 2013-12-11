#include <gtest/gtest.h>

#include "../smart_pointer.hpp"

class SimpleClass {
private:
    int value_;
    int *counter_;

public:
    SimpleClass(int value, int *counter) :
        value_(value),
        counter_(counter) {
            ++*counter_;
        }

    int getValue() const {
        return value_;
    }
    void setValue(int value) {
        value_ = value;
    }

    ~SimpleClass() {
        --*counter_;
    }
};

using tanyatik::SmartPointer;

TEST(smart_pointer, copy_assign) {
   
    int counter = 0;
    {
        SmartPointer<SimpleClass> s1(new SimpleClass(1, &counter));
        ASSERT_EQ(1, s1->getValue());
        SmartPointer<SimpleClass> s2(s1);
        ASSERT_EQ(1, s2->getValue());
        SmartPointer<SimpleClass> s3(new SimpleClass(3, &counter));
        ASSERT_EQ(3, s3->getValue());
        SmartPointer<SimpleClass> s4(new SimpleClass(4, &counter));
        ASSERT_EQ(4, s4->getValue());
        s1 = s4;
        ASSERT_EQ(4, s1->getValue());
        ASSERT_EQ(4, s4->getValue());
        s2 = s3;
        ASSERT_EQ(3, s2->getValue());
        ASSERT_EQ(3, s3->getValue());
        s4 = s3;
        ASSERT_EQ(3, s3->getValue());
        ASSERT_EQ(3, s4->getValue());
        ASSERT_EQ(4, s1->getValue());
        ASSERT_EQ(3, s2->getValue());
    }
    ASSERT_EQ(0, counter);
}

TEST(smart_pointer, self_assign)
{
    int counter = 0;
    {
        SmartPointer<SimpleClass> s1(new SimpleClass(1, &counter));
        ASSERT_EQ(1, s1->getValue());
        //s1 = s1;
    }
    ASSERT_EQ(0, counter);
}

TEST(smart_pointer, get)
{
    int counter = 0;
    {
        SmartPointer<SimpleClass> s1(new SimpleClass(100, &counter));
        SimpleClass *pointer = get(s1);
        pointer->setValue(3);
        ASSERT_EQ(3, s1->getValue());
    }
    ASSERT_EQ(0, counter);
    //delete s1;
}

TEST(smart_pointer, array_pointer)
{
    using tanyatik::ArrayStorage;
    typedef SmartPointer<int, ArrayStorage> ArrayPointer; 
    ArrayPointer arr1(new int[1024]);
    ArrayPointer arr2 = arr1;
    ArrayPointer arr3(new int[2048]);
    arr3 = arr1;
    arr2 = arr1;

}
