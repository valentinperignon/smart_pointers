#ifndef SP_SHARED_H
#define SP_SHARED_H

#include <cstddef>
#include <utility>
#include <map>

#include <iostream>

namespace sp {
  template<typename T>
  class Shared {
  public:
    Shared(T* ptr = nullptr)
    : pointer(ptr)
    {
      std::size_t newValue = 1;
      if (auto it = Shared<T>::listOfPointers.find(ptr); it != Shared<T>::listOfPointers.end()) {
        newValue = it->second + 1;
      }
      Shared::listOfPointers[ptr] = newValue;
    }

    ~Shared() {
      if (auto it = Shared<T>::listOfPointers.find(this->pointer); it->second == 1) {
        Shared<T>::listOfPointers.erase(it);
        delete this->pointer;
      } else {
        it->second -= 1;
      }
    }

    Shared(const Shared<T>& other)
    : pointer(other.pointer)
    {
      Shared<T>::listOfPointers[other.pointer] += 1;
    }

    Shared(Shared&& other) {
      std::swap(this->pointer, other.pointer);
    }

    Shared& operator=(const Shared& other) {
      Shared<T>::listOfPointers[this->pointer] -= 1;
      auto it = Shared<T>::listOfPointers.find(this->pointer);
      it->second -= 1;
      if (it->second == 0) {
        delete this->pointer;
        Shared<T>::listOfPointers.erase(it);
      }
      Shared<T>::listOfPointers[other.pointer] += 1;
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
    static std::map<T*, size_t> listOfPointers;
  };
}

#endif // SP_SHARED_H
