#ifndef BOOKINGAGENT_HPP
#define BOOKINGAGENT_HPP

#include "User.hpp"
#include <iostream>
#include "Flight.hpp"
#include "Reservation.hpp"
#include "Payment.hpp"
#include <vector>
#include <memory>  // For shared_ptr
#include "json.hpp"
#include <fstream>
#include <memory> // For shared_ptr

class BookingAgent : public User {
public:
    BookingAgent(std::string username, std::string password);
    
    void displayMenu();       // Displays Passenger menu
    void searchFlights();     // Allows user to search for flights
    void bookFlight();        // Handles booking process
    void viewAllReservations();  // Displays the passenger's reservations
    void checkIn();           // Allows check-in and prints boarding pass
    void cancelReservation(); // Allows removal of a reservation
    void viewSpecificReservations();
    void confirmCashPayment(); // Confirms cash payment at the airport

private:
    std::vector<std::shared_ptr<Reservation>> reservations; // Stores passenger reservations
    string generateRandomReservationID();
};

#endif

