#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "05_Show.cpp"
#include "07_Customer.cpp"
#include "09_Payment.cpp"

enum class BookingStatus {
    CONFIRMED,
    FAILED,
    CANCELLED
};

// Booking Class: Tracks booking transaction state
// OOP Concept: Static Members (static variable nextBookingId shared by all instances to generate unique IDs)
// OOP Concept: Aggregation (Booking holds references to Show, Customer, and ShowSeats)
class Booking {
private:
    // OOP Concept: Static Members
    static int nextBookingId;

    std::string bookingId;
    const Show* show;
    Customer customer;
    std::vector<ShowSeat*> bookedSeats;
    double totalAmount;
    BookingStatus status;
    std::string paymentMethodName;

public:
    Booking() : bookingId(""), show(nullptr), totalAmount(0.0), status(BookingStatus::FAILED) {}

    Booking(const Show* show, Customer customer, std::vector<ShowSeat*> seats, double totalAmount, std::string paymentMethodName) {
        // Generate unique booking ID (e.g. BK1001)
        this->bookingId = "BK" + std::to_string(nextBookingId++);
        this->show = show;
        this->customer = customer;
        this->bookedSeats = seats;
        this->totalAmount = totalAmount;
        this->status = BookingStatus::CONFIRMED;
        this->paymentMethodName = paymentMethodName;
    }

    std::string getBookingId() const { return this->bookingId; }
    const Show* getShow() const { return this->show; }
    Customer getCustomer() const { return this->customer; }
    const std::vector<ShowSeat*>& getBookedSeats() const { return this->bookedSeats; }
    double getTotalAmount() const { return this->totalAmount; }
    BookingStatus getStatus() const { return this->status; }
    std::string getPaymentMethodName() const { return this->paymentMethodName; }

    std::string getStatusString() const {
        switch (status) {
            case BookingStatus::CONFIRMED: return "CONFIRMED";
            case BookingStatus::FAILED: return "FAILED";
            case BookingStatus::CANCELLED: return "CANCELLED";
            default: return "UNKNOWN";
        }
    }

    void cancelBooking() {
        if (status == BookingStatus::CONFIRMED) {
            this->status = BookingStatus::CANCELLED;
            // Release the seats back to AVAILABLE
            for (auto* ss : bookedSeats) {
                if (ss != nullptr) {
                    ss->cancelSeat();
                }
            }
        }
    }
};

// Initialize static member
int Booking::nextBookingId = 1001;
