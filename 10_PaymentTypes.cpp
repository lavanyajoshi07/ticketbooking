#pragma once
#include <iostream>
#include <string>
#include "09_Payment.cpp"

// OOP Concept: Inheritance (UpiPayment derived from Payment)
class UpiPayment : public Payment {
private:
    std::string upiId;
    bool shouldSucceed;

public:
    UpiPayment(std::string upiId, bool shouldSucceed = true) 
        : upiId(upiId), shouldSucceed(shouldSucceed) {}

    // OOP Concept: Runtime Polymorphism (overriding pure virtual pay method)
    bool pay(double amount) override {
        if (!shouldSucceed) {
            std::cout << "[UPI] Payment of Rs." << amount << " via UPI ID (" << upiId << ") FAILED (Simulated Error).\n";
            return false;
        }
        std::cout << "[UPI] Rs." << amount << " paid successfully via UPI ID: " << upiId << "\n";
        return true;
    }

    std::string getMethodName() const override { return "UPI (" + upiId + ")"; }
};

// OOP Concept: Inheritance (CardPayment derived from Payment)
class CardPayment : public Payment {
private:
    std::string cardNumber;
    bool shouldSucceed;

public:
    CardPayment(std::string cardNumber, bool shouldSucceed = true) 
        : cardNumber(cardNumber), shouldSucceed(shouldSucceed) {}

    // OOP Concept: Runtime Polymorphism
    bool pay(double amount) override {
        if (!shouldSucceed) {
            std::cout << "[CARD] Payment of Rs." << amount << " via Card ending in " 
                      << cardNumber.substr(cardNumber.length() > 4 ? cardNumber.length() - 4 : 0) 
                      << " FAILED (Insufficient Funds).\n";
            return false;
        }
        std::cout << "[CARD] Rs." << amount << " paid successfully using Card.\n";
        return true;
    }

    std::string getMethodName() const override { return "Card"; }
};

// OOP Concept: Inheritance (CashPayment derived from Payment)
class CashPayment : public Payment {
private:
    bool shouldSucceed;

public:
    CashPayment(bool shouldSucceed = true) : shouldSucceed(shouldSucceed) {}

    // OOP Concept: Runtime Polymorphism
    bool pay(double amount) override {
        if (!shouldSucceed) {
            std::cout << "[CASH] Cash payment of Rs." << amount << " FAILED (Counter Error).\n";
            return false;
        }
        std::cout << "[CASH] Rs." << amount << " paid in cash at counter successfully.\n";
        return true;
    }

    std::string getMethodName() const override { return "Cash"; }
};
