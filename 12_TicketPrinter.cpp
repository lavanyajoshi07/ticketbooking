#pragma once
#include <iostream>
#include <string>
#include "08_Booking.cpp"

// TicketPrinter Class: Formats and outputs formatted tickets
// Single Responsibility: Formatting and outputting tickets ONLY
class TicketPrinter {
public:
    void printTicket(const Booking& booking) const {
        std::cout << "\n================ TICKET ================\n";
        std::cout << " Booking ID : " << booking.getBookingId() << "\n";
        if (booking.getShow() != nullptr && booking.getShow()->getMovie() != nullptr) {
            std::cout << " Movie      : " << booking.getShow()->getMovie()->getTitle() << "\n";
            std::cout << " Screen     : Screen-" << booking.getShow()->getScreen()->getScreenNumber()
                      << "  " << booking.getShow()->getStartTime() << "\n";
        }
        std::cout << " Seats      : ";
        const auto& seats = booking.getBookedSeats();
        for (size_t i = 0; i < seats.size(); ++i) {
            std::cout << seats[i]->getSeatNumber() << (i + 1 < seats.size() ? ", " : "");
        }
        std::cout << "\n";
        std::cout << " Amount     : Rs." << booking.getTotalAmount() << "\n";
        std::cout << " Payment    : " << booking.getPaymentMethodName() << "\n";
        std::cout << " Status     : " << booking.getStatusString() << "\n";
        std::cout << "========================================\n\n";
    }
};
