#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "01_Movie.cpp"
#include "03_Screen.cpp"
#include "06_ShowSeat.cpp"

// Show Class: Represents a movie screening on a specific screen at a specific time
// OOP Concept: Aggregation (Show stores references to existing Movie & Screen objects)
// OOP Concept: Composition (Show creates and owns its ShowSeats status collection)
class Show {
private:
    int showId;
    const Movie* movie;   // Aggregation (borrows Movie reference)
    const Screen* screen; // Aggregation (borrows Screen reference)
    std::string startTime;
    std::vector<ShowSeat> showSeats; // Composition (owns ShowSeats)

public:
    Show(int showId, const Movie* movie, const Screen* screen, std::string startTime) {
        this->showId = showId;
        this->movie = movie;
        this->screen = screen;
        this->startTime = startTime;

        // Initialize show seats from screen physical seats
        if (screen != nullptr) {
            for (const auto& physicalSeat : screen->getPhysicalSeats()) {
                this->showSeats.push_back(ShowSeat(physicalSeat));
            }
        }
    }

    int getShowId() const { return this->showId; }
    const Movie* getMovie() const { return this->movie; }
    const Screen* getScreen() const { return this->screen; }
    std::string getStartTime() const { return this->startTime; }
    std::vector<ShowSeat>& getShowSeats() { return this->showSeats; }
    const std::vector<ShowSeat>& getShowSeats() const { return this->showSeats; }

    ShowSeat* getShowSeatByNumber(const std::string& seatNum) {
        for (auto& ss : showSeats) {
            if (ss.getSeatNumber() == seatNum) {
                return &ss;
            }
        }
        return nullptr;
    }

    // Displays seat layout showing AVAILABLE ([ ]) and BOOKED ([X]) status
    void displaySeatLayout() const {
        std::cout << "\nSCREEN-" << screen->getScreenNumber() << "  " << startTime 
                  << " | " << movie->getTitle() << "\n";
        std::cout << "--------------------------------------------------\n";

        // Group seats by category for display
        std::vector<std::string> categories = {"SILVER", "GOLD", "PLATINUM"};
        for (const auto& cat : categories) {
            std::cout << std::left << std::setw(10) << cat << " ";
            for (const auto& ss : showSeats) {
                if (Seat::getTypeName(ss.getType()) == cat) {
                    std::cout << ss.getSeatNumber() 
                              << (ss.isAvailable() ? "[ ] " : "[X] ");
                }
            }
            std::cout << "\n";
        }
        std::cout << "--------------------------------------------------\n";
        std::cout << "( [ ] = available    [X] = booked )\n\n";
    }
};
