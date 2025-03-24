// Flight.cpp
#include "../header/Flight.hpp"  // Include the Flight class header file
#include "../header/json.hpp"    // Include the JSON library for handling JSON objects
#include <iostream>              // For console input/output operations (cout, cerr)
#include <string>                // For using string data type
#include <fstream>               // For reading and writing files

using namespace std;
using json = nlohmann::json;  // Define alias for JSON from the nlohmann library

//hna al load byrg3 vector of flights wna sa3tha 22dr a3dl fyh w a3ml kol haga w arg3 aktbo tany 
//hna save file bt5ly kol mra load w ab3t vector of flights w arg3 aktbo tany

// ************************** CONSTRUCTORS **************************

// Constructor to initialize a flight object with given details
Flight::Flight(const string& flightNumber, const string& origin, const string& destination,
  const string& departureTime, const string& arrivalTime,
  const string& aircraftType, int totalSeats, const string& status, const string& flightPrice)
    : flightNumber(flightNumber), origin(origin), destination(destination),
      departureTime(departureTime), arrivalTime(arrivalTime),
      aircraftType(aircraftType), totalSeats(totalSeats), status(status),flightPrice(flightPrice) {}

// Default constructor initializes an empty flight with default values
Flight::Flight() : flightNumber(""), origin(""), destination(""),
                   departureTime(""), arrivalTime(""),
                   aircraftType(""), totalSeats(0), status("") {}

// ************************** DISPLAY FUNCTION **************************

// Function to display flight information to the console
void Flight::displayFlightInfo() const {
    cout << "Flight Number : " << flightNumber << endl;
    cout << "Origin        : " << origin << endl;
    cout << "Destination   : " << destination << endl;
    cout << "Departure Time: " << departureTime << endl;
    cout << "Arrival Time  : " << arrivalTime << endl;
    cout << "Aircraft Type : " << aircraftType << endl;
    cout << "Total Seats   : " << totalSeats << endl;
    cout << "Flight Status : " << status << endl;
    cout << "Flight Price : " << flightPrice << endl;
    cout << "------------------------------------" << endl;
}

// ************************** JSON CONVERSION FUNCTIONS **************************

// Converts a Flight object into a JSON object for storage or transmission
json Flight::toJson() const {
    return {
        {"flightNumber", flightNumber},
        {"origin", origin},
        {"destination", destination},
        {"departureTime", departureTime},
        {"arrivalTime", arrivalTime},
        {"aircraftType", aircraftType},
        {"totalSeats", totalSeats},
        {"status", status},
        {"price", flightPrice}
    };
}

// Converts a JSON object back into a Flight object
Flight Flight::fromJson(const json& j) {
    return Flight(
        j.at("flightNumber").get<string>(),   // Extract flight number from JSON
        j.at("origin").get<string>(),         // Extract origin
        j.at("destination").get<string>(),    // Extract destination
        j.at("departureTime").get<string>(),  // Extract departure time
        j.at("arrivalTime").get<string>(),    // Extract arrival time
        j.at("aircraftType").get<string>(),   // Extract aircraft Type
        j.at("totalSeats").get<int>(),        // Extract total seats
        j.at("status").get<string>(),          // Extract flight status
        j.at("price").get<string>()          // Extract flight status
    );
}

// ************************** FILE HANDLING FUNCTIONS **************************

// Saves a list of flights to a JSON file
void Flight::saveFlights(const vector<Flight>& flights, const string& filename) {
    json flightsArray = json::array();  // Create an empty JSON array to store flights
    //each time make empty array which make each time override

    // Convert each Flight object to JSON and add it to the array
    for (const Flight& flight : flights) {
        flightsArray.push_back(flight.toJson());
    }

    // Open the file for writing
    ofstream file(filename);
    if (!file.is_open()) {  // Check if file opening failed
        cerr << "Error: Cannot open file " << filename << " for writing.\n";
        return;  // Exit function if file couldn't be opened
    }

    // Write the formatted JSON array to the file
    file << setw(4) << flightsArray << endl;  // setw(4) ensures pretty formatting
}

// Loads a list of flights from a JSON file
vector<Flight> Flight::loadFlights(const string& filename) {
    vector<Flight> flights;  // Create a vector to store loaded flights
    ifstream file(filename); // Open the file for reading

    if (!file.is_open()) {  // Check if file opening failed
        cerr << "No existing flights data found at " << filename << ".\n";
        return flights;  // Return empty vector if file doesn't exist
    }

    json flightsJson;  
    file >> flightsJson;  // Read JSON data from the file

    // Convert each JSON object into a Flight object and add it to the list
    for (const auto& flightJson : flightsJson) {
        flights.push_back(Flight::fromJson(flightJson));
    }

    return flights;  // Return the list of flights
}

