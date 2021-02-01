#ifndef SP_SHARED_H
#define SP_SHARED_H

#include <cstddef>
#include <utility>

namespace sp {
  template<typename T>
  class Shared {
  public:
    Shared(T* ptr = nullptr) {
    }

    ~Shared() {
    }

    Shared(const Shared<T>& other) {
    }

    Shared(Shared&& other) {
    }

    Shared& operator=(const Shared& other) {
      return *this;
    }

    Shared& operator=(Shared&& other) {
      return *this;
    }

    T* get() {
      return nullptr;
    }

    T& operator*() {
      return T();
    }

    T* operator->() {
      return nullptr;
    }

    std::size_t count() const {
      return 0;
    }

    bool exists() const {
      return false;
    }

    template<typename> friend class Weak;

  private:

  };
}

#endif // SP_SHARED_H
