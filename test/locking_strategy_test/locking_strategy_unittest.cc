#include <cstdio>
#include <thread>
#include <future>
#include <chrono>
#include <ratio>

#include "gtest/gtest.h"

#include "locking_strategy.h"


typedef std::chrono::duration<float, std::ratio<1, 10> > BASE_DURATION;

TEST(LockingTest, SimpleExclusive) {
  EXPECT_EQ(0, get_exclusive_lock(1, 1, 1));
  EXPECT_EQ(1, has_exclusive_lock(1, 1, 1));
  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_exclusive_lock(1, 1, 1));
}

TEST(LockingTest, SimpleShared) {
  EXPECT_EQ(0, get_shared_lock(1, 1, 1));
  EXPECT_EQ(1, has_shared_lock(1, 1, 1));
  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, has_shared_lock(1, 1, 1));
}

TEST(LockingTest, SimpleExclusiveConflict) {
  EXPECT_EQ(0, get_exclusive_lock(1, 1, 1));
  EXPECT_EQ(-1, get_exclusive_lock(2, 1, 1));
  EXPECT_EQ(0, release_lock(1, 1, 1));
  EXPECT_EQ(0, get_exclusive_lock(2, 1, 1));
  EXPECT_EQ(0, release_lock(2, 1, 1));
}