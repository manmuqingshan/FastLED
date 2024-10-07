#include "bytestreammemory.h"
#include <string.h>
#include "fl_math.h"

FASTLED_NAMESPACE_BEGIN

ByteStreamMemory::ByteStreamMemory(uint32_t size_buffer)
    : mBuffer(size_buffer) {}

ByteStreamMemory::~ByteStreamMemory() = default;

bool ByteStreamMemory::available() const {
    return !mBuffer.full();
}

size_t ByteStreamMemory::read(uint8_t *dst, size_t bytesToRead) {
    if (!available()) {
        return 0;
    }

    size_t actualBytesToRead = min(bytesToRead, mBuffer.size());
    size_t bytesRead = 0;

    while (bytesRead < actualBytesToRead) {
        dst[bytesRead] = mBuffer.pop_front();
        bytesRead++;
    }

    return bytesRead;
}

size_t ByteStreamMemory::write(const uint8_t* src, size_t n) {
    for (uint16_t i = 0; i < n; ++i) {
        if (mBuffer.full()) {
            return i;
        }
        mBuffer.push_back(src[i]);
    }
    return n;
}

FASTLED_NAMESPACE_END