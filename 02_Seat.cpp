#pragma once
#include <iostream>
#include <string>

enum class SeatType {
    SILVER,
    GOLD,
    PLATINUM
};

// Seat Class: Represents a physical seat in a screen
// OOP Concept: Encapsulation (controlled data access via getters)
class Seat {
private:
    std::string seatNumber; // e.g. "A1", "B2"
    SeatType type;

public:
    // OOP Concept: this Keyword
    Seat(std::string seatNumber, SeatType type) {
        this->seatNumber = seatNumber;
        this->type = type;
    }

    std::string getSeatNumber() const { return this->seatNumber; }
    SeatType getType() const { return this->type; }

    static std::string getTypeName(SeatType type) {
        switch (type) {
            case SeatType::SILVER: return "SILVER";
            case SeatType::GOLD: return "GOLD";
            case SeatType::PLATINUM: return "PLATINUM";
            default: return "UNKNOWN";
        }
    }

    static double getBasePrice(SeatType type) {
        switch (type) {
            case SeatType::SILVER: return 150.0;
            case SeatType::GOLD: return 250.0;
            case SeatType::PLATINUM: return 400.0;
            default: return 0.0;
        }
    }

    double getBasePrice() const {
        return getBasePrice(this->type);
    }
};
