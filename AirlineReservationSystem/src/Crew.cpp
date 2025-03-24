#include "../header/Crew.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>  // For remove_if

using json = nlohmann::json;

std::vector<Crew> allCrewMembers; // Static vector to store all loaded crew members


Crew::Crew(std::string name, std::string role)
    : name(name), role(role) {}

std::string Crew::getName() const {
    return name;
}

std::string Crew::getRole() const {
    return role;
}

std::vector<std::shared_ptr<Flight>> Crew::getAssignedFlights() const {
    return assignedFlights;
}

void Crew::assignFlight(std::shared_ptr<Flight> flight) {
    assignedFlights.push_back(flight);  // Assign a flight to the crew member
}

void Crew::removeFlight(const std::string& flightNumber) {
    assignedFlights.erase(std::remove_if(assignedFlights.begin(), assignedFlights.end(),
        [&](const std::shared_ptr<Flight>& flight) {
            return flight->getFlightNumber() == flightNumber;
        }), assignedFlights.end());
}

void Crew::displayCrewInfo() const {
    std::cout << "Crew Name: " << name << "\nRole: " << role << "\nAssigned Flights: ";
    for (const auto& flight : assignedFlights) {
        std::cout << flight->getFlightNumber() << " ";
    }
    std::cout << "\n";
}

// Save crew to file, appending data rather than overwriting
void Crew::saveCrewToFile(const std::string& filename) {
    json crewJson;
    std::ifstream file(filename);  // Open the file for reading
    if (file.is_open()) {
        file >> crewJson;  // Load existing crew data from the file
        file.close();
    }

    // Add the crew member's flight data
    json flightData;
    for (const auto& crewMember : assignedFlights) {
        flightData["flightNumber"] = crewMember->getFlightNumber();
        flightData["origin"] = crewMember->getOrigin();
        flightData["destination"] = crewMember->getDestination();
        flightData["departureTime"] = crewMember->getDepartureTime();
        flightData["arrivalTime"] = crewMember->getArrivalTime();
    }

    crewJson[name] = flightData;  // Add new crew member data to the JSON object

    // Save the updated crew data back to the file
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << std::setw(4) << crewJson << std::endl;  // Pretty print JSON
    } else {
        std::cerr << "Error: Unable to save crew data.\n";
    }
}


Crew* Crew::getCrewByName(const std::string& crewName) {
    // Ensure crew data is loaded
    if (allCrewMembers.empty()) {
        std::cerr << "No crew data loaded. Loading crew data now...\n";
        loadCrewFromFile(PATH_OF_CREW_DATA_BASE);  // Load crew data from file if not already loaded
    }

    // Search for the crew member by name
    for (auto& crew : allCrewMembers) {
        if (crew.getName() == crewName) {
            return &crew;  // Return pointer to crew member
        }
    }

    std::cerr << "Crew member " << crewName << " not found.\n";
    return nullptr;  // Return nullptr if crew member is not found
}



void Crew::loadCrewFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open crew data file.\n";
        return;
    }

    json crewJson;
    file >> crewJson;

    // Load the crew data from the file and store them in the static vector
    for (auto& element : crewJson.items()) {
        std::string crewName = element.key();
        auto flightData = element.value();  // Flight data for this crew member

        Crew crewMember(crewName, "Role Placeholder");  // Placeholder role for now

        // Parse flight data and assign flights to the crew member
        for (auto& flight : flightData) {
            std::string flightNumber = flight.contains("flightNumber") ? flight["flightNumber"].get<std::string>() : "Unknown";
            std::string origin = flight.contains("origin") ? flight["origin"].get<std::string>() : "Unknown";
            std::string destination = flight.contains("destination") ? flight["destination"].get<std::string>() : "Unknown";
            std::string departureTime = flight.contains("departureTime") ? flight["departureTime"].get<std::string>() : "Unknown";
            std::string arrivalTime = flight.contains("arrivalTime") ? flight["arrivalTime"].get<std::string>() : "Unknown";

            // Provide default values for missing fields
            std::string aircraftType = flight.contains("aircraftType") ? flight["aircraftType"].get<std::string>() : "Unknown";
            int totalSeats = flight.contains("totalSeats") ? flight["totalSeats"].get<int>() : 0;
            std::string status = flight.contains("status") ? flight["status"].get<std::string>() : "Unknown";
            std::string flightPrice = flight.contains("flightPrice") ? flight["flightPrice"].get<std::string>() : "0$";

            // Create the Flight object with either the provided data or the default values
            std::shared_ptr<Flight> flightObj = std::make_shared<Flight>(
                flightNumber, origin, destination, departureTime, arrivalTime, 
                aircraftType, totalSeats, status, flightPrice
            );

            crewMember.assignFlight(flightObj);  // Assign the created flight to the crew member
        }

        // Add to the global crew member list
        allCrewMembers.push_back(crewMember);
    }
}



// Method to remove a crew member entirely from the file
bool Crew::removeCrewFromFile(const std::string& crewName, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open crew data file.\n";
        return false;
    }

    json crewJson;
    file >> crewJson;

    if (crewJson.contains(crewName)) {
        crewJson.erase(crewName);  // Remove the crew member by name
    } else {
        std::cout << "Crew member " << crewName << " not found.\n";
        return false;
    }

    // Save the modified JSON back to the file
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << std::setw(4) << crewJson << std::endl;
    } else {
        std::cerr << "Error: Unable to save updated crew data.\n";
        return false;
    }

    return true;
}

// Update crew assignment in the file
void Crew::updateCrewInFile(const std::string& crewName, const std::shared_ptr<Flight>& flight, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open crew data file.\n";
        return;
    }

    json crewJson;
    file >> crewJson;

    // Find and update the crew member's assigned flights
    if (crewJson.contains(crewName)) {
        // Remove the existing flights and reassign them
        crewJson[crewName] = json::array();
        crewJson[crewName].push_back(flight->toJson());  // Add the updated flight data
    }

    // Save the updated crew data back to the file
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << std::setw(4) << crewJson << std::endl;
    } else {
        std::cerr << "Error: Unable to save updated crew data.\n";
    }
}
