#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "02_Seat.cpp"

// Screen Class: Represents an auditorium screen in the cinema
// OOP Concept: Composition (Screen contains and owns a collection of physical Seat objects)
class Screen {
private:
    int screenNumber;
    std::string screenName;
    std::vector<Seat> physicalSeats;

public:
    Screen(int screenNumber, std::string screenName) {
        this->screenNumber = screenNumber;
        this->screenName = screenName;
    }

    int getScreenNumber() const { return this->screenNumber; }
    std::string getScreenName() const { return this->screenName; }
    const std::vector<Seat>& getPhysicalSeats() const { return this->physicalSeats; }

    void addSeat(const Seat& seat) {
        this->physicalSeats.push_back(seat);
    }

    const Seat* findSeat(const std::string& seatNum) const {
        for (const auto& seat : physicalSeats) {
            if (seat.getSeatNumber() == seatNum) {
                return &seat;
            }
        }
        return nullptr;
    }
};
