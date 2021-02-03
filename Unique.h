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
      : pointer(ptr)
    { }

    /**
     * @brief Destructor
     */
    ~Unique() {
      delete this->pointer;
    }

    /**
     * @brief Copy constructor - deleted
     */
    Unique(const Unique<T>& other) = delete;

    /**
     * @brief Move constructor
     */
    Unique(Unique&& other) : pointer(std::exchange(other.pointer, nullptr)) {}

    /**
     * @brief Copy assignment - deleted
     */
    Unique& operator=(const Unique& other) = delete;

    /**
     * @brief Move assignment
     */
    Unique& operator=(Unique&& other) {
      std::swap(this->pointer, other.pointer);
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
     * @brief Check if the raw pointer exists
     */
    bool exists() const {
      return this->pointer != nullptr;
    }

  private:
    T* pointer;
  };
}

#endif // SP_UNIQUE_H
