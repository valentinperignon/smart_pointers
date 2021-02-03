#ifndef SP_SHARED_H
#define SP_SHARED_H

#include <cstddef>
#include <utility>
#include <map>

#include <iostream>

namespace sp {
  class PtrCounter {
  public:
    PtrCounter();

    size_t get() const;

    void operator++();

    void operator++(int);

    void operator--();

    void operator--(int);

  private:
    size_t count;  
  };

  /**
   * @brief Smart shared pointer class
   */
  template<typename T>
  class Shared {
  public:
    /**
     * @brief Constructor takes a dynamic pointer
     */
    Shared(T* ptr = nullptr)
    : pointer(ptr)
    , counter(new PtrCounter())
    {
      if (this->pointer != nullptr) {
        ++(*this->counter);
      }
    }

    /**
     * @brief Destructor
     */
    ~Shared() {
      --(*this->counter);
      if (this->counter->get() <= 0) {
        delete this->pointer;
        delete this->counter;
      }
    }

    /**
     * @brief Copy constructor
     */
    Shared(const Shared<T>& other)
    : pointer(other.pointer)
    , counter(other.counter)
    {
      ++(*this->counter);
    }

    /**
     * @brief Move constructor
     */
    Shared(Shared&& other)
    : pointer(nullptr)
    {
      std::cout << "TODO" << std::endl;
    }

    /**
     * @brief Copy assignment
     */
    Shared& operator=(const Shared& other) {
      if (this->counter->get() == 0) {
        delete this->pointer;
        delete this->counter;
      }

      this->pointer = other.pointer;
      this->counter = other.counter;
      ++(*this->counter);
      return *this;
    }

    /**
     * @brief Move assignment
     */
    Shared& operator=(Shared&& other) {
      std::cout << "TODO" << std::endl;
      return *this;
    }

    /**
     * @brief Get the raw pointer
     */
    T* get() {
      return this->pointer;
    }

    /**
     * @brief Get a reference on pointed data
     */
    T& operator*() {
      return *(this->pointer);
    }

    /**
     * @brief Get the raw pointer
     */
    T* operator->() {
      return this->pointer;
    }

    /**
     * @brief Get the reference number on raw data
     */
    std::size_t count() const {
      return this->counter->get();
    }

    /**
     * @brief Get the number of Shared pointed on the current pointer
     */
    bool exists() const {
      return this->pointer != nullptr;
    }

    template<typename> friend class Weak;

  private:
    T* pointer;
    PtrCounter* counter;
  };
}

#endif // SP_SHARED_H
