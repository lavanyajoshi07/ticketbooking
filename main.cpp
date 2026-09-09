#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>

// Include modular entity and service classes in topological dependency order
// Course Rule: One class per file, No header files
#include "01_Movie.cpp"
#include "02_Seat.cpp"
#include "03_Screen.cpp"
#include "04_Cinema.cpp"
#include "05_Show.cpp"
#include "06_ShowSeat.cpp"
#include "07_Customer.cpp"
#include "08_Booking.cpp"
#include "09_Payment.cpp"
#include "10_PaymentTypes.cpp"
#include "11_PriceCalculator.cpp"
#include "12_TicketPrinter.cpp"
#include "13_BookingService.cpp"

// Helper function to split comma-separated seat strings (e.g. "A1,B2")
std::vector<std::string> parseSeatInput(const std::string& input) {
    std::vector<std::string> seats;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, ',')) {
        // Trim whitespace
        item.erase(0, item.find_first_not_of(" \t"));
        item.erase(item.find_last_not_of(" \t") + 1);
        if (!item.empty()) {
            seats.push_back(item);
        }
    }
    return seats;
}

// Populate sample cinema, screens, movies, and shows
void setupCinemaData(BookingService& service) {
    // Add Screens & Physical Seats
    Screen screen1(1, "Audi-1");
    screen1.addSeat(Seat("A1", SeatType::SILVER));
    screen1.addSeat(Seat("A2", SeatType::SILVER));
    screen1.addSeat(Seat("A3", SeatType::SILVER));
    screen1.addSeat(Seat("A4", SeatType::SILVER));
    screen1.addSeat(Seat("B1", SeatType::GOLD));
    screen1.addSeat(Seat("B2", SeatType::GOLD));
    screen1.addSeat(Seat("B3", SeatType::GOLD));
    screen1.addSeat(Seat("C1", SeatType::PLATINUM));
    screen1.addSeat(Seat("C2", SeatType::PLATINUM));

    Screen screen2(2, "Audi-2");
    screen2.addSeat(Seat("A1", SeatType::SILVER));
    screen2.addSeat(Seat("A2", SeatType::SILVER));
    screen2.addSeat(Seat("B1", SeatType::GOLD));
    screen2.addSeat(Seat("B2", SeatType::GOLD));
    screen2.addSeat(Seat("C1", SeatType::PLATINUM));

    service.getCinema().addScreen(screen1);
    service.getCinema().addScreen(screen2);

    // Add Movies
    Movie m1(1, "3 Idiots", "Hindi", 170);
    Movie m2(2, "Interstellar", "English", 169);
    service.addMovie(m1);
    service.addMovie(m2);

    // Add Shows (Aggregates pointers to existing Movies and Screens)
    Screen* sc1Ptr = service.getCinema().getScreenByNumber(1);
    Screen* sc2Ptr = service.getCinema().getScreenByNumber(2);
    const Movie* m1Ptr = service.getMovieById(1);
    const Movie* m2Ptr = service.getMovieById(2);

    // Show 1: 3 Idiots at 06:00 PM on Screen-1
    service.addShow(Show(1, m1Ptr, sc1Ptr, "06:00 PM"));
    // Show 2: Interstellar at 09:00 PM on Screen-2
    service.addShow(Show(2, m2Ptr, sc2Ptr, "09:00 PM"));
}

// Automated Demonstration of Required Edge Cases
void runAutomatedDemo(BookingService& service, TicketPrinter& printer) {
    std::cout << "\n=======================================================\n";
    std::cout << "       AUTOMATED DEMONSTRATION OF ALL EDGE CASES        \n";
    std::cout << "=======================================================\n";

    Customer demoUser("Rahul Sharma", "9876543210");

    // Test 1: Successful Booking
    std::cout << "\n--- DEMO 1: Successful Booking (A1, B2) via UPI ---\n";
    UpiPayment upiSuccess("rahul@upi", true);
    service.bookSeats(1, demoUser, {"A1", "B2"}, &upiSuccess, printer);

    // Test Edge Case 1: Booking a seat already BOOKED -> rejected
    std::cout << "\n--- DEMO EDGE CASE 1: Attempting to book A1 again (Already Booked) ---\n";
    UpiPayment upi2("user2@upi", true);
    service.bookSeats(1, demoUser, {"A1", "A2"}, &upi2, printer);

    // Test Edge Case 2: Failed Payment -> seats released, booking NOT confirmed
    std::cout << "\n--- DEMO EDGE CASE 2: Booking B1 with Failed Card Payment ---\n";
    CardPayment cardFail("4111222233334444", false); // Simulated failure
    service.bookSeats(1, demoUser, {"B1"}, &cardFail, printer);

    // Verify B1 is still AVAILABLE
    Show* show1 = service.getShowById(1);
    if (show1) {
        ShowSeat* b1 = show1->getShowSeatByNumber("B1");
        std::cout << "Verification: Seat B1 status after failed payment is " 
                  << (b1 && b1->isAvailable() ? "AVAILABLE (Success)" : "BOOKED (Bug!)") << "\n";
    }

    // Test Edge Case 3: Cancelling a booking -> seats become AVAILABLE again
    std::cout << "\n--- DEMO EDGE CASE 3: Cancelling BK1001 (A1, B2) ---\n";
    service.cancelBooking("BK1001");

    // Verify A1 and B2 are now AVAILABLE again
    if (show1) {
        std::cout << "Seat Layout after Cancellation:\n";
        show1->displaySeatLayout();
    }

    // Test Edge Case 4: Invalid Seat / Input Validation
    std::cout << "\n--- DEMO EDGE CASE 4: Invalid Seat Code ('Z99') ---\n";
    service.bookSeats(1, demoUser, {"Z99"}, &upiSuccess, printer);

    std::cout << "=======================================================\n";
    std::cout << "            END OF AUTOMATED DEMONSTRATION             \n";
    std::cout << "=======================================================\n\n";
}

