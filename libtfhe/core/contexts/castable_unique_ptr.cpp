#include "castable_unique_ptr.h"

void CastableUniquePointer::deleteSmartPointer() {
    if (data == nullptr) return;
    deleter(data);
    data = nullptr;
    deleter = nullptr;
}

void CastableUniquePointer::constructMove(CastableUniquePointer &&smartPointer) {
    data = smartPointer.data;
    deleter = smartPointer.deleter;
    smartPointer.data = 0;
    smartPointer.deleter = 0;
}

void CastableUniquePointer::operator=(CastableUniquePointer &&smartPointer) {
    deleteSmartPointer();
    constructMove(std::move(smartPointer));
}


void CastableUniquePointer::constructGrab(void *data, DELETER_FUN deleter) {
    this->data = data;
    if (data != nullptr) {
        this->deleter = deleter;
    } else {
        this->deleter = nullptr;
    }
}

