#pragma once
#include <iostream>
#include <vector>
#include "06_ShowSeat.cpp"

// PriceCalculator Class: Responsible for calculating ticket costs
// OOP Concept: Compile-Time Polymorphism (overloaded calculateTotal methods)
class PriceCalculator {
public:
    // Overload 1: Calculate price for a list of ShowSeat pointers
    double calculateTotal(const std::vector<ShowSeat*>& selectedSeats) const {
        double total = 0.0;
        for (const auto* ss : selectedSeats) {
            if (ss != nullptr) {
                total += ss->getPrice();
            }
        }
        return total;
    }

    // Overload 2: Calculate price for a single seat type and count
    double calculateTotal(SeatType type, int count) const {
        return Seat::getBasePrice(type) * count;
    }

    // Overload 3: Calculate price with a flat discount percentage
    double calculateTotal(const std::vector<ShowSeat*>& selectedSeats, double discountPercentage) const {
        double subtotal = calculateTotal(selectedSeats);
        return subtotal * (1.0 - (discountPercentage / 100.0));
    }
};
