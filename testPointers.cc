#include <gtest/gtest.h>

#include "Shared.h"
#include "Weak.h"
#include "Unique.h"

TEST(test, test) {
  sp::Shared<int> shared;
  sp::Weak<int> weak;
  sp::Unique<int> unique;
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

  EXPECT_EQ(*sh1, 42);
  EXPECT_EQ(*sh2, 42);
}

TEST(TestSharedPointer, MoveConstructorAndAssignement) {
  sp::Shared<int> sh1(new int(42));
  EXPECT_TRUE(sh1.exists());

  sp::Shared<int> sh2 = std::move(sh1);
  EXPECT_TRUE(sh2.exists());
  EXPECT_EQ(*sh2, 42);

  sp::Shared<int> sh3;
  sh3 = std::move(sh2);
  EXPECT_TRUE(sh3.exists());
  EXPECT_EQ(*sh3, 42);
}

TEST(TestSharedPointer, GetPointer) {
  sp::Shared<int> sh(new int(42));

  EXPECT_EQ(*sh, 42);
  *sh = 4242;
  EXPECT_EQ(*sh, 4242);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