void Flight::addFlightToFile(const Flight& newFlight, const string& filename) {
    vector<Flight> flights = loadFlights(filename); // Load existing flights

    flights.push_back(newFlight); // Add the new flight to the list

    saveFlights(flights, filename); // Save the updated list back to file

    cout << "Flight added successfully to " << filename << "!" << endl;
}

void Flight::removeFlightFromFile(const string& flightNumber, const string& filename) {
    vector<Flight> flights = loadFlights(filename); // Load existing flights
    bool found = false;

    // Create a new list excluding the flight to be removed
    vector<Flight> updatedFlights;
    for (const Flight& flight : flights) {
        if (flight.getFlightNumber() != flightNumber) {
            updatedFlights.push_back(flight);
        } else {
            found = true; // Mark that flight was found and removed
        }
    }

    // Check if the flight was found and removed
    if (!found) {
        cout << "Flight with number " << flightNumber << " not found in " << filename << "." << endl;
        return;
    }

    // Save the updated list back to the file
    saveFlights(updatedFlights, filename);
    cout << "Flight " << flightNumber << " removed successfully from " << filename << "!" << endl;
}


void Flight::updateFlightInFile(const string& flightNumber, const string& fieldToUpdate, const string& newValue, const string& filename) {
    vector<Flight> flights = loadFlights(filename); // Load existing flights
    bool found = false;

    // Iterate over flights to find and update the required flight
    for (Flight& flight : flights) {
        if (flight.getFlightNumber() == flightNumber) {
            found = true;

            // Update the specific field based on user input
            if (fieldToUpdate == "departureTime") {
                flight.setDepartureTime(newValue);
            } else if (fieldToUpdate == "arrivalTime") {
                flight.setArrivalTime(newValue);
            } else if (fieldToUpdate == "status") {
                flight.setStatus(newValue);
            } else if (fieldToUpdate == "origin") {
                flight.setOrigin(newValue);
            } else if (fieldToUpdate == "destination") {
                flight.setDestination(newValue);
            } else if (fieldToUpdate == "aircraftType") {
                flight.setAircraftType(newValue);
            }else if (fieldToUpdate == "flightPrice")
            {
                flight.setflightPrice(newValue);
            } else {
                cout << "Invalid field name: " << fieldToUpdate << endl;
                return;
            }

            break; // Stop searching after finding the flight
        }
    }

    // If flight was not found, display a message
    if (!found) {
        cout << "Flight with number " << flightNumber << " not found in " << filename << "." << endl;
        return;
    }

    // Save the updated list back to the file
    saveFlights(flights, filename);
    cout << "Flight " << flightNumber << " updated successfully in " << filename << "!" << endl;
}

// Setter for Flight Number
void Flight::setFlightNumber(const string& flightNumber) {
    this->flightNumber = flightNumber;
}

// Setter for Origin
void Flight::setOrigin(const string& origin) {
    this->origin = origin;
}

// Setter for Destination
void Flight::setDestination(const string& destination) {
    this->destination = destination;
}

// Setter for Departure Time
void Flight::setDepartureTime(const string& departureTime) {
    this->departureTime = departureTime;
}

// Setter for Arrival Time
void Flight::setArrivalTime(const string& arrivalTime) {
    this->arrivalTime = arrivalTime;
}

// Setter for Aircraft Type
void Flight::setAircraftType(const string& aircraftType) {
    this->aircraftType = aircraftType;
}

// Setter for Total Seats
void Flight::setTotalSeats(int totalSeats) {
    this->totalSeats = totalSeats;
}

// Setter for Flight Status
void Flight::setStatus(const string& status) {
    this->status = status;
}

void Flight::setflightPrice(const string& price)
{
    this->flightPrice = price;
}

// Getter for Flight Number
string Flight::getFlightNumber() const {
    return flightNumber;
}

string Flight::getflightPrice() const
{
    return flightPrice;
}

// Getter for Origin
string Flight::getOrigin() const {
    return origin;
}

// Getter for Destination
string Flight::getDestination() const {
    return destination;
}

// Getter for Departure Time
string Flight::getDepartureTime() const {
    return departureTime;
}

// Getter for Arrival Time
string Flight::getArrivalTime() const {
    return arrivalTime;
}

// Getter for Aircraft Type
string Flight::getAircraftType() const {
    return aircraftType;
}

// Getter for Total Seats
int Flight::getTotalSeats() const {
    return totalSeats;
}

// Getter for Flight Status
string Flight::getStatus() const {
    return status;
}
