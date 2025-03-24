#include "../header/Reservation.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
using json = nlohmann::json;

// ************************** CONSTRUCTORS **************************
Reservation::Reservation(std::string reservationID, std::string passengerName, std::shared_ptr<Flight> flight, 
                         std::string seatNumber, std::string paymentMethod, std::string paymentDetails)
    : reservationID(reservationID), passengerName(passengerName), flight(flight), 
      seatNumber(seatNumber), paymentMethod(paymentMethod), paymentDetails(paymentDetails), isPaid(false) {}

Reservation::Reservation() : reservationID(""), passengerName(""), seatNumber(""), paymentMethod(""), paymentDetails(""), isPaid(false) {}

// ************************** GETTERS **************************
string Reservation::getReservationID() const {
    return reservationID;
}

string Reservation::getPassengerName() const {
    return passengerName;
}

std::shared_ptr<Flight> Reservation::getFlight() const {
    return flight;
}

string Reservation::getSeatNumber() const {
    return seatNumber;
}

string Reservation::getPaymentMethod() const {
    return paymentMethod;
}

string Reservation::getPaymentDetails() const {
    return paymentDetails;
}

// ************************** JSON CONVERSION FUNCTIONS **************************
json Reservation::toJson() const {
    return {
        {"reservationID", reservationID},
        {"passengerName", passengerName},
        {"flight", flight->toJson()},
        {"seatNumber", seatNumber},
        {"paymentMethod", paymentMethod},
        {"paymentDetails", paymentDetails},
        {"isPaid", isPaid}
    };
}

Reservation Reservation::fromJson(const json& j) {
    return Reservation(
        j.at("reservationID").get<string>(),
        j.at("passengerName").get<string>(),
        make_shared<Flight>(Flight::fromJson(j.at("flight"))),
        j.at("seatNumber").get<string>(),
        j.at("paymentMethod").get<string>(),
        j.at("paymentDetails").get<string>()
    );
}

// ************************** DISPLAY RESERVATION **************************
void Reservation::displayReservation() const {
    cout << "\n--- Reservation Details ---\n";
    cout << "Reservation ID : " << reservationID << endl;
    cout << "Passenger Name : " << passengerName << endl;
    cout << "Seat Number    : " << seatNumber << endl;
    cout << "Payment Method : " << paymentMethod << endl;
    cout << "Flight Details : \n";
    flight->displayFlightInfo();
}

void Reservation::displayHoldingReservation() const {
    cout << "\n--- Reservation Details ---\n";
    cout << "Reservation ID : " << reservationID << endl;
    cout<<"Payment is on hold. Please complete the payment to confirm the reservation.\n";
    cout << "Passenger Name : " << passengerName << endl;
    cout << "Seat Number    : " << seatNumber << endl;
    cout << "Payment Method : " << paymentMethod << endl;
    cout << "Flight Details : \n";
    flight->displayFlightInfo();
}
// ************************** PRINT BOARDING PASS **************************
void Reservation::printBoardingPass() const {
    //cout << "\nCheck-In Successful!\n";
    cout << "Boarding Pass:\n";
    cout << "-----------------------------\n";
    cout << "Reservation ID: " << reservationID << endl;
    cout << "Passenger: " << passengerName << endl;
    cout << "Flight: " << flight->getFlightNumber() << endl;
    cout << "Origin: " << flight->getOrigin() << endl;
    cout << "Destination: " << flight->getDestination() << endl;
    cout << "Departure: " << flight->getDepartureTime() << endl;
    cout << "Seat: " << seatNumber << endl;
    cout << "-----------------------------\n";
}

// ************************** ADD RESERVATION **************************
// ************************** REMOVE RESERVATION **************************
void Reservation::addReservation(const Reservation& newReservation) {
    vector<std::shared_ptr<Reservation>> reservations = loadReservations();
    reservations.push_back(make_shared<Reservation>(newReservation));
    saveReservations(reservations);
    cout << "Reservation added successfully!" << endl;
}

void Reservation::addBookingAgentReservation(const Reservation& newReservation) {
    vector<std::shared_ptr<Reservation>> reservations = loadBookingAgentReservations();
    reservations.push_back(make_shared<Reservation>(newReservation));
    saveBookingAgentReservations(reservations);
    cout << "Reservation added successfully!" << endl;
}

void Reservation::removeReservation(const std::string& reservationID) {
    vector<std::shared_ptr<Reservation>> reservations = loadReservations();
    auto it = find_if(reservations.begin(), reservations.end(), [&reservationID](const std::shared_ptr<Reservation>& reservation) {
        return reservation->getReservationID() == reservationID;
    });

    if (it != reservations.end()) {
        reservations.erase(it);
        saveReservations(reservations);
        cout << "Reservation " << reservationID << " removed successfully!" << endl;
    } else {
        cout << "Reservation ID " << reservationID << " not found!" << endl;
    }
}

