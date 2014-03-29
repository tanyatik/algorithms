#include <iostream>

#include <gtest/gtest.h>

#include "../string.hpp"

namespace tanyatik {

TEST(cow_string_t_test, simple_test) {
    cow_string_t a("abc");
    cow_string_t b(a);

    b = "123";

    EXPECT_STREQ("abc", a.get());
    EXPECT_STREQ("123", b.get());

    a = "aaa";
    EXPECT_STREQ("aaa", a.get());
    EXPECT_STREQ("123", b.get());
}

void foo(cow_string_t *c) {
    cow_string_t a = "123";
    *c = a;
}

TEST(cow_string_t_test, pointer_init_test) {
    cow_string_t a;
    foo(&a);
    EXPECT_STREQ("123", a.get());
}

TEST(cow_string_t_test, strings_test) {
    cow_string_t a("aaa");
    cow_string_t b(a);
    cow_string_t c(a);

    EXPECT_EQ(a.get(), b.get());
    EXPECT_EQ(b.get(), c.get());

    EXPECT_STREQ("aaa", a.get());
    EXPECT_STREQ("aaa", b.get());
    EXPECT_STREQ("aaa", c.get());
}

TEST(cow_string_t_test, several_string_test) {
    cow_string_t a("aaa");
    cow_string_t b("bbb");

    cow_string_t c("ccc");

    b = a;
    a = c;

    EXPECT_STREQ("ccc", a.get());
    EXPECT_STREQ("aaa", b.get());
    EXPECT_STREQ("ccc", c.get());

    a = "12345"; 
    EXPECT_STREQ("12345", a.get());
    EXPECT_STREQ("aaa", b.get());
    EXPECT_STREQ("ccc", c.get());

    c = "!!!"; 
    EXPECT_STREQ("12345", a.get());
    EXPECT_STREQ("aaa", b.get());
    EXPECT_STREQ("!!!", c.get());
}

TEST(cow_string_t_test, outer_block_test) {
    cow_string_t a;
    
    {
        cow_string_t b = "123";
        a = b; 
    }

    EXPECT_STREQ("123", a.get());
}

TEST(cow_string_t_test, get_init_test) {
    cow_string_t a = "123";
    cow_string_t b(a.get());
    EXPECT_STREQ("123", a.get());
    EXPECT_STREQ("123", b.get());
}

TEST(cow_string_t_test, lazy_copy_test) {
    cow_string_t a = "123";
    cow_string_t b(a);

    EXPECT_TRUE(a.get() == b.get()) << "pointers are not equal";

    b = "456";
    EXPECT_STREQ("123", a.get());
    EXPECT_STREQ("456", b.get());
}

TEST(cow_string_t_test, read_index_test) {
    cow_string_t a = "123";
    const char *data = a.get();
    
    EXPECT_EQ('2', static_cast<const cow_string_t &>(a)[1]);
    EXPECT_TRUE(a.get() == data) << "pointers are not equal";
}

TEST(cow_string_t_test, write_index_test) {
    cow_string_t a = "123";
    const char *data = a.get();
    
    a[1] = '4';
    EXPECT_STREQ("143", a.get());
    EXPECT_TRUE(a.get() != data) << "pointers are same";
}

} // namespace tanyatik

