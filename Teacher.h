#pragma once
#include "Person.h"

class Teacher : public Person {
private:
    int years;

public:
    Teacher(std::string name = "", int years = 0, PersonID id = {})
        : Person(std::move(name), id), years(years) {}

    double norm() const { return static_cast<double>(years); }

    Teacher operator*(int k) const {
        return Teacher(name, years * k, id);
    }

    Teacher& operator*=(int k) {
        years *= k;
        return *this;
    }

    Teacher& operator+=(const Teacher& other) {
        years += other.years;
        return *this;
    }

    bool operator==(const Teacher& other) const {
        return static_cast<const Person&>(*this) == static_cast<const Person&>(other) &&
               years == other.years;
    }

    friend std::ostream& operator<<(std::ostream& os, const Teacher& t) {
        return os << "(Teacher: " << t.name << ", Years: " << t.years << ", ID: " << t.id << ")";
    }
};
