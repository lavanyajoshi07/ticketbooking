#pragma once
#include <iostream>
#include <string>

// Payment Abstract Base Class: Defines payment contract
// OOP Concept: Abstraction (pure virtual method pay force implementation in derived classes)
class Payment {
public:
    virtual ~Payment() {}

    // Pure virtual method
    virtual bool pay(double amount) = 0;
    virtual std::string getMethodName() const = 0;
};
