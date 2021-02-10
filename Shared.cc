#include "Shared.h"

namespace sp {
  ControlBlock::ControlBlock()
    : m_useCount(0)
    , m_weakCount(0)
  {}

  void ControlBlock::increaseUsePointer() {
    ++m_useCount;
  }

  void ControlBlock::decreaseUsePointer() {
    if (m_useCount > 0) {
      --m_useCount;
    }
  }

  void ControlBlock::increaseWeakPointer() {
    ++m_weakCount;
  }

  void ControlBlock::decreaseWeakPointer() {
    if (m_weakCount > 0) {
      --m_weakCount;
    }
  }

  size_t ControlBlock::getUsePointer() const {
    return m_useCount;
  }

  size_t ControlBlock::getWeakPointer() const {
    return m_weakCount;
  }
}
