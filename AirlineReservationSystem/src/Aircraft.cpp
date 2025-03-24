#include "../header/Aircraft.hpp"

using namespace std;
using json = nlohmann::json;


//mo3zm fkrt al class any b load mn json f map <string,aircraft> w 22dr a3dl w a8yr kol haga w save tany 

// Constructor
Aircraft::Aircraft(const string& id, const string& type, int cap, shared_ptr<Maintenance> maint)
    : aircraftID(id), aircraftType(type), capacity(cap), maintenance(maint) {}

// Getter and Setter Methods
string Aircraft::getAircraftID() const { return aircraftID; }
string Aircraft::getAircraftType() const { return aircraftType; }
int Aircraft::getCapacity() const { return capacity; }
shared_ptr<Maintenance> Aircraft::getMaintenance() const { return maintenance; }

void Aircraft::setAircraftID(const string& id) { aircraftID = id; }
void Aircraft::setAircraftType(const string& type) { aircraftType = type; }
void Aircraft::setCapacity(int cap) { capacity = cap; }
void Aircraft::setMaintenance(shared_ptr<Maintenance> maint) { maintenance = maint; }

// Display Aircraft Information
void Aircraft::displayAircraftInfo() const {
    cout << "Aircraft ID: " << aircraftID << endl;
    cout << "Aircraft Type: " << aircraftType << endl;
    cout << "Capacity: " << capacity << endl;
    if (maintenance) {
        maintenance->logMaintenance();  // Display maintenance log if maintenance is set
    } else {
        cout << "No maintenance data available.\n";
    }
}

// Convert Aircraft to JSON
json Aircraft::toJson() const {
    json maintenanceJson;
    if (maintenance) {
        maintenanceJson = maintenance->toJson();
    }

    return {
        {"aircraftID", aircraftID},
        {"aircraftType", aircraftType},
        {"capacity", capacity},
        {"maintenance", maintenanceJson}  // Include maintenance details in the JSON
    };
}

// Static method to load aircraft data from a JSON file
void Aircraft::loadAircraftData(const string& filename, map<string, Aircraft>& aircraftRecords) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open aircraft data file.\n";
        return;
    }

    json aircraftJson;
    try {
        file >> aircraftJson;
    } catch (const json::parse_error& e) {
        cerr << "Error parsing JSON: " << e.what() << endl;
        return;
    }

    file.close();

    // Parse the JSON file and populate the aircraft records
    for (auto& element : aircraftJson.items()) {
        string aircraftID = element.key();
        auto aircraftData = element.value();

        // Check if required fields exist
        if (!aircraftData.contains("aircraftType") || !aircraftData.contains("capacity")) {
            cerr << "Error: Missing required fields in the data for aircraft " << aircraftID << ".\n";
            continue;  // Skip this aircraft if necessary fields are missing
        }

        // Extract maintenance details and create a Maintenance object
        shared_ptr<Maintenance> maint = nullptr;
        if (aircraftData.contains("maintenance") && !aircraftData["maintenance"].is_null()) {
            auto maintenanceData = aircraftData["maintenance"];
            if (maintenanceData.contains("maintenanceDetails") && !maintenanceData["maintenanceDetails"].is_null()) {
                maint = make_shared<Maintenance>(
                    maintenanceData["aircraftID"],
                    maintenanceData["maintenanceDetails"]
                );
            } else {
                cerr << "Warning: Maintenance details are missing for aircraft " << aircraftID << ".\n";
            }
        }

        // Create Aircraft objects and add them to the map
        Aircraft aircraft(
            aircraftID,
            aircraftData["aircraftType"],
            aircraftData["capacity"],
            maint
        );

        aircraftRecords[aircraftID] = aircraft;
    }
}


// Static method to save aircraft data to a JSON file
void Aircraft::saveAircraftData(const string& filename, const map<string, Aircraft>& aircraftRecords) {
    json aircraftJson;

    // Convert aircraft data to JSON
    for (const auto& record : aircraftRecords) {
        aircraftJson[record.first] = record.second.toJson();
    }

    // Save to the file
    ofstream file(filename);
    if (file.is_open()) {
        file << setw(4) << aircraftJson << endl;  // Pretty print JSON
    } else {
        cerr << "Error: Unable to open file for saving aircraft data.\n";
    }
}

// Static method to edit an aircraft's details
bool Aircraft::editAircraft(const string& filename, const string& aircraftID, const string& fieldToUpdate, const string& newValue) {
    map<string, Aircraft> aircraftRecords;
    //load have key and value is aircraft object
    loadAircraftData(filename, aircraftRecords);
    cout<<"new value: "<<newValue<<endl;

    if (aircraftRecords.find(aircraftID) == aircraftRecords.end()) {
        cout << "Aircraft ID not found.\n";
        return false;
    }

    // Modify the specific field based on user input
    //return aircraft object
    Aircraft& aircraft = aircraftRecords[aircraftID];

    if (fieldToUpdate == "aircraftType") {
        if (newValue.empty()) {
            cout << "Error: aircraftType cannot be empty.\n";
            return false;
        }
        aircraft.setAircraftType(newValue);
    } else if (fieldToUpdate == "capacity") {
        try {
            int newCapacity = stoi(newValue);  // Convert to int
            aircraft.setCapacity(newCapacity);
        } catch (const invalid_argument& e) {
            cout << "Error: Invalid value for capacity. Please enter a valid number.\n";
            return false;
        }
    } else if (fieldToUpdate == "maintenanceStatus") {
        // Assuming we modify the maintenance status field
        shared_ptr<Maintenance> maint = aircraft.getMaintenance();
        if (maint) {
            maint->setMaintenanceDetails(newValue);
        } else {
            cout << "Error: Maintenance data not found for aircraft " << aircraftID << ".\n";
            return false;
        }
    } else {
        cout << "Invalid field name: " << fieldToUpdate << endl;
        return false;
    }

    // Save the updated data back to the file
    //save the update
    saveAircraftData(filename, aircraftRecords);
    //cout << "Aircraft " << aircraftID << " updated successfully.\n";
    return true;
}


// Static method to delete an aircraft from the file
bool Aircraft::deleteAircraft(const string& filename, const string& aircraftID) {
    map<string, Aircraft> aircraftRecords;
    loadAircraftData(filename, aircraftRecords);

    if (aircraftRecords.find(aircraftID) == aircraftRecords.end()) {
        cout << "Aircraft ID not found.\n";
        return false;
    }

    // Remove the aircraft record
    aircraftRecords.erase(aircraftID);

    // Save the updated data back to the file
    saveAircraftData(filename, aircraftRecords);
    //cout << "Aircraft " << aircraftID << " removed successfully.\n";
    return true;
}

