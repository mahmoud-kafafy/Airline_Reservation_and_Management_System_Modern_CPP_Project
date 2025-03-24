#ifndef CREW_HPP
#define CREW_HPP

#include <string>
#include <vector>
#include <memory>  // For shared_ptr
#include "Flight.hpp"
#include "json.hpp"

#define PATH_OF_CREW_DATA_BASE "data_base/crewData.json"

class Crew {
public:
    Crew(std::string name, std::string role);

    std::string getName() const;
    std::string getRole() const;
    std::vector<std::shared_ptr<Flight>> getAssignedFlights() const;
    void assignFlight(std::shared_ptr<Flight> flight);  // Assign a flight to this crew member
    void removeFlight(const std::string& flightNumber);  // Remove the flight assignment by flight number

    void displayCrewInfo() const;  // Display crew member details
    void saveCrewToFile(const std::string& filename);  // Save crew data to a JSON file (appending)
    static void loadCrewFromFile(const std::string& filename);  // Load crew data from a JSON file
    static bool removeCrewFromFile(const std::string& crewName, const std::string& filename);  // Remove crew from JSON file
    static void updateCrewInFile(const std::string& crewName, const std::shared_ptr<Flight>& flight, const std::string& filename);  // Update flight assignments for crew
    static Crew* getCrewByName(const std::string& crewName);


private:
    std::string name;
    std::string role;
    std::vector<std::shared_ptr<Flight>> assignedFlights;  // Store the flights assigned to this crew member
};

#endif // CREW_HPP
