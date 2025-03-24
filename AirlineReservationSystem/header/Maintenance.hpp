#ifndef MAINTENANCE_HPP
#define MAINTENANCE_HPP

#include <string>
#include <iostream>
#include "json.hpp"
#include <fstream>
#include <vector>
#include <map>

using namespace std;
using json = nlohmann::json;

#define PATH_OF_MAINTENANCE_DATA_BASE "data_base/maintenanceData.json"

class Maintenance {
private:
    string aircraftID;
    string maintenanceDetails;

public:
    // Constructor
    Maintenance(const string& id, const string& details);

    // Getter Methods
    string getAircraftID() const;
    string getMaintenanceDetails() const;

    // Setter Methods
    void setAircraftID(const string& id);
    void setMaintenanceDetails(const string& details);

    // Schedule Maintenance
    void scheduleMaintenance() const;

    // Log Maintenance
    void logMaintenance() const;

    // Convert to JSON
    json toJson() const;

    // Static method to load all maintenance data from a JSON file
    static void loadMaintenanceData(const string& filename, map<string, vector<Maintenance>>& maintenanceRecords);

    // Static method to save all maintenance data to a JSON file
    static void saveMaintenanceData(const string& filename, const map<string, vector<Maintenance>>& maintenanceRecords);

    // Static method to edit a specific maintenance record
    static bool editMaintenance(const string& filename, const string& aircraftID, const string& newDetails);

    // Static method to remove a specific maintenance record
    static bool removeMaintenance(const string& filename, const string& aircraftID);
};

#endif
