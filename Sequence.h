#pragma once
#include <cstddef>

template <typename T>
class Sequence {
public:
    virtual ~Sequence() = default;
    virtual T& get(size_t index) = 0;
    virtual const T& get(size_t index) const = 0;
    virtual size_t getSize() const = 0;
    virtual void append(const T& value) = 0;  // добавляем copy-append
    virtual void append(T&& value) = 0;       // сохраняем move-append
};
