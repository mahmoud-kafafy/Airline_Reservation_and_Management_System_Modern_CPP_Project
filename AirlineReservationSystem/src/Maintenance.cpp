#include "../header/Maintenance.hpp"
#include <iostream>


using namespace std;
using json = nlohmann::json;

// Constructor
Maintenance::Maintenance(const string& id, const string& details)
    : aircraftID(id), maintenanceDetails(details) {}

// Getter and Setter Methods
string Maintenance::getAircraftID() const { return aircraftID; }
string Maintenance::getMaintenanceDetails() const { return maintenanceDetails; }

void Maintenance::setAircraftID(const string& id) { aircraftID = id; }
void Maintenance::setMaintenanceDetails(const string& details) { maintenanceDetails = details; }

// Log Maintenance
void Maintenance::logMaintenance() const {
    cout << "Maintenance log for aircraft " << aircraftID << " - " << maintenanceDetails << endl;
}

// Convert Maintenance to JSON
json Maintenance::toJson() const {
    return {
        {"aircraftID", aircraftID},
        {"maintenanceDetails", maintenanceDetails}
    };
}

// Static method to load maintenance data from a JSON file
void Maintenance::loadMaintenanceData(const string& filename, map<string, vector<Maintenance>>& maintenanceRecords) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open maintenance data file.\n";
        return;
    }

    json maintenanceJson;
    file >> maintenanceJson;
    file.close();

    // Parse the JSON file and populate the maintenance records
    for (auto& element : maintenanceJson.items()) {
        string aircraftID = element.key();
        auto maintenanceData = element.value();

        // Create maintenance objects and add to the map
        vector<Maintenance> maintenanceList;
        for (const auto& data : maintenanceData) {
            string details = data["maintenanceDetails"];
            maintenanceList.push_back(Maintenance(aircraftID, details));
        }
        maintenanceRecords[aircraftID] = maintenanceList;
    }
}

// Static method to save all maintenance data to a JSON file
void Maintenance::saveMaintenanceData(const string& filename, const map<string, vector<Maintenance>>& maintenanceRecords) {
    json maintenanceJson;

    // Convert maintenance data to JSON
    for (const auto& record : maintenanceRecords) {
        json maintenanceArray;
        for (const auto& maintenance : record.second) {
            maintenanceArray.push_back(maintenance.toJson());
        }
        maintenanceJson[record.first] = maintenanceArray;
    }

    // Save to the file
    ofstream file(filename);
    if (file.is_open()) {
        file << setw(4) << maintenanceJson << endl;  // Pretty print JSON
    } else {
        cerr << "Error: Unable to open file for saving maintenance data.\n";
    }
}
