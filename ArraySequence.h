#pragma once
#include "Sequence.h"
#include "DynamicArray.h"
#include <utility>

template <typename T>
class ArraySequence : public Sequence<T> {
private:
    DynamicArray<T> array;

public:
    ArraySequence() = default;
    ~ArraySequence() override = default;

    ArraySequence(const ArraySequence&) = default;
    ArraySequence& operator=(const ArraySequence&) = default;
    ArraySequence(ArraySequence&&) noexcept = default;
    ArraySequence& operator=(ArraySequence&&) noexcept = default;

    T& get(size_t idx) override { return array[idx]; }
    const T& get(size_t idx) const override { return array[idx]; }
    size_t getSize() const override { return array.getSize(); }

    void append(const T& value) override {
        array.push_back(value);
    }
    void append(T&& value) override {
        array.push_back(std::move(value));
    }
};