// ************************** SAVE RESERVATIONS **************************
// ************************** SAVE RESERVATIONS **************************
// Save reservations to the file
void Reservation::saveReservations(const vector<std::shared_ptr<Reservation>>& reservations) {
    // Load existing reservations data from the file
    ifstream file(PATH_OF_RESERVATION_DATA_BASE);
    json allReservations;

    if (file.is_open()) {
        file >> allReservations;  // Load current reservations from the file
        file.close();
    }

    // Check if the reservation already exists
    for (const auto& res : reservations) {
        bool exists = false;
        
        // Check if the reservation already exists for the passenger
        if (allReservations.contains(res->getPassengerName())) {
            auto& userReservations = allReservations[res->getPassengerName()];
            for (const auto& existingReservation : userReservations) {
                if (existingReservation["reservationID"] == res->getReservationID()) {
                    exists = true;  // Duplicate reservation found
                    break;
                }
            }
        }

        // If not, add the new reservation for the passenger
        if (!exists) {
            allReservations[res->getPassengerName()].push_back(res->toJson());
        }
    }

    // Save the updated reservations back to the file
    ofstream outFile(PATH_OF_RESERVATION_DATA_BASE);
    if (!outFile.is_open()) {
        cerr << "Error: Unable to save reservations.\n";
        return;
    }

    outFile << setw(4) << allReservations << endl;  // Save to the file in a pretty format
}


void Reservation::saveBookingAgentReservations(const vector<std::shared_ptr<Reservation>>& reservations) {
    // Load existing reservations data from the file
    ifstream file(PATH_OF_BOOKING_AGENT_RESERVATION_DATA_BASE);
    json allReservations;

    if (file.is_open()) {
        file >> allReservations;  // Load current reservations from the file
        file.close();
    }

    // Check if the reservation already exists
    for (const auto& res : reservations) {
        bool exists = false;
        
        // Check if the reservation already exists for the passenger
        if (allReservations.contains(res->getPassengerName())) {
            auto& userReservations = allReservations[res->getPassengerName()];
            for (const auto& existingReservation : userReservations) {
                if (existingReservation["reservationID"] == res->getReservationID()) {
                    exists = true;  // Duplicate reservation found
                    break;
                }
            }
        }

        // If not, add the new reservation for the passenger
        if (!exists) {
            allReservations[res->getPassengerName()].push_back(res->toJson());
        }
    }

    // Save the updated reservations back to the file
    ofstream outFile(PATH_OF_BOOKING_AGENT_RESERVATION_DATA_BASE);
    if (!outFile.is_open()) {
        cerr << "Error: Unable to save reservations.\n";
        return;
    }

    outFile << setw(4) << allReservations << endl;  // Save to the file in a pretty format
}


// ************************** LOAD RESERVATIONS **************************
vector<std::shared_ptr<Reservation>> Reservation::loadReservations() {
    vector<std::shared_ptr<Reservation>> reservations;
    ifstream file(PATH_OF_RESERVATION_DATA_BASE); // Open the file containing reservation data

    if (!file.is_open()) {
        cerr << "No reservation data found. Starting fresh.\n";
        return reservations; // Return an empty list if no file is found
    }

    json reservationsJson;
    file >> reservationsJson;  // Load the data from the file
    file.close();

    // Iterate over all reservations and load them into the reservations vector
    for (const auto& user : reservationsJson.items()) {
        const auto& userReservations = user.value();  // This will be an array of reservations

        for (const auto& resJson : userReservations) {
            // Create a reservation object from the JSON data
            auto reservation = make_shared<Reservation>(Reservation::fromJson(resJson));

            // Check if the "isPaid" attribute exists and update it
            if (resJson.contains("isPaid")) {
                bool isPaid = resJson["isPaid"];
                reservation->setIsPaid(isPaid);  // Set the payment status
            }

            reservations.push_back(reservation);  // Add the reservation to the list
        }
    }

    return reservations;  // Return the loaded reservations
}

vector<std::shared_ptr<Reservation>> Reservation::loadBookingAgentReservations() {
    vector<std::shared_ptr<Reservation>> reservations;
    ifstream file(PATH_OF_BOOKING_AGENT_RESERVATION_DATA_BASE); // Open the file containing reservation data

    if (!file.is_open()) {
        cerr << "No reservation data found. Starting fresh.\n";
        return reservations; // Return an empty list if no file is found
    }

    json reservationsJson;
    file >> reservationsJson;  // Load the data from the file
    file.close();

    // Iterate over all reservations and load them into the reservations vector
    for (const auto& user : reservationsJson.items()) {
        const auto& userReservations = user.value();  // This will be an array of reservations

        for (const auto& resJson : userReservations) {
            // Create a reservation object from the JSON data
            auto reservation = make_shared<Reservation>(Reservation::fromJson(resJson));

            // Check if the "isPaid" attribute exists and update it
            if (resJson.contains("isPaid")) {
                bool isPaid = resJson["isPaid"];
                reservation->setIsPaid(isPaid);  // Set the payment status
            }

            reservations.push_back(reservation);  // Add the reservation to the list
        }
    }

    return reservations;  // Return the loaded reservations
}




// ************************** PAYMENT HANDLING **************************
void Reservation::setIsPaid(bool status) { isPaid = status; }
bool Reservation::getIsPaid() const { return isPaid; }

void Reservation::setPaymentMethod(const string& method) { paymentMethod = method; }
void Reservation::setPaymentDetails(const string& details) { paymentDetails = details; }
