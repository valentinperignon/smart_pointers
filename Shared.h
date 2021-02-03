#ifndef SP_SHARED_H
#define SP_SHARED_H

#include <cstddef>
#include <utility>
#include <map>

#include <iostream>

namespace sp {
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
    {
      std::size_t newValue = 1;
      if (auto it = Shared<T>::listOfPointers.find(ptr); it != Shared<T>::listOfPointers.end()) {
        newValue = it->second + 1;
      }
      Shared::listOfPointers[ptr] = newValue;
    }

    /**
     * @brief Destructor
     */
    ~Shared() {
      if (auto it = Shared<T>::listOfPointers.find(this->pointer); it->second == 1) {
        Shared<T>::listOfPointers.erase(it);
        delete this->pointer;
      } else {
        it->second -= 1;
      }
    }

    /**
     * @brief Copy constructor
     */
    Shared(const Shared<T>& other)
    : pointer(other.pointer)
    {
      Shared<T>::listOfPointers[other.pointer] += 1;
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

    /**
     * @brief Move assignment
     */
    Shared& operator=(Shared&& other) {
      std::cout << "TODO" << std::endl;
      this->pointer = nullptr;
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
      return Shared<T>::listOfPointers[this->pointer];
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
    static std::map<T*, size_t> listOfPointers;
  };
}

#endif // SP_SHARED_H
