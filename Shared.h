#ifndef SP_SHARED_H
#define SP_SHARED_H

#include <cstddef>
#include <utility>

namespace sp {
  template<typename T>
  class Shared {
  public:
    Shared(T* ptr = nullptr)
    : pointer(ptr)
    { }

    ~Shared() {
    
    }

    Shared(const Shared<T>& other)
    : pointer(other.pointer)
    { }

    Shared(Shared&& other) {
      std::swap(this->pointer, other.pointer);
    }

    Shared& operator=(const Shared& other) {
      this->pointer = other.pointer;
      return *this;
    }

    Shared& operator=(Shared&& other) {
      std::swap(this->pointer, other.pointer);
      return *this;
    }

    T* get() {
      return this->pointer;
    }

    T& operator*() {
      return *(this->pointer);
    }

    T* operator->() {
      return this->pointer;
    }

    std::size_t count() const {
      return 0;
    }

    bool exists() const {
      return this->pointer != nullptr;
    }

    template<typename> friend class Weak;

  private:
    T* pointer;
  };
}

#endif // SP_SHARED_H
