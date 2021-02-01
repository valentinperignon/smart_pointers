#ifndef SP_UNIQUE_H
#define SP_UNIQUE_H

namespace sp {
  template<typename T>
  class Unique {
  public:
    Unique(T* ptr = nullptr)
      : pointer(ptr)
    { }

    ~Unique() {
      delete this->pointer;
    }

    Unique(const Unique<T>& other) = delete;

    Unique(Unique&& other) : pointer(std::exchange(other.pointer, nullptr)) {}

    Unique& operator=(const Unique& other) = delete;

    Unique& operator=(Unique&& other) {
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

    bool exists() const {
      return this->pointer != nullptr;
    }

  private:
    T* pointer;
  };
}

#endif // SP_UNIQUE_H
