#ifndef SP_WEAK_H
#define SP_WEAK_H

#include "Shared.h"

namespace sp {
  template<typename T>
  class Weak {
  public:
    Weak() {
    }

    Weak(const Shared<T>& shared) {
    }

    ~Weak() {
    }

    Weak(const Weak& other) {
    }

    Weak(Weak&& other) {
    }

    Weak& operator=(const Weak& other) {
      return *this;
    }

    Weak& operator=(Weak&& other) {
      return *this;
    }

    Weak& operator=(Shared<T>& shared) {
      return *this;
    }

    Shared<T> lock() {
      return Shared<T>();
    }

  private:
    // implementation defined
  };
}

#endif // SP_WEAK_H
