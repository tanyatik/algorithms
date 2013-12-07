#include <iostream>
#include <gtest/gtest.h>

#include "../memory_manager.hpp"

TEST(memory_manager, single_allocation) {
    MemoryManager manager(10, 2);
    int result = manager.allocate(5);
    ASSERT_EQ(0, result);
    result = manager.allocate(3);
    ASSERT_EQ(5, result);
}

TEST(memory_manager, allocations) {
    MemoryManager manager(10, 12);
    ASSERT_EQ(0, manager.allocate(2));
    ASSERT_EQ(2, manager.allocate(2));
    ASSERT_EQ(4, manager.allocate(2));
    ASSERT_EQ(6, manager.allocate(2));
    ASSERT_EQ(8, manager.allocate(2));
    manager.deallocate(0);
    manager.deallocate(1);
    manager.deallocate(2);
    manager.deallocate(3);
    manager.deallocate(4);
    ASSERT_EQ(0, manager.allocate(10));
    ASSERT_EQ(-1, manager.allocate(1));
}

TEST(memory_manager, deallocations_turn_occupied_to_free) {
    MemoryManager manager(3, 7);

    ASSERT_EQ(0, manager.allocate(1));
    ASSERT_EQ(1, manager.allocate(1));
    ASSERT_EQ(2, manager.allocate(1));
    ASSERT_EQ(-1, manager.allocate(1));

    manager.deallocate(1);
    ASSERT_EQ(1, manager.allocate(1));
    ASSERT_EQ(-1, manager.allocate(1));
}

TEST(memory_manager, deallocations_merge_two_free_blocks_left) {
    MemoryManager manager(3, 7);

    ASSERT_EQ(0, manager.allocate(1));
    ASSERT_EQ(1, manager.allocate(1));
    ASSERT_EQ(2, manager.allocate(1));

    manager.deallocate(0);
    manager.deallocate(1);
    ASSERT_EQ(0, manager.allocate(2));
}

TEST(memory_manager, deallocations_merge_two_free_blocks_right) {
    MemoryManager manager(3, 7);

    ASSERT_EQ(0, manager.allocate(1));
    ASSERT_EQ(1, manager.allocate(1));
    ASSERT_EQ(2, manager.allocate(1));

    manager.deallocate(2);
    manager.deallocate(1);
    ASSERT_EQ(1, manager.allocate(2));
}

TEST(memory_manager, deallocations_merge_three_free_blocks) {
    MemoryManager manager(3, 7);

    ASSERT_EQ(0, manager.allocate(1));
    ASSERT_EQ(1, manager.allocate(1));
    ASSERT_EQ(2, manager.allocate(1));

    manager.deallocate(0);
    manager.deallocate(2);
    manager.deallocate(1);
    ASSERT_EQ(0, manager.allocate(3));
}

TEST(memory_manager, random_test) {
{
    MemoryManager manager(5, 7);
    ASSERT_EQ(0, manager.allocate(3));
    manager.deallocate(0);
    ASSERT_EQ(0, manager.allocate(3));
    ASSERT_EQ(-1, manager.allocate(3));
    ASSERT_EQ(-1, manager.allocate(3));
    manager.deallocate(2);
    manager.deallocate(3);
}
{
    MemoryManager manager(5, 7);
    ASSERT_EQ(0, manager.allocate(5));
    ASSERT_EQ(-1, manager.allocate(3));
    manager.deallocate(0);
    manager.deallocate(1);
    ASSERT_EQ(0, manager.allocate(5));
    ASSERT_EQ(-1, manager.allocate(3));
    manager.deallocate(5); // should do nothing
}
}

