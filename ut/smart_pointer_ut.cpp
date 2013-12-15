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
        SmartPointer<SimpleClass> s_first(new SimpleClass(1, &counter));
        ASSERT_EQ(1, s_first->getValue());
        SmartPointer<SimpleClass> s_second(s_first);
        ASSERT_EQ(1, s_second->getValue());
        SmartPointer<SimpleClass> s_third(s_first);
        ASSERT_EQ(1, s_third->getValue());
        SmartPointer<SimpleClass> s4(s_first);
        ASSERT_EQ(1, s4->getValue());
        //delete s_first;
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
        SmartPointer<SimpleClass> s_first (new SimpleClass(1, &counter));
        ASSERT_EQ(1, s_first->getValue());

        SmartPointer<SimpleClass> s_second(s_first);
        ASSERT_TRUE(s_first == s_second);

        SmartPointer<SimpleClass> s_third = s_second;
        ASSERT_TRUE(s_first == s_third);
        ASSERT_TRUE(s_second == s_third);
        ASSERT_TRUE(s_first == s_second);

        SimpleClass *raw_first = get(s_first);
        ASSERT_TRUE(s_first == raw_first);
        ASSERT_TRUE(raw_first == s_first);

        SimpleClass *raw_second = get(s_second);
        ASSERT_TRUE(s_first == raw_second);
        ASSERT_TRUE(raw_second == s_first);

        ASSERT_TRUE(s_first);
        ASSERT_TRUE(s_second);

        s_first = s_first;
        ASSERT_TRUE(s_first);
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
        SimpleDerived *raw_derived = get(s_derived);

        ASSERT_TRUE(s_base != s_derived);
        ASSERT_TRUE(s_derived != s_base);
        ASSERT_FALSE(s_base == s_derived); 
        ASSERT_FALSE(s_derived == s_base); 
        ASSERT_TRUE(raw_derived == s_derived);
        ASSERT_FALSE(raw_derived == s_base);

        SimpleClass *raw_base = get(s_base);
        ASSERT_FALSE(s_derived == raw_base); 

        ASSERT_FALSE(raw_derived < s_derived);
    }
    ASSERT_EQ(0, counter);
}

TEST(smart_pointer, get)
{
    int counter = 0;
    {
        SmartPointer<SimpleClass> s_first(new SimpleClass(100, &counter));
        SimpleClass *pointer = get(s_first);
        pointer->setValue(3);
        ASSERT_EQ(3, s_first->getValue());
        ASSERT_EQ(3, get(s_first)->getValue());
    }
    ASSERT_EQ(0, counter);
}

TEST(smart_pointer, array_pointer)
{
    using tanyatik::ArrayStorage;
    typedef SmartPointer<int, ArrayStorage> ArrayPointer; 
    ArrayPointer arr_first(new int[1024]);
    ArrayPointer arr_second = arr_first;
    ArrayPointer arr_third(new int[2048]);
    arr_third = arr_first;
    arr_second = arr_first;
}

TEST(smart_pointer, weak_pointer)
{
    using tanyatik::WeakPointer;
    
    int counter = 0;
    {
        WeakPointer<SimpleClass> w_first;
        {
            SmartPointer<SimpleClass> s_first(new SimpleClass(1, &counter));
            w_first = s_first;
        }
        ASSERT_TRUE(w_first.expired());
        SmartPointer<SimpleClass> s_second = w_first.lock();
        ASSERT_TRUE(s_second == 0);

        SmartPointer<SimpleClass> s_third(new SimpleClass(2, &counter));
        WeakPointer<SimpleClass> w_second;
        SmartPointer<SimpleClass> s_fourth = w_second.lock();
    }
    ASSERT_EQ(0, counter);
}
