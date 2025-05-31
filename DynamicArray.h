#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T>
class DynamicArray {
private:
    T* data = nullptr;
    size_t capacity = 0;
    size_t size = 0;

    void resize() {
        capacity = capacity ? capacity * 2 : 1;
        T* new_data = new T[capacity];
        for (size_t i = 0; i < size; ++i)
            new_data[i] = std::move(data[i]);
        delete[] data;
        data = new_data;
    }

public:
    DynamicArray() = default;

    // Copy constructor
    DynamicArray(const DynamicArray& other)
        : capacity(other.capacity), size(other.size)
    {
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i)
            data[i] = other.data[i];
    }

    // Copy assignment
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            size = other.size;
            data = new T[capacity];
            for (size_t i = 0; i < size; ++i)
                data[i] = other.data[i];
        }
        return *this;
    }

    // Move constructor
    DynamicArray(DynamicArray&& other) noexcept
        : data(other.data), capacity(other.capacity), size(other.size)
    {
        other.data = nullptr;
        other.capacity = 0;
        other.size = 0;
    }

    // Move assignment
    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            capacity = other.capacity;
            size = other.size;
            other.data = nullptr;
            other.capacity = 0;
            other.size = 0;
        }
        return *this;
    }

    ~DynamicArray() { delete[] data; }

    void push_back(const T& value) {
        if (size >= capacity) resize();
        data[size++] = value;
    }

    void push_back(T&& value) {
        if (size >= capacity) resize();
        data[size++] = std::move(value);
    }

    T& operator[](size_t idx) {
        if (idx >= size) throw std::out_of_range("Index");
        return data[idx];
    }
    const T& operator[](size_t idx) const {
        if (idx >= size) throw std::out_of_range("Index");
        return data[idx];
    }

    size_t getSize() const { return size; }
};