#ifndef SP_UNIQUE_H
#define SP_UNIQUE_H

namespace sp {
  /**
   * @brief Smart unique pointer
   */
  template<typename T>
  class Unique {
  public:
    /**
     * @brief Constructor takes a dynamic pointer
     */
    Unique(T* ptr = nullptr)
      : m_pointer(ptr)
    { }

    /**
     * @brief Destructor
     */
    ~Unique() {
      delete m_pointer;
    }

    /**
     * @brief Copy constructor - deleted
     */
    Unique(const Unique<T>& other) = delete;

    /**
     * @brief Move constructor
     */
    Unique(Unique&& other) : m_pointer(std::exchange(other.m_pointer, nullptr)) {}

    /**
     * @brief Copy assignment - deleted
     */
    Unique& operator=(const Unique& other) = delete;

    /**
     * @brief Move assignment
     */
    Unique& operator=(Unique&& other) {
      std::swap(m_pointer, other.m_pointer);
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
     * @brief Check if the raw pointer exists
     */
    bool exists() const {
      return m_pointer != nullptr;
    }

  private:
    T* m_pointer;
  };
}

#endif // SP_UNIQUE_H
