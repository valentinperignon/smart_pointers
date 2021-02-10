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
      : m_pointer(nullptr)
      , m_controlBlock(nullptr)
    {}

    /**
     * @brief Constructor takes a Shared pointer
     */
    Weak(const Shared<T>& shared)
      : m_pointer(shared.m_pointer)
      , m_controlBlock(shared.m_controlBlock)
    {
      if (m_controlBlock != nullptr) {
        m_controlBlock->increaseWeakPointer();
      }
    }

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
      : m_pointer(other.m_pointer)
      , m_controlBlock(other.m_controlBlock)
    {
      if (m_controlBlock) {
        m_controlBlock->increaseWeakPointer();
      }
    }

    /**
     * @brief Move constructor
     */
    Weak(Weak&& other)
      : m_pointer(std::exchange(other.m_pointer, nullptr))
      , m_controlBlock(std::exchange(other.m_controlBlock, nullptr))
    {}

    /**
     * @brief Copy assignment
     */
    Weak& operator=(const Weak& other) {
      deletePointers();

      m_pointer = other.m_pointer;
      m_controlBlock = other.m_controlBlock;
      if (m_controlBlock != nullptr)  {
        m_controlBlock->increaseWeakPointer();
      }
      return *this;
    }

    /**
     * @brief Move assignment
     */
    Weak& operator=(Weak&& other) {
      deletePointers();
      
      std::swap(m_pointer, other.m_pointer);
      std::swap(m_controlBlock, other.m_controlBlock);
      return *this;
    }

    /**
     * @brief Assignment from Shared
     */
    Weak& operator=(Shared<T>& shared) {
      deletePointers();
      
      m_pointer = shared.m_pointer;
      m_controlBlock = shared.m_controlBlock;
      if (m_controlBlock != nullptr) {
        m_controlBlock->increaseWeakPointer();
      }
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
      if (m_controlBlock == nullptr || m_controlBlock->getUsePointer() == 0) {
        return Shared<T>(nullptr);
      }
      return Shared<T>(m_pointer, m_controlBlock);
    }

  private:
    T* m_pointer;
    ControlBlock* m_controlBlock;

    void deletePointers() {
      if (m_pointer != nullptr) {
        m_pointer = nullptr;
        m_controlBlock->decreaseWeakPointer();

        if (
          m_controlBlock->getUsePointer() == 0
          &&
          m_controlBlock->getWeakPointer() == 0
          ) {
          delete m_controlBlock;
          m_controlBlock = nullptr;
        }
      }
    }
  };
}

#endif // SP_WEAK_H
