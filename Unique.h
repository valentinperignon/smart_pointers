#ifndef SP_UNIQUE_H
#define SP_UNIQUE_H

namespace sp {
  template<typename T>
  class Unique {
  public:
    Unique(T* ptr = nullptr) {
    }

    ~Unique() {
    }

    Unique(const Unique<T>& other) = delete;

    Unique(Unique&& other) {
    }

    Unique& operator=(const Unique& other) = delete;

    Unique& operator=(Unique&& other) {
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

    bool exists() const {
      return false;
    }

  private:
    // implementation defined
  };
}

#endif // SP_UNIQUE_H
