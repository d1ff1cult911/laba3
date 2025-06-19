#pragma once
#include "PersonID.h"

class Person {
protected:
    std::string name;
    PersonID id;

public:
    Person(std::string name = "", PersonID id = {})
        : name(std::move(name)), id(std::move(id)) {}

    virtual ~Person() = default;

    virtual std::string getInfo() const {
        return name + " " + id.passport;
    }

    bool operator==(const Person& other) const {
        return name == other.name && id == other.id;
    }

    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        return os << "(" << p.name << ", " << p.id << ")";
    }
    bool operator!=(const Person& other) const {
    return !(*this == other);
}



};
