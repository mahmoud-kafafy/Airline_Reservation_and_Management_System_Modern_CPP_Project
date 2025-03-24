#ifndef AIRCRAFT_HPP
#define AIRCRAFT_HPP

#include <string>
#include <iostream>
#include <memory>  // To handle pointers
#include "Maintenance.hpp"  // Include Maintenance class

#define PATH_OF_AIR_CRAFT_DATA_BASE "data_base/aircraftDataBase.json"

using namespace std;

class Aircraft {
private:
    string aircraftID;
    string aircraftType;
    int capacity;
    shared_ptr<Maintenance> maintenance;  // Pointer to Maintenance object

public:
    // Constructor
     // Default constructor (added this)
    Aircraft() : aircraftID(""), aircraftType(""), capacity(0), maintenance(nullptr) {}
    Aircraft(const string& id, const string& type, int cap, shared_ptr<Maintenance> maint);

    // Getter and Setter Methods
    string getAircraftID() const;
    string getAircraftType() const;
    int getCapacity() const;
    shared_ptr<Maintenance> getMaintenance() const;  // To access the maintenance object

    void setAircraftID(const string& id);
    void setAircraftType(const string& type);
    void setCapacity(int cap);
    void setMaintenance(shared_ptr<Maintenance> maint);

    // Display Aircraft Information
    void displayAircraftInfo() const;

    // Convert to JSON
    json toJson() const;

    // Static method to load aircraft data from a JSON file
    static void loadAircraftData(const string& filename, map<string, Aircraft>& aircraftRecords);

    // Static method to save aircraft data to a JSON file
    static void saveAircraftData(const string& filename, const map<string, Aircraft>& aircraftRecords);

    // Static method to edit an aircraft's details
    static bool editAircraft(const string& filename, const string& aircraftID, const string& fieldToUpdate, const string& newValue);

    // Static method to delete an aircraft from the file
    static bool deleteAircraft(const string& filename, const string& aircraftID);
};

#endif
