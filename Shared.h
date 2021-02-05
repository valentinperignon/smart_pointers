#ifndef SP_SHARED_H
#define SP_SHARED_H

#include <cstddef>
#include <utility>
#include <cassert>

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
      , use_counter(ptr ? new PtrCounter() : nullptr)
      , weak_counter(ptr ? new PtrCounter() : nullptr)
    {
      if (this->pointer != nullptr) {
        ++(*this->use_counter);
      }
    }

    /**
     * @brief Destructor
     */
    ~Shared() {
      clean();
    }

    /**
     * @brief Copy constructor
     */
    Shared(const Shared<T>& other)
      : pointer(other.pointer)
      , use_counter(other.use_counter)
      , weak_counter(other.weak_counter)
    {
      ++(*this->use_counter);
    }

    /**
     * @brief Move constructor
     */
    Shared(Shared&& other)
      : pointer(std::exchange(other.pointer, nullptr))
      , use_counter(std::exchange(other.use_counter, nullptr))
      , weak_counter(std::exchange(other.weak_counter, nullptr))
    {}

    /**
     * @brief Copy assignment
     */
    Shared& operator=(const Shared& other) {
      if (pointer)
        std::cout << use_counter->get();
      clean();
      if (pointer)
        std::cout << use_counter->get();

      this->pointer = other.pointer;
      this->use_counter = other.use_counter;
      this->weak_counter = other.weak_counter;

      ++(*this->use_counter);

      return *this;
    }

    /**
     * @brief Move assignment
     */
    Shared& operator=(Shared&& other) {
      std::swap(pointer, other.pointer);
      std::swap(use_counter, other.use_counter);
      std::swap(weak_counter, other.weak_counter);
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
      return this->use_counter->get();
    }

    /**
     * @brief Get the number of Shared pointed on the current pointer
     */
    bool exists() const {
      return this->pointer != nullptr;
    }

    void print(const std::string str = "") const {
      if (pointer) {
        std::cout << "[" << str << "]" << " - ";
        std::cout << "V = " << *pointer << " | ";
        std::cout << "U = " << use_counter->get() << " | ";
        std::cout << "W = " << weak_counter->get() << std::endl;

      }
      else {
        std::cout << str << "- No exists\n";
      }
    }

    template<typename> friend class Weak;

  private:

    Shared(T* ptr, PtrCounter* use_counter, PtrCounter* weak_counter)
      : pointer(ptr)
      , use_counter(use_counter)
      , weak_counter(weak_counter)
    {}

    /**
     * Reset the shared pointer
     * Delete the resource if there is no longer a shared pointer that references it
     * And delete the counter if there is no longer weak pointer that reference it
     */
    void clean() {
      if (pointer) {
        //T number = *pointer;
        --(*use_counter);
        //std::cout << "shared cleaned 1," << number << std::endl;

        if (use_counter->get() == 0) {
          //std::cout << "shared cleaned 2," << number << std::endl;
          delete pointer;


          if (weak_counter->get() == 0) {
            //std::cout << "shared cleaned 3," << number << std::endl;
            delete use_counter;
            delete weak_counter;
            use_counter = nullptr;
            weak_counter = nullptr;
          }

          pointer = nullptr;
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

#endif // SP_SHARED_H
