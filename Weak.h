#ifndef SP_WEAK_H
#define SP_WEAK_H

#include "Shared.h"

namespace sp {
  /**
   * @brief Smart weak pointer
   */
  template<typename T>
  class Weak {
  public:
    /**
     * @brief Default constructor
     */
    Weak()
      : pointer(nullptr)
      , use_counter(nullptr)
      , weak_counter(nullptr)
    {}

    /**
     * @brief Constructor takes a Shared pointer
     */
    Weak(const Shared<T>& shared)
      : pointer(shared.pointer)
      , use_counter(shared.use_counter)
      , weak_counter(shared.weak_counter)
    {
      (*weak_counter)++;
    }

    /**
     * @brief Destructeur
     */
    ~Weak() {
      clean();
    }

    /**
     * @brief Copy constructor
     */
    Weak(const Weak& other) {
      pointer = other.pointer;
      use_counter = other.use_counter;
      weak_counter = other.weak_counter;
      (*weak_counter)++;
    }

    /**
     * @brief Move constructor
     */
    Weak(Weak&& other)
      : pointer(std::exchange(other.pointer, nullptr))
      , use_counter(std::exchange(other.use_counter, nullptr))
      , weak_counter(std::exchange(other.weak_counter, nullptr))
    {}

    /**
     * @brief Copy assignment
     */
    Weak& operator=(const Weak& other) {
      clean();
      pointer = other.pointer;
      use_counter = other.use_counter;
      weak_counter = other.weak_counter;

      (*weak_counter)++;

      return *this;
    }

    /**
     * @brief Move assignment
     */
    Weak& operator=(Weak&& other) {
      clean();
      std::swap(pointer, other.pointer);
      std::swap(use_counter, other.use_counter);
      std::swap(weak_counter, other.weak_counter);

      return *this;
    }

    /**
     * @brief Assignment from Shared
     */
    Weak& operator=(Shared<T>& shared) {
      pointer = shared.pointer;
      use_counter = shared.use_counter;
      weak_counter = shared.weak_counter;
      (*weak_counter)++;
      return *this;
    }

    /**
     * @brief Get a Shared pointer from the Weak pointer
     *
     * If the raw pointer still exists, the method
     * initialize a Shared object. Otherwise, the method
     * retrun a non existing Shared pointeur.
     */
    Shared<T> lock() {
      (*use_counter)++;
      return Shared<int>(pointer, use_counter, weak_counter);
    }

  private:

    void clean() {
      if (pointer) {
        (*weak_counter)--;
        std::cout << use_counter->get() << "-" << weak_counter->get() << std::endl;
        if (use_counter->get() == 0 && weak_counter->get() == 0) {
          delete use_counter;
          delete weak_counter;
          use_counter = nullptr;
          weak_counter = nullptr;
        }
      }
      else {
        assert(use_counter == nullptr);
        assert(weak_counter == nullptr);
      }
    }

    T* pointer;
    PtrCounter* use_counter;
    PtrCounter* weak_counter;
  };
}

#endif // SP_WEAK_H