int main() {
    BookingService service("PVR Cinemas", "Central Mall");
    TicketPrinter printer;
    setupCinemaData(service);

    std::cout << "=======================================================\n";
    std::cout << "    WELCOME TO MOVIE TICKET BOOKING SYSTEM (INOX/PVR)  \n";
    std::cout << "=======================================================\n";

    while (true) {
        std::cout << "\n===== MAIN MENU =====\n";
        std::cout << "1. View Movies & Schedule\n";
        std::cout << "2. View Seat Layout for a Show\n";
        std::cout << "3. Book Seats\n";
        std::cout << "4. Cancel Booking\n";
        std::cout << "5. View All Tickets\n";
        std::cout << "6. Run Automated Edge-Case Demo\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose an option (0-6): ";

        int choice = -1;
        if (!(std::cin >> choice)) {
            // EDGE CASE 4: Invalid input handling (non-integer menu choice)
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n[ERROR] Invalid choice! Please enter a valid number (0-6).\n";
            continue;
        }

        if (choice == 0) {
            std::cout << "\nThank you for using Movie Ticket Booking System. Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1: {
                service.listMovies();
                std::cout << "\nEnter Movie ID to view shows (or 0 to return): ";
                int mId;
                if (std::cin >> mId && mId > 0) {
                    service.listShowsForMovie(mId);
                }
                break;
            }
            case 2: {
                std::cout << "Enter Show ID: ";
                int sId;
                if (std::cin >> sId) {
                    Show* show = service.getShowById(sId);
                    if (show) {
                        show->displaySeatLayout();
                    } else {
                        std::cout << "[ERROR] Show ID " << sId << " not found!\n";
                    }
                }
                break;
            }
            case 3: {
                std::cout << "\n--- BOOK TICKETS ---\n";
                std::cout << "Enter Show ID: ";
                int sId;
                if (!(std::cin >> sId)) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "[ERROR] Invalid Show ID.\n";
                    break;
                }

                Show* show = service.getShowById(sId);
                if (!show) {
                    std::cout << "[ERROR] Show ID not found.\n";
                    break;
                }

                // Display current seat layout
                show->displaySeatLayout();

                std::cout << "Enter Customer Name: ";
                std::string custName;
                std::cin.ignore();
                std::getline(std::cin, custName);

                std::cout << "Enter Customer Phone: ";
                std::string custPhone;
                std::getline(std::cin, custPhone);
                Customer cust(custName, custPhone);

                std::cout << "Enter Seat Numbers separated by comma (e.g. A1,B2): ";
                std::string seatInput;
                std::getline(std::cin, seatInput);
                std::vector<std::string> selectedSeats = parseSeatInput(seatInput);

                if (selectedSeats.empty()) {
                    std::cout << "[ERROR] No valid seats specified.\n";
                    break;
                }

                std::cout << "\nSelect Payment Method:\n";
                std::cout << "1. UPI\n";
                std::cout << "2. Card\n";
                std::cout << "3. Cash\n";
                std::cout << "Choose payment method (1-3): ";
                int payChoice;
                std::cin >> payChoice;

                std::unique_ptr<Payment> paymentObj = nullptr;
                if (payChoice == 1) {
                    std::cout << "Enter UPI ID: ";
                    std::string upi;
                    std::cin >> upi;
                    paymentObj = std::make_unique<UpiPayment>(upi);
                } else if (payChoice == 2) {
                    std::cout << "Enter Card Number: ";
                    std::string card;
                    std::cin >> card;
                    paymentObj = std::make_unique<CardPayment>(card);
                } else if (payChoice == 3) {
                    paymentObj = std::make_unique<CashPayment>();
                } else {
                    std::cout << "[ERROR] Invalid payment choice!\n";
                    break;
                }

                // Execute booking flow
                service.bookSeats(sId, cust, selectedSeats, paymentObj.get(), printer);
                break;
            }
            case 4: {
                std::cout << "\n--- CANCEL BOOKING ---\n";
                std::cout << "Enter Booking ID to cancel (e.g. BK1001): ";
                std::string bId;
                std::cin >> bId;
                service.cancelBooking(bId);
                break;
            }
            case 5: {
                service.listAllBookings(printer);
                break;
            }
            case 6: {
                runAutomatedDemo(service, printer);
                break;
            }
            default: {
                std::cout << "\n[ERROR] Invalid option selected! Please choose between 0 and 6.\n";
                break;
            }
        }
    }

    return 0;
}
