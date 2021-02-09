#include <gtest/gtest.h>

#include "Shared.h"
#include "Weak.h"
#include "Unique.h"

/* ---------- Unique Pointer ---------- */

TEST(TestUniquePointer, Move) {
  sp::Unique<int> first(new int(4));
  sp::Unique<int> second = std::move(first);

  EXPECT_EQ(*second, 4);
}

TEST(TestUniquePointer, MoveAssignment) {
  sp::Unique<int> first(new int(6));

  sp::Unique<int> second;
  second = std::move(first);

  EXPECT_EQ(*second, 6);
}

TEST(TestUniquePointer, MoveEmpty) {
  sp::Unique<int> first;
  EXPECT_FALSE(first.exists());

  sp::Unique<int> second = std::move(first);
  EXPECT_FALSE(second.exists());

  sp::Unique<int> third;
  third = std::move(second);
  EXPECT_FALSE(third.exists());
}

TEST(TestUniquePointer, DoesExist) {
  sp::Unique<int> unique(new int(42));
  EXPECT_NE(unique.get(), nullptr);
  EXPECT_TRUE(unique.exists());
}

TEST(TestUniquePointer, DoesNotExist) {
  sp::Unique<int> unique;
  EXPECT_EQ(unique.get(), nullptr);
  EXPECT_FALSE(unique.exists());
}

TEST(TestUniquePointer, StarOperator) {
  sp::Unique<int> unique(new int(3));
  ++(*unique);

  EXPECT_EQ(*unique, 4);
  EXPECT_TRUE(unique.exists());
}

TEST(TestUniquePointer, Get) {
  sp::Unique<int> unique(new int(19));

  EXPECT_EQ(*unique.get(), 19);
}

TEST(TestUniquePointer, GetModified) {
  sp::Unique<int> unique(new int(15));

  int* number = unique.get();
  *number = 20;
  EXPECT_EQ(*unique, 20);
}

/* ---------- Shared Pointer ---------- */

