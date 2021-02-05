#include <gtest/gtest.h>

#include "Shared.h"
#include "Weak.h"
#include "Unique.h"

TEST(Test, test) {
  sp::Shared<int> shared;
  sp::Weak<int> weak;
  sp::Unique<int> unique;
}

/* ---------- Unique Pointer ---------- */

// TEST(TestUniquePointer, Move) {
//   sp::Unique<int> first(new int(4));

//   sp::Unique<int> second = std::move(first);

//   EXPECT_EQ(first.get(), nullptr);

//   EXPECT_EQ(*second, 4);
// }

// TEST(TestUniquePointer, MoveEmpty) {
//   sp::Unique<int> first;

//   EXPECT_FALSE(first.exists());

//   sp::Unique<int> second = std::move(first);

//   EXPECT_EQ(first.get(), nullptr);

//   EXPECT_FALSE(second.exists());
//   EXPECT_EQ(second.get(), nullptr);
// }

// TEST(TestUniquePointer, MoveAssignment) {
//   sp::Unique<int> first(new int(6));

//   sp::Unique<int> second;

//   second = std::move(first);

//   first.~Unique();

//   EXPECT_EQ(*second, 6);
// }

// TEST(TestUniquePointer, DoesExist) {
//   sp::Unique<int> unique(new int(42));
//   EXPECT_NE(unique.get(), nullptr);
//   EXPECT_TRUE(unique.exists());
// }

// TEST(TestUniquePointer, DoesNotExist) {
//   sp::Unique<int> unique;
//   EXPECT_EQ(unique.get(), nullptr);
//   EXPECT_FALSE(unique.exists());
// }

// TEST(TestUniquePointer, StarOperator) {
//   sp::Unique<int> unique(new int(3));
//   ++(*unique);
//   std::cout << *unique << std::endl;

//   EXPECT_EQ(*unique, 4);
//   EXPECT_TRUE(unique.exists());
// }

// TEST(TestUniquePointer, Get) {
//   sp::Unique<int> unique(new int(19));

//   EXPECT_EQ(*unique.get(), 19);
// }

// TEST(TestUniquePointer, GetModified) {
//   sp::Unique<int> unique(new int(15));

//   int* number = unique.get();

//   *number = 20;

//   EXPECT_EQ(*unique, 20);
// }

// /* ---------- Shared Pointer ---------- */

// TEST(TestSharedPointer, ConstructorWithValue) {
//   sp::Shared<int> sh(new int(42));
//   EXPECT_TRUE(sh.exists());
// }

// TEST(TestSharedPointer, DefaultConstructor) {
//   sp::Shared<int> sh;
//   EXPECT_FALSE(sh.exists());
// }

// TEST(TestSharedPointer, CopyConstructorAndAssignement) {
//   sp::Shared<int> sh1(new int(42));
//   EXPECT_TRUE(sh1.exists());

//   sp::Shared<int> sh2 = sh1;
//   EXPECT_TRUE(sh2.exists());

//   EXPECT_EQ(*sh1, 42);
//   EXPECT_EQ(*sh2, 42);
// }

// TEST(TestSharedPointer, Get) {
//   sp::Shared<int> sh(new int(42));

//   EXPECT_EQ(*(sh.get()), 42);
//   *(sh.get()) = 4242;
//   EXPECT_EQ(*sh, 4242);
// }

// TEST(TestSharedPointer, StarOperator) {
//   sp::Shared<int> sh(new int(42));

//   EXPECT_EQ(*sh, 42);
//   *sh = 4242;
//   EXPECT_EQ(*sh, 4242);
// }

// TEST(TestSharedPointer, ArrowPointer) {
//   struct t {
//     int a;
//     int b;
//   };

//   sp::Shared<t> sh(new t);
//   sh.get()->a = 42;
//   sh.get()->b = 4242;
//   EXPECT_EQ(sh->a, 42);
//   EXPECT_EQ(sh->b, 4242);
// }

// TEST(TestSharedPointer, Count) {
//   sp::Shared<int> sh1(new int(42));
//   EXPECT_EQ(sh1.count(), 1u);

//   sp::Shared<int> sh2 = sh1;
//   EXPECT_EQ(sh1.count(), 2u);
//   EXPECT_EQ(sh2.count(), 2u);

//   {
//     sp::Shared<int> tmp = sh2;
//     EXPECT_EQ(sh1.count(), 3u);
//     EXPECT_EQ(sh2.count(), 3u);
//     EXPECT_EQ(tmp.count(), 3u);
//   }
//   EXPECT_EQ(sh1.count(), 2u);
//   EXPECT_EQ(sh2.count(), 2u);

//   sp::Shared<int> sh3;
//   sh3 = sh1;
//   EXPECT_EQ(sh1.count(), 3u);
//   EXPECT_EQ(sh2.count(), 3u);
//   EXPECT_EQ(sh3.count(), 3u);
// }

/* ---------- Weak Pointer ---------- */

TEST(TestWeakPointer, Default) {
  sp::Shared<int> shared(new int(42));
  EXPECT_EQ(*shared, 42);

  shared.print("shared 1");

  sp::Weak<int> weak1(shared);
  shared.print("shared 2");
  weak1.print("weak1 1");
  {
    auto tmp = weak1.lock();
    weak1.print("weak1 2");
    shared.print("shared 3");
    tmp.print("tmp 1");
    EXPECT_TRUE(tmp.exists());
    (*tmp) /= 2;
    EXPECT_EQ(*tmp, 21);

  }

  shared.print("shared 4");
  weak1.print("weak1 3");

  shared = sp::Shared<int>(new int(1337));

  shared.print("shared 5");
  weak1.print("weak1 4");

  sp::Weak<int> weak2(shared);

  weak2.print("weak2 1");

  {
    auto tmp = weak1.lock();
    tmp.print("tmp 2");
    weak2.print("weak2 2");
    shared.print("shared 6");
    EXPECT_FALSE(tmp.exists());

    tmp = weak2.lock();
    tmp.print("tmp 3");
    weak2.print("weak2 3");
    shared.print("shared 7");
    EXPECT_EQ(*tmp, 1337);
  }

  shared.print("shared 8");
  weak2.print("weak2 4");

}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
