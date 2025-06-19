#pragma once
#include <string>
#include <iostream>

struct PersonID {
    std::string passport;
    int birthYear;

    PersonID(std::string passport = "", int birthYear = 0)
        : passport(std::move(passport)), birthYear(birthYear) {}

    bool operator==(const PersonID& other) const {
        return passport == other.passport && birthYear == other.birthYear;
    }

    friend std::ostream& operator<<(std::ostream& os, const PersonID& id) {
        return os << "[" << id.passport << ", " << id.birthYear << "]";
    }
};
