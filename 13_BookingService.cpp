#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "04_Cinema.cpp"
#include "05_Show.cpp"
#include "07_Customer.cpp"
#include "08_Booking.cpp"
#include "09_Payment.cpp"
#include "11_PriceCalculator.cpp"
#include "12_TicketPrinter.cpp"

// BookingService Class: Orchestrator that runs the booking flow end-to-end
// OOP Concept: Association (Customer interacts with BookingService to perform operations)
class BookingService {
private:
    Cinema cinema;
    std::vector<Movie> movies;
    std::vector<Show> shows;
    std::vector<Booking> bookings;
    PriceCalculator calculator;

public:
    BookingService(std::string cinemaName, std::string location) 
        : cinema(cinemaName, location) {}

    Cinema& getCinema() { return cinema; }

    void addMovie(const Movie& movie) {
        movies.push_back(movie);
    }

    const Movie* getMovieById(int id) const {
        for (const auto& m : movies) {
            if (m.getId() == id) {
                return &m;
            }
        }
        return nullptr;
    }

    void addShow(const Show& show) {
        shows.push_back(show);
    }

    void listMovies() const {
        std::cout << "\n===== MOVIES CURRENTLY PLAYING =====\n";
        if (movies.empty()) {
            std::cout << "No movies available.\n";
            return;
        }
        for (const auto& m : movies) {
            m.displayInfo();
        }
        std::cout << "=====================================\n";
    }

    void listShowsForMovie(int movieId) const {
        std::cout << "\n===== SHOWS FOR MOVIE ID " << movieId << " =====\n";
        bool found = false;
        for (const auto& s : shows) {
            if (s.getMovie() != nullptr && s.getMovie()->getId() == movieId) {
                found = true;
                std::cout << "Show ID: [" << s.getShowId() << "] | Screen-" 
                          << s.getScreen()->getScreenNumber() << " | Time: " 
                          << s.getStartTime() << "\n";
            }
        }
        if (!found) {
            std::cout << "No shows found for Movie ID " << movieId << ".\n";
        }
        std::cout << "======================================\n";
    }

    Show* getShowById(int showId) {
        for (auto& s : shows) {
            if (s.getShowId() == showId) {
                return &s;
            }
        }
        return nullptr;
    }

    Booking* findBooking(const std::string& bookingId) {
        for (auto& b : bookings) {
            if (b.getBookingId() == bookingId) {
                return &b;
            }
        }
        return nullptr;
    }

    // Core Booking Orchestration Flow (FR4, FR5, FR6, FR7)
    // Edge Case 1: Booking a seat already BOOKED -> rejected, nothing changes
    // Edge Case 2: Failed payment -> booking NOT confirmed, seats released
    bool bookSeats(int showId, const Customer& customer, const std::vector<std::string>& seatNumbers, Payment* paymentMethod, const TicketPrinter& printer) {
        Show* show = getShowById(showId);
        if (show == nullptr) {
            std::cout << "Error: Show ID " << showId << " not found.\n";
            return false;
        }

        if (seatNumbers.empty()) {
            std::cout << "Error: No seats selected.\n";
            return false;
        }

        std::vector<ShowSeat*> selectedSeats;

        // Step 1: Validate existence and availability of ALL selected seats
        for (const auto& sNo : seatNumbers) {
            ShowSeat* ss = show->getShowSeatByNumber(sNo);
            if (ss == nullptr) {
                std::cout << "Error: Seat '" << sNo << "' does not exist for this show.\n";
                return false;
            }
            // EDGE CASE 1: Reject if ANY seat is already BOOKED
            if (!ss->isAvailable()) {
                std::cout << "\n[BOOKING REJECTED] Seat '" << sNo 
                          << "' is already BOOKED! No seats were changed.\n";
                return false;
            }
            selectedSeats.push_back(ss);
        }

        // Step 2: Calculate total price
        double totalAmount = calculator.calculateTotal(selectedSeats);

        std::cout << "\nSelected Seats: ";
        for (size_t i = 0; i < selectedSeats.size(); ++i) {
            std::cout << selectedSeats[i]->getSeatNumber() 
                      << " (" << Seat::getTypeName(selectedSeats[i]->getType()) 
                      << " Rs." << selectedSeats[i]->getPrice() << ")"
                      << (i + 1 < selectedSeats.size() ? ", " : "");
        }
        std::cout << "\nTOTAL AMOUNT: Rs." << totalAmount << "\n\n";

        // Step 3: Process Payment (Runtime Polymorphism dynamic dispatch)
        if (paymentMethod == nullptr) {
            std::cout << "Error: Invalid payment method.\n";
            return false;
        }

        std::cout << "Processing Payment via " << paymentMethod->getMethodName() << "...\n";
        bool paymentSuccess = paymentMethod->pay(totalAmount);

        // EDGE CASE 2: Payment failed -> seats released, booking NOT confirmed
        if (!paymentSuccess) {
            std::cout << "\n[BOOKING FAILED] Payment was unsuccessful. Seats remain AVAILABLE.\n";
            return false;
        }

        // Step 4: Lock seats now that payment succeeded
        for (auto* ss : selectedSeats) {
            ss->bookSeat();
        }

        // Step 5: Create Booking record
        Booking newBooking(show, customer, selectedSeats, totalAmount, paymentMethod->getMethodName());
        bookings.push_back(newBooking);

        // Step 6: Print ticket
        printer.printTicket(bookings.back());
        return true;
    }

    // EDGE CASE 3: Cancel a booking -> seats show AVAILABLE again (FR8)
    bool cancelBooking(const std::string& bookingId) {
        Booking* booking = findBooking(bookingId);
        if (booking == nullptr) {
            std::cout << "Error: Booking ID '" << bookingId << "' not found.\n";
            return false;
        }

        if (booking->getStatus() == BookingStatus::CANCELLED) {
            std::cout << "Notice: Booking ID '" << bookingId << "' is already CANCELLED.\n";
            return false;
        }

        // Release seats and update status to CANCELLED
        booking->cancelBooking();
        std::cout << "\n[SUCCESS] Booking ID " << bookingId << " has been CANCELLED.\n";
        std::cout << "All associated seats are now AVAILABLE again!\n\n";
        return true;
    }

    void listAllBookings(const TicketPrinter& printer) const {
        std::cout << "\n===== ALL SYSTEM BOOKINGS =====\n";
        if (bookings.empty()) {
            std::cout << "No bookings made yet.\n";
            return;
        }
        for (const auto& b : bookings) {
            printer.printTicket(b);
        }
    }
};
