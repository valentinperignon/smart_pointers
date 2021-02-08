#ifndef SP_SHARED_H
#define SP_SHARED_H

#include <cstddef>
#include <utility>

namespace sp {
  /**
   * @brief Counter of smart pointers
   */
  class ControlBlock {
  public:
    ControlBlock();

    size_t getUsePointer() const;

    size_t getWeakPointer() const;

    void increaseUsePointer();

    void decreaseUsePointer();

    void increaseWeakPointer();

    void decreaseWeakPointer();

  private:
    size_t useCount;

    size_t weakCount;
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
      , controlBlock(ptr != nullptr ? new ControlBlock() : nullptr)
    {
      if (this->pointer != nullptr) {
        this->controlBlock->increaseUsePointer();
      }
    }

    /**
     * @brief Destructor
     */
    ~Shared() {
      deletePointers();
    }

    /**
     * @brief Copy constructor
     */
    Shared(const Shared<T>& other)
      : pointer(other.pointer)
      , controlBlock(other.controlBlock)
    {
      if (this->controlBlock != nullptr) {
        this->controlBlock->increaseUsePointer();
      }
    }

    /**
     * @brief Move constructor
     */
    Shared(Shared&& other)
      : pointer(std::exchange(other.pointer, nullptr))
      , controlBlock(std::exchange(other.controlBlock, nullptr))
    {}

    /**
     * @brief Copy assignment
     */
    Shared& operator=(const Shared& other) {
      deletePointers();

      this->pointer = other.pointer;
      this->controlBlock = other.controlBlock;
      if (this->controlBlock != nullptr) {
        this->controlBlock->increaseUsePointer();
      }
      return *this;
    }

    /**
     * @brief Move assignment
     */
    Shared& operator=(Shared&& other) {
      std::swap(this->pointer, other.pointer);
      std::swap(this->controlBlock, other.controlBlock);
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
      if (this->controlBlock == nullptr) {
        return 0;
      }
      return this->controlBlock->getUsePointer();
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
    ControlBlock* controlBlock;

    Shared(T* ptr, ControlBlock* ctrlB)
      : pointer(ptr)
      , controlBlock(ctrlB)
    {
      if (this->pointer != nullptr) {
        this->controlBlock->increaseUsePointer();
      }
    }

    void deletePointers() {
      if (this->pointer != nullptr) {
        this->controlBlock->decreaseUsePointer();

        if (this->controlBlock->getUsePointer() == 0) {
          delete this->pointer;
          this->pointer = nullptr;

          if (this->controlBlock->getWeakPointer() == 0) {
            delete this->controlBlock;
            this->controlBlock = nullptr;
          }
        }
      }
    }
  };
}

#endif // SP_SHARED_H
