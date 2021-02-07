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
      , controlBlock(nullptr)
    {}

    /**
     * @brief Constructor takes a Shared pointer
     */
    Weak(const Shared<T>& shared)
      : pointer(shared.pointer)
      , controlBlock(shared.controlBlock)
    {}

    /**
     * @brief Destructeur
     */
    ~Weak() {
      deletePointers();
    }

    /**
     * @brief Copy constructor
     */
    Weak(const Weak& other)
      : pointer(other.pointer)
      , controlBlock(other.controlBlock)
    {
      this->controlBlock->increaseWeakPointer();
    }

    /**
     * @brief Move constructor
     */
    Weak(Weak&& other)
      : pointer(std::exchange(other.pointer, nullptr))
      , controlBlock(std::exchange(other.controlBlock, nullptr))
    {}

    /**
     * @brief Copy assignment
     */
    Weak& operator=(const Weak& other) {
      this->pointer = other.pointer;
      this->controlBlock = other.controlBlock;
      this->controlBlock->increaseWeakPointer();
      return *this;
    }

    /**
     * @brief Move assignment
     */
    Weak& operator=(Weak&& other) {
      deletePointers();
      
      std::swap(this->pointer, other.pointer);
      std::swap(this->controlBlock, other.controlBlock);
      return *this;
    }

    /**
     * @brief Assignment from Shared
     */
    Weak& operator=(Shared<T>& shared) {
      this->pointer = shared.pointer;
      this->controlBlock = shared.controlBlock;
      this->controlBlock->increaseWeakPointer();
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
      return Shared<T>(this->pointer, this->controlBlock);
    }

  private:
    T* pointer;
    ControlBlock* controlBlock;

    void deletePointers() {
      if (this->pointer != nullptr) {
        this->pointer = nullptr;
        this->controlBlock->decreaseWeakPointer();

        if (
            this->controlBlock->getUsePointer() == 0
              &&
            this->controlBlock->getWeakPointer() == 0
          ) {
          delete this->controlBlock;
          this->controlBlock = nullptr;
        }
      }
    }
  };
}

#endif // SP_WEAK_H
