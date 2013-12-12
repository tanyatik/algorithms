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

struct SimpleDerived : public SimpleClass {
    SimpleDerived(int *counter) :
        SimpleClass(0, counter) {}
};

using tanyatik::SmartPointer;

TEST(smart_pointer, copy_assign) {
    int counter = 0;
    {
        SmartPointer<SimpleClass> s1(new SimpleClass(1, &counter));
        ASSERT_EQ(1, s1->getValue());
        SmartPointer<SimpleClass> s2(s1);
        ASSERT_EQ(1, s2->getValue());
        SmartPointer<SimpleClass> s3(s1);
        ASSERT_EQ(1, s3->getValue());
        SmartPointer<SimpleClass> s4(s1);
        ASSERT_EQ(1, s4->getValue());
        //delete s1;
    }
    ASSERT_EQ(0, counter);
}

TEST(smart_pointer, assign) {
    int counter = 0;
    {
        SmartPointer<SimpleClass> s_first(new SimpleClass(1, &counter));
        ASSERT_EQ(1, s_first->getValue());
        SmartPointer<SimpleClass> s_second(new SimpleClass(3, &counter));
        ASSERT_EQ(3, s_second->getValue());
        SmartPointer<SimpleClass> s_third(new SimpleClass(4, &counter));
        ASSERT_EQ(4, s_third->getValue());
        s_first = s_third;
        ASSERT_EQ(4, s_first->getValue());
        ASSERT_EQ(4, s_third->getValue());
        ASSERT_EQ(3, s_second->getValue());
        s_third = s_second;
        ASSERT_EQ(3, s_second->getValue());
        ASSERT_EQ(3, s_third->getValue());
        ASSERT_EQ(4, s_first->getValue());
    }
}


TEST(smart_pointer, comparisons)
{
    int counter = 0;
    {
        SmartPointer<SimpleClass> s1 (new SimpleClass(1, &counter));
        ASSERT_EQ(1, s1->getValue());

        SmartPointer<SimpleClass> s2(s1);
        ASSERT_TRUE(s1 == s2);

        SmartPointer<SimpleClass> s3 = s2;
        ASSERT_TRUE(s1 == s3);
        ASSERT_TRUE(s2 == s3);
        ASSERT_TRUE(s1 == s2);

        SimpleClass *raw_first = get(s1);
        ASSERT_TRUE(s1 == raw_first);
        ASSERT_TRUE(raw_first == s1);

        SimpleClass *raw_second = get(s2);
        ASSERT_TRUE(s1 == raw_second);
        ASSERT_TRUE(raw_second == s1);

        ASSERT_TRUE(s1);
        ASSERT_TRUE(s2);

        //s1 = s1;
    }
    ASSERT_EQ(0, counter);
}

TEST(smart_pointer, null_pointer) 
{
    int counter = 0;
    {
        SmartPointer<SimpleClass> n(nullptr);
        ASSERT_FALSE(n);
        ASSERT_TRUE(n == nullptr);
        ASSERT_TRUE(nullptr == n);
        ASSERT_TRUE(0 == n);
        ASSERT_TRUE(n == 0);
        ASSERT_TRUE(!n);

        SmartPointer<SimpleClass> s_base(new SimpleClass(10, &counter));
        ASSERT_FALSE(s_base == n);
        ASSERT_FALSE(n == s_base);
        ASSERT_FALSE(0 == s_base);
        ASSERT_FALSE(s_base == 0);
        ASSERT_TRUE(s_base);

        SmartPointer<SimpleDerived> s_derived(new SimpleDerived(&counter));
        SimpleDerived *raw_derived_null = 0;
        ASSERT_TRUE(n == raw_derived_null);
        ASSERT_TRUE(raw_derived_null != s_derived);

        ASSERT_TRUE(s_base != s_derived);
        ASSERT_TRUE(s_derived != s_base);
        ASSERT_FALSE(s_base == s_derived); 
        ASSERT_FALSE(s_derived == s_base); 
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
        ASSERT_EQ(3, get(s1)->getValue());
    }
    ASSERT_EQ(0, counter);
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
