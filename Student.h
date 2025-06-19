#pragma once
#include "Person.h"

class Student : public Person {
private:
    int grade;

public:
    Student(std::string name = "", int grade = 0, PersonID id = {})
        : Person(std::move(name), id), grade(grade) {}

    double norm() const { return static_cast<double>(grade); }

    Student operator*(int k) const {
        return Student(name, grade * k, id);
    }

    Student& operator*=(int k) {
        grade *= k;
        return *this;
    }

    Student& operator+=(const Student& other) {
        grade += other.grade;
        return *this;
    }

    bool operator==(const Student& other) const {
        return static_cast<const Person&>(*this) == static_cast<const Person&>(other) &&
               grade == other.grade;
    }

    friend std::ostream& operator<<(std::ostream& os, const Student& s) {
        return os << "(Student: " << s.name << ", Grade: " << s.grade << ", ID: " << s.id << ")";
    }
};
