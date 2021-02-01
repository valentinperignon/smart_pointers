#include <gtest/gtest.h>

#include "Shared.h"
#include "Weak.h"
#include "Unique.h"

TEST(Test, test) {
  sp::Shared<int> shared;
  sp::Weak<int> weak;
  sp::Unique<int> unique;
}


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

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
