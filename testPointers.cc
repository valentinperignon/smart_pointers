#include <gtest/gtest.h>

#include "Shared.h"
#include "Weak.h"
#include "Unique.h"

/* ---------- Unique Pointer ---------- */

TEST(MoveConstructor, Default) {
  sp::Unique<int> first(new int(4));


  sp::Unique<int> second = std::move(first);

  EXPECT_EQ(first.get(), nullptr);



  EXPECT_EQ(*second, 4);
}

TEST(MoveConstructor, Empty) {
  sp::Unique<int> first;

  EXPECT_FALSE(first.exists());

  sp::Unique<int> second = std::move(first);

  EXPECT_EQ(first.get(), nullptr);

  EXPECT_FALSE(second.exists());
  EXPECT_EQ(second.get(), nullptr);
}

TEST(MoveAssignment, Default) {
  sp::Unique<int> first(new int(6));

  sp::Unique<int> second;

  second = std::move(first);

  first.~Unique();

  EXPECT_EQ(*second, 6);
}

TEST(DoesExist, Default) {
  sp::Unique<int> unique(new int(42));
  EXPECT_NE(unique.get(), nullptr);
  EXPECT_TRUE(unique.exists());
}

TEST(DoesNotExist, Default) {
  sp::Unique<int> unique;
  EXPECT_EQ(unique.get(), nullptr);
  EXPECT_FALSE(unique.exists());
}

TEST(StarOperator, Default) {
  sp::Unique<int> unique(new int(3));
  ++(*unique);
  std::cout << *unique << std::endl;

  EXPECT_EQ(*unique, 4);
  EXPECT_TRUE(unique.exists());
}

TEST(Get, Default) {
  sp::Unique<int> unique(new int(19));

  EXPECT_EQ(*unique.get(), 19);
}

TEST(Get, Assigment) {
  sp::Unique<int> unique(new int(15));

  int* number = unique.get();

  *number = 20;

  EXPECT_EQ(*unique, 20);
}

/* ---------- Shared Pointer ---------- */

TEST(TestSharedPointer, ConstructorWithValue) {
  sp::Shared<int> sh(new int(42));
  EXPECT_TRUE(sh.exists());
}

TEST(TestSharedPointer, DefaultConstructor) {
  sp::Shared<int> sh;
  EXPECT_FALSE(sh.exists());
}

TEST(TestSharedPointer, CopyConstructorAndAssignement) {
  sp::Shared<int> sh1(new int(42));
  EXPECT_TRUE(sh1.exists());

  sp::Shared<int> sh2 = sh1;
  EXPECT_TRUE(sh2.exists());

  sp::Shared<int> sh3;
  sh3 = sh2;
  EXPECT_TRUE(sh3.exists());

  EXPECT_EQ(*sh1, 42);
  EXPECT_EQ(*sh2, 42);
  EXPECT_EQ(*sh3, 42);
}

TEST(TestSharedPointer, MoveConstructorAndAssignement) {
  sp::Shared<int> sh1(new int(42));
  EXPECT_TRUE(sh1.exists());

  sp::Shared<int> sh2 = std::move(sh1);
  EXPECT_TRUE(sh2.exists());

  sp::Shared<int> sh3;
  sh3 = std::move(sh2);
  EXPECT_TRUE(sh3.exists());
}

TEST(TestSharedPointer, Get) {
  sp::Shared<int> sh(new int(42));

  EXPECT_EQ(*(sh.get()), 42);
  *(sh.get()) = 4242;
  EXPECT_EQ(*sh, 4242);
}

TEST(TestSharedPointer, StarOperator) {
  sp::Shared<int> sh(new int(42));

  EXPECT_EQ(*sh, 42);
  *sh = 4242;
  EXPECT_EQ(*sh, 4242);
}

TEST(TestSharedPointer, ArrowPointer) {
  struct t {
    int a;
    int b;
  };

  sp::Shared<t> sh(new t);
  sh.get()->a = 42;
  sh.get()->b = 4242;
  EXPECT_EQ(sh->a, 42);
  EXPECT_EQ(sh->b, 4242);
}

TEST(TestSharedPointer, Count) {
  sp::Shared<int> sh1(new int(42));
  EXPECT_EQ(sh1.count(), 1u);

  sp::Shared<int> sh2 = sh1;
  EXPECT_EQ(sh1.count(), 2u);
  EXPECT_EQ(sh2.count(), 2u);

  {
    sp::Shared<int> tmp = sh2;
    EXPECT_EQ(sh1.count(), 3u);
    EXPECT_EQ(sh2.count(), 3u);
    EXPECT_EQ(tmp.count(), 3u);
  }
  EXPECT_EQ(sh1.count(), 2u);
  EXPECT_EQ(sh2.count(), 2u);

  sp::Shared<int> sh3;
  sh3 = sh1;
  EXPECT_EQ(sh1.count(), 3u);
  EXPECT_EQ(sh2.count(), 3u);
  EXPECT_EQ(sh3.count(), 3u);
}

TEST(TestSharedPointer, CountNull) {
  sp::Shared<int> sh1;
  EXPECT_EQ(sh1.count(), 0u);
  EXPECT_FALSE(sh1.exists());

  sp::Shared<int> sh2 = sh1;
  EXPECT_EQ(sh2.count(), 0u);
  EXPECT_FALSE(sh2.exists());

  sp::Shared<int> sh3;
  sh3 = sh2;
  EXPECT_EQ(sh3.count(), 0u);
  EXPECT_FALSE(sh3.exists());
}

/* ---------- Weak Pointer ---------- */

TEST(TestWeakPointer, ConstructorWithValue) {
  sp::Shared<int> sh(new int(42));

  sp::Weak<int> wk(sh);
  EXPECT_EQ(sh.count(), 1u);
  sp::Shared<int> shFromWk = wk.lock();
  EXPECT_EQ(sh.count(), 2u);

  EXPECT_TRUE(shFromWk.exists());
  EXPECT_EQ(*shFromWk, 42);
}

/* ---------- main ---------- */

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
