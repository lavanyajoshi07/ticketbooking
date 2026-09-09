#pragma once
#include <iostream>
#include <string>
#include "02_Seat.cpp"

enum class SeatStatus {
    AVAILABLE,
    BOOKED
};

// ShowSeat Class: Status of ONE seat FOR ONE show
// OOP Concept: Encapsulation (keep seatStatus private and modify only through validation methods bookSeat and cancelSeat)
class ShowSeat {
private:
    Seat seat;
    SeatStatus status;

public:
    ShowSeat(Seat seat) : seat(seat), status(SeatStatus::AVAILABLE) {}

    std::string getSeatNumber() const { return seat.getSeatNumber(); }
    SeatType getType() const { return seat.getType(); }
    double getPrice() const { return seat.getBasePrice(); }
    SeatStatus getStatus() const { return this->status; }
    bool isAvailable() const { return this->status == SeatStatus::AVAILABLE; }

    // Validation method for booking a seat
    bool bookSeat() {
        if (this->status == SeatStatus::BOOKED) {
            return false; // Already booked, operation fails
        }
        this->status = SeatStatus::BOOKED;
        return true;
    }

    // Validation method for cancelling a seat booking
    void cancelSeat() {
        this->status = SeatStatus::AVAILABLE;
    }
};
