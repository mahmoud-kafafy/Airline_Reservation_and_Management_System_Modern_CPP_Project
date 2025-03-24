#ifndef RESERVATION_HPP
#define RESERVATION_HPP

#include "Flight.hpp"
#include "json.hpp"
#include <string>
#include <vector>
#include <memory>  // Include for shared_ptr

#define PATH_OF_RESERVATION_DATA_BASE "data_base/reservation.json"
#define PATH_OF_BOOKING_AGENT_RESERVATION_DATA_BASE "data_base/bookingAgentReservation.json"

class Reservation {
public:
    // ************************** CONSTRUCTORS **************************
    Reservation(std::string reservationID, std::string passengerName, std::shared_ptr<Flight> flight, 
                std::string seatNumber, std::string paymentMethod, std::string paymentDetails);
    Reservation(); // Default constructor

    // ************************** GETTERS **************************
    std::string getReservationID() const;
    std::string getPassengerName() const;
    std::shared_ptr<Flight> getFlight() const;
    std::string getSeatNumber() const;
    std::string getPaymentMethod() const;
    std::string getPaymentDetails() const;

    // ************************** JSON CONVERSION **************************
    nlohmann::json toJson() const; // Converts reservation to JSON format
    static Reservation fromJson(const nlohmann::json& j); // Creates Reservation object from JSON

    // ************************** DISPLAY AND BOARDING PASS **************************
    void displayReservation() const; // Displays reservation details
    void printBoardingPass() const;  // Generates boarding pass for check-in
    void displayHoldingReservation() const; // Display reservation details when payment is on hold

    // ************************** FILE HANDLING FUNCTIONS **************************
    static void saveReservations(const std::vector<std::shared_ptr<Reservation>>& reservations); // Saves all reservations to a JSON file
    static std::vector<std::shared_ptr<Reservation>> loadReservations(); // Loads all reservations from a JSON file
    void addReservation(const Reservation& newReservation);
    static void removeReservation(const std::string& reservationID);
    static void saveBookingAgentReservations(const vector<std::shared_ptr<Reservation>>& reservations);
    void addBookingAgentReservation(const Reservation& newReservation);
    static vector<std::shared_ptr<Reservation>> loadBookingAgentReservations();




    // ************************** PAYMENT HANDLING **************************
    void setIsPaid(bool status); // Set payment status
    bool getIsPaid() const; // Get payment status
    void setPaymentMethod(const std::string& method); // Set payment method
    void setPaymentDetails(const std::string& details); // Set payment details

private:
    std::string reservationID;
    std::string passengerName;
    std::shared_ptr<Flight> flight;  // Changed to shared_ptr<Flight>
    std::string seatNumber;
    std::string paymentMethod;
    std::string paymentDetails;
    bool isPaid;  // Track whether the reservation is paid
};

#endif // RESERVATION_HPP
