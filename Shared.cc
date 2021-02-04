#include "Shared.h"

namespace sp {
  PtrCounter::PtrCounter()
    : count(0)
  { }

  size_t PtrCounter::get() const {
    return this->count;
  }

  void PtrCounter::operator++() {
    this->count++;
  }

  void PtrCounter::operator++(int) {
    this->count++;
  }

  void PtrCounter::operator--() {
    if (this->count == 0) {
      return;
    }
    this->count--;
  }

  void PtrCounter::operator--(int) {
    if (this->count == 0) {
      return;
    }
    this->count--;
  }
}
