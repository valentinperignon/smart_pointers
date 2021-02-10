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

    /**
     * @brief Get the number of shared pointer
     */
    size_t getUsePointer() const;

    /**
     * @brief Get the number of weak pointer
     */
    size_t getWeakPointer() const;

    /**
     * @brief Increase by one the shared pointer counter
     */
    void increaseUsePointer();

    /**
     * @brief Decrease by one the shared pointer counter
     */
    void decreaseUsePointer();

    /**
     * @brief Increase by one the weak pointer counter
     */
    void increaseWeakPointer();

    /**
     * @brief Decrease by one the weak pointer counter
     */
    void decreaseWeakPointer();

  private:
    size_t m_useCount;
    size_t m_weakCount;
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
      : m_pointer(ptr)
      , m_controlBlock(ptr != nullptr ? new ControlBlock() : nullptr)
    {
      if (m_controlBlock != nullptr) {
        m_controlBlock->increaseUsePointer();
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
      : m_pointer(other.m_pointer)
      , m_controlBlock(other.m_controlBlock)
    {
      if (m_controlBlock != nullptr) {
        m_controlBlock->increaseUsePointer();
      }
    }

    /**
     * @brief Move constructor
     */
    Shared(Shared&& other)
      : m_pointer(std::exchange(other.m_pointer, nullptr))
      , m_controlBlock(std::exchange(other.m_controlBlock, nullptr))
    {}

    /**
     * @brief Copy assignment
     */
    Shared& operator=(const Shared& other) {
      deletePointers();

      m_pointer = other.m_pointer;
      m_controlBlock = other.m_controlBlock;
      if (m_controlBlock != nullptr) {
        m_controlBlock->increaseUsePointer();
      }
      return *this;
    }

    /**
     * @brief Move assignment
     */
    Shared& operator=(Shared&& other) {
      std::swap(m_pointer, other.m_pointer);
      std::swap(m_controlBlock, other.m_controlBlock);
      return *this;
    }

    /**
     * @brief Get the raw pointer
     */
    T* get() {
      return m_pointer;
    }

    /**
     * @brief Get a reference on pointed data
     */
    T& operator*() {
      return *(m_pointer);
    }

    /**
     * @brief Get the raw pointer
     */
    T* operator->() {
      return m_pointer;
    }

    /**
     * @brief Get the reference number on raw data
     */
    std::size_t count() const {
      if (m_controlBlock == nullptr) {
        return 0;
      }
      return m_controlBlock->getUsePointer();
    }

    /**
     * @brief Get the number of Shared pointed on the current pointer
     */
    bool exists() const {
      return m_pointer != nullptr;
    }

    template<typename> friend class Weak;

  private:
    T* m_pointer;
    ControlBlock* m_controlBlock;

    Shared(T* ptr, ControlBlock* ctrlB)
      : m_pointer(ptr)
      , m_controlBlock(ctrlB)
    {
      if (m_pointer != nullptr) {
        m_controlBlock->increaseUsePointer();
      }
    }

    void deletePointers() {
      if (m_pointer != nullptr) {
        m_controlBlock->decreaseUsePointer();

        if (m_controlBlock->getUsePointer() == 0) {
          delete m_pointer;
          m_pointer = nullptr;

          if (m_controlBlock->getWeakPointer() == 0) {
            delete m_controlBlock;
            m_controlBlock = nullptr;
          }
        }
      }
    }
  };
}

#endif // SP_SHARED_H
