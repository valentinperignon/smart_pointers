#include "Shared.h"

namespace sp {
  ControlBlock::ControlBlock()
    : useCount(0)
    , weakCount(0)
  {}

  void ControlBlock::increaseUsePointer() {
    ++this->useCount;
  }

  void ControlBlock::decreaseUsePointer() {
    if (this->useCount > 0) {
      --this->useCount;
    }
  }

  void ControlBlock::increaseWeakPointer() {
    ++this->weakCount;
  }

  void ControlBlock::decreaseWeakPointer() {
    if (this->weakCount > 0) {
      --this->weakCount;
    }
  }

  size_t ControlBlock::getUsePointer() const {
    return this->useCount;
  }

  size_t ControlBlock::getWeakPointer() const {
    return this->weakCount;
  }
}