TEST(TestSharedPointer, ConstructorWithValue) {
  sp::Shared<int> sh(new int(42));
  EXPECT_TRUE(sh.exists());
  EXPECT_EQ(*sh, 42);
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

TEST(TestSharedPointer, MoveEmpty) {
  sp::Shared<double> first;
  EXPECT_FALSE(first.exists());

  sp::Shared<double> second = std::move(first);
  EXPECT_FALSE(second.exists());

  sp::Shared<double> third;
  third = std::move(second);
  EXPECT_FALSE(third.exists());
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
    double b;
  };

  sp::Shared<t> sh(new t);
  sh.get()->a = 42;
  sh.get()->b = 42.42;
  EXPECT_EQ(sh->a, 42);
  EXPECT_EQ(sh->b, 42.42);
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

TEST(TestSharedPointer, Exists) {
  sp::Shared<int> sh1;
  EXPECT_FALSE(sh1.exists());

  sp::Shared<int> sh2(new int(42));
  EXPECT_TRUE(sh2.exists());

  sh1 = sh2;
  EXPECT_TRUE(sh1.exists());
}

/* ---------- Weak Pointer ---------- */

TEST(TestWeakPointer, Default) {
  sp::Shared<int> shared(new int(42));
  EXPECT_EQ(*shared, 42);

  sp::Weak<int> weak1(shared);
  {
    auto tmp = weak1.lock();
    EXPECT_TRUE(tmp.exists());
    (*tmp) /= 2;
    EXPECT_EQ(*tmp, 21);
  }

  shared = sp::Shared<int>(new int(1337));

  sp::Weak<int> weak2(shared);
  {
    auto tmp = weak1.lock();
    EXPECT_FALSE(tmp.exists());

    tmp = weak2.lock();
    EXPECT_EQ(*tmp, 1337);
  }
}

TEST(TestWeakPointer, ConstructorWithValue) {
  sp::Shared<int> sh(new int(42));

  sp::Weak<int> wk(sh);
  EXPECT_EQ(sh.count(), 1u);
  sp::Shared<int> shFromWk = wk.lock();
  EXPECT_EQ(sh.count(), 2u);

  EXPECT_TRUE(shFromWk.exists());
  EXPECT_EQ(*shFromWk, 42);
}

TEST(TestWeakPointer, ConstructorWithoutValue) {
  sp::Weak<int> wk;
  EXPECT_FALSE(wk.lock().exists());
}

TEST(TestWeakPointer, CopyConstructorAndAssignement) {
  sp::Shared<int> sh(new int(42));

  sp::Weak<int> wk1(sh);
  sp::Shared<int> shFromWk1 = wk1.lock();

  sp::Weak<int> wk2 = wk1;
  sp::Shared<int> shFromWk2 = wk2.lock();
  EXPECT_EQ(shFromWk1.count(), shFromWk2.count());
  EXPECT_EQ(*shFromWk1, *shFromWk2);

  sp::Weak<int> wk3;
  wk3 = wk1;
  sp::Shared<int> shFromWk3 = wk3.lock();
  EXPECT_EQ(shFromWk1.count(), shFromWk3.count());
  EXPECT_EQ(*shFromWk1, *shFromWk3);
}

TEST(TestWeakPointer, MoveAssigment) {
  sp::Weak<std::string> weak0;

  {
    sp::Shared<std::string> shared(new std::string("Should we use a Trello ? "));
    EXPECT_EQ(*shared, "Should we use a Trello ? ");

    sp::Weak<std::string> weak1(shared);

    EXPECT_TRUE(shared.exists());

    sp::Weak<std::string> weak2(weak1);

    EXPECT_TRUE(weak1.lock().exists());
    EXPECT_EQ(*weak1.lock(), *weak2.lock());

    weak0 = std::move(weak2);
    EXPECT_TRUE(weak0.lock().exists());
  }

  EXPECT_FALSE(weak0.lock().exists());
}

TEST(TestWeakPointer, MoveConstructor) {
  sp::Weak<double> weak0;
  {
    sp::Shared<double> shared = new double(3.14);

    {
      sp::Weak<double> weak1 = shared;

      EXPECT_EQ(*weak1.lock().get(), 3.14);

      sp::Weak<double> weak2 = std::move(weak1);

      EXPECT_EQ(*weak2.lock().get(), 3.14);
    }

    weak0 = shared;
    EXPECT_TRUE(shared.exists());
    EXPECT_EQ(*weak0.lock().get(), 3.14);
  }
  EXPECT_FALSE(weak0.lock().exists());

  sp::Shared<double> shared2(new double(42.0));
  weak0 = shared2;
  EXPECT_TRUE(weak0.lock().exists());
  EXPECT_EQ(*weak0.lock().get(), 42.0);
}

TEST(TestWeakPointer, EqualSharedOperator) {
  sp::Shared<int> sh(new int(42));
  EXPECT_EQ(sh.count(), 1u);

  sp::Shared<int> tmp(new int(4242));
  sp::Weak<int> wk(tmp);
  EXPECT_EQ(*(wk.lock()), 4242);

  wk = sh;
  sp::Shared<int> shFromWk = wk.lock();
  EXPECT_EQ(*shFromWk, 42);
  EXPECT_EQ(shFromWk.count(), 2u);
  EXPECT_EQ(shFromWk.count(), sh.count());
  EXPECT_EQ(*shFromWk, *sh);
}

TEST(TestWeakPointer, Lock) {
  sp::Shared<int> sh(new int(42));
  sp::Weak<int> wk(sh);

  sp::Shared<int> shFromWk = wk.lock();
  EXPECT_TRUE(shFromWk.exists());
  EXPECT_EQ(*shFromWk, 42);
  EXPECT_EQ(shFromWk.count(), 2u);

  sp::Weak<double> wk2;
  sp::Shared<double> shFromWk2 = wk2.lock();
  EXPECT_FALSE(shFromWk2.exists());
}

TEST(TestWeakPointer, Count) {
  sp::Shared<int> sh(new int(42));
  EXPECT_EQ(sh.count(), 1u);

  sp::Weak<int> wk1 = sh;
  sp::Weak<int> wk2 = sh;
  
  {
    sp::Shared<int> fromWk1 = wk1.lock();
    EXPECT_EQ(sh.count(), 2u);
    EXPECT_EQ(sh.count(), fromWk1.count());
  }

  EXPECT_EQ(sh.count(), 1u);
}

TEST(TestWeakPointer, LifeCycleSimple) {
  sp::Weak<int> wk;
  EXPECT_FALSE(wk.lock().exists());

  {
    sp::Shared<int> sh(new int(42));
    wk = sh;
    EXPECT_TRUE(wk.lock().exists());
  }

  EXPECT_FALSE(wk.lock().exists());
}

/* ---------- main ---------- */

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
