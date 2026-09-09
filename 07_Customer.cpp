#pragma once
#include <iostream>
#include <string>

// Customer Class: Represents a customer buying tickets
// OOP Concept: Encapsulation (private customer data with public getters)
class Customer {
private:
    std::string name;
    std::string phone;

public:
    Customer() : name("Guest"), phone("0000000000") {}
    Customer(std::string name, std::string phone) {
        this->name = name;
        this->phone = phone;
    }

    std::string getName() const { return this->name; }
    std::string getPhone() const { return this->phone; }
};
