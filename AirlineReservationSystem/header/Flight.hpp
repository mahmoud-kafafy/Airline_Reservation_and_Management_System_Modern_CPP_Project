// Flight.hpp
#ifndef FLIGHT_HPP
#define FLIGHT_HPP

#include <iostream>
#include "json.hpp"


using json = nlohmann::json;
using namespace std;

#define PATH_OF_FLIGHTS_DATA_BASE "data_base/flights.json"


class Flight {
private:
    string flightNumber;
    string origin;
    string destination;
    string departureTime;
    string arrivalTime;
    string aircraftType;
    int totalSeats;
    string status;
    string flightPrice;

    // Setter functions to modify specific attributes
    void setFlightNumber(const string& flightNumber);
    void setOrigin(const string& origin);
    void setDestination(const string& destination);
    void setDepartureTime(const string& departureTime);
    void setArrivalTime(const string& arrivalTime);
    void setAircraftType(const string& aircraftType);
    void setTotalSeats(int totalSeats);
    void setStatus(const string& status);
    void setflightPrice(const string& price);
    

public:
    // Constructor (initializes all data members clearly)
    Flight(const string& flightNumber, const string& origin, const string& destination,
           const string& departureTime, const string& arrivalTime,
           const string& aircraftType, int totalSeats, const string& status, const string& price);

    // Default constructor
    Flight();

    // Display flight information
    void displayFlightInfo() const;

    // Convert flight data into JSON format
    json toJson() const;

    // Create a flight object from JSON data
    static Flight fromJson(const json& j);

    // Clearly saves all flights to a JSON file
    static void saveFlights(const vector<Flight>& flights, const string& filename);

    // Clearly loads all flights from a JSON file
    static vector<Flight> loadFlights(const string& filename); 

    static void addFlightToFile(const Flight& newFlight, const string& filename);

    static void removeFlightFromFile(const string& flightNumber, const string& filename);

    static void updateFlightInFile(const string& flightNumber, const string& fieldToUpdate, const string& newValue, const string& filename);

    // Getter functions to retrieve specific attributes
    string getFlightNumber() const;
    string getOrigin() const;
    string getDestination() const;
    string getDepartureTime() const;
    string getArrivalTime() const;
    string getAircraftType() const;
    string getflightPrice() const;
    int getTotalSeats() const;
    string getStatus() const;


};

#endif
