#include "../header/Administrator.hpp"
#include <iostream>
#include <vector>
#include <fstream>


std::vector<Flight> flights; // Store flights in a vector (Can later be replaced with file storage)


Administrator::Administrator(std::string username, std::string password)
    : User(username, password, "Administrator") {
         // Load flights from file clearly upon administrator login
        flights = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);
    }

void Administrator::displayMenu() {
    int choice;
    do {
        std::cout << "\n\t\t\t-------- Administrator Menu --------" << std::endl;
        std::cout << "1. Manage Flights" << std::endl;
        std::cout << "2. Manage Aircraft" << std::endl;
        std::cout << "3. Manage Users" << std::endl;
        std::cout << "4. Generate Reports" << std::endl;
        std::cout << "5. Logout" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: manageFlights(); break;
            case 2: manageAircraft(); break;
            case 4: generateOperationalReport(); break; 
            case 5: std::cout << "Logging out...\n"; break;
            default: std::cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 5);
}

void Administrator::manageFlights() {
    int choice;
    do {
        std::cout << "\n\t\t\t-------- Manage Flights --------" << std::endl;
        std::cout << "1. Add New Flight" << std::endl;
        std::cout << "2. Update Existing Flight" << std::endl;
        std::cout << "3. Remove Flight" << std::endl;
        std::cout << "4. View All Flights" << std::endl;
        std::cout << "5. Assign Crew Team" << std::endl;
        std::cout << "6. Back to Main Menu" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: addNewFlight(); break;
            case 2: updateFlight(); break;
            case 3: removeFlight(); break;
            case 4: viewAllFlights(); break;
            case 5: assignCrew();  // Method to assign crew to a flight return;
            case 6: return;
            default: std::cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 5);
}

void Administrator::addNewFlight() {
    std::string flightNumber, origin, destination, departureTime, arrivalTime, aircraftType, status, flightPrice;
    int totalSeats;

    std::cout << "\n\t\t\t-------- Add New Flight --------\n";
    std::cout << "Enter Flight Number: ";
    std::cin >> flightNumber;
    std::cout << "Enter Origin: ";
    std::cin.ignore();
    std::getline(std::cin, origin);
    std::cout << "Enter Destination: ";
    std::getline(std::cin, destination);
    std::cout << "Enter Departure Date and Time (YYYY-MM-DD HH:MM): ";
    std::getline(std::cin, departureTime);
    std::cout << "Enter Arrival Date and Time (YYYY-MM-DD HH:MM): ";
    std::getline(std::cin, arrivalTime);
    //////////////////////////////////////////////////////////////////////////
    // Load all aircrafts and display them to the user
    map<string, Aircraft> aircraftRecords;
    Aircraft::loadAircraftData(PATH_OF_AIR_CRAFT_DATA_BASE, aircraftRecords);  // Load aircraft data from the file

    if (aircraftRecords.empty()) {
        std::cout << "No available aircrafts in the database.\n";
        return;  // Exit if no aircrafts are available
    }

    // Display available aircrafts
    std::cout << "\n\t\t\t-------- Available Aircrafts --------\n";
    int counter = 1;
    for (const auto& aircraftPair : aircraftRecords) {
        std::cout << counter++ << ". ";
        aircraftPair.second.displayAircraftInfo();
        std::cout << "------------------------------------" << std::endl;
    }

    // Ask the user to choose an aircraft
    size_t aircraftChoice;
    std::cout << "Enter the number of the aircraft you want to use: ";
    std::cin >> aircraftChoice;

    if (aircraftChoice < 1 || aircraftChoice > aircraftRecords.size()) {
        std::cout << "Invalid choice. Please select a valid aircraft number.\n";
        return;
    }

    // Get the selected aircraft ID
    auto selectedAircraftIt = std::next(aircraftRecords.begin(), aircraftChoice - 1);
    aircraftType = selectedAircraftIt->second.getAircraftType();  // Get the aircraft type from the selected aircraft
    //////////////////////////////////////////////////////////////////////////
    std::cout << "Enter Total Seats: ";
    std::cin >> totalSeats;
    std::cout << "Enter Flight Price in $ (eg. 20$): ";
    std::cin.ignore();
    std::getline(std::cin, flightPrice);
    // Ask the user to enter a status (1, 2, or 3)
    std::cout << "Enter Status (1-Scheduled / 2-Delayed / 3-Canceled): ";
    int statusInput;
    std::cin >> statusInput;

    // Map the input to the appropriate status string
    if (statusInput == 1) {
        status = "Scheduled";
    } else if (statusInput == 2) {
        status = "Delayed";
    } else if (statusInput == 3) {
        status = "Canceled";
    } else {
        std::cout << "Invalid status input. Setting to 'Scheduled' by default.\n";
        status = "Scheduled";  // Default status if input is invalid
    }

    Flight newFlight(flightNumber, origin, destination, departureTime, arrivalTime,
        aircraftType, totalSeats, status, flightPrice);

    //flights.push_back(newFlight);
    Flight::addFlightToFile(newFlight, PATH_OF_FLIGHTS_DATA_BASE); // clearly saves immediately
    
    std::cout << "\nFlight " << flightNumber << " that is from "<< origin <<" to "<< destination <<" at "<< departureTime <<" has been successfully added to the schedule.\n";
}


void Administrator::viewAllFlights() {
    // Load flights from the Flight class' static method
    flights = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);

    cout << "\n\t\t\t-------- All Flights --------\n";
    
    if (flights.empty()) {
        cout << "No flights available.\n";
        return;
    }

    // Display each flight
    for (const Flight& flight : flights) {
        flight.displayFlightInfo();
    }
}


void Administrator::updateFlight() {
    // Load and display available flights
    flights = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);

    if (flights.empty()) {
        std::cout << "\nNo flights available to update.\n";
        return;
    }

    std::cout << "\n\t\t\t-------- Available Flights --------\n";
    for (const Flight& flight : flights) {
        flight.displayFlightInfo();
    }

    // Get user input for the flight number to update
    std::string flightNumberToUpdate;
    std::cout << "\nEnter the flight number you want to update: ";
    std::cin >> flightNumberToUpdate;

    // Display available fields with numbers
    std::cout << "\nWhich field do you want to update? Choose a number:\n";
    std::cout << "1. Departure Time\n";
    std::cout << "2. Arrival Time\n";
    std::cout << "3. Status\n";
    std::cout << "4. Origin\n";
    std::cout << "5. Destination\n";
    std::cout << "6. Aircraft Type\n";
    std::cout << "7. Total Seats\n";
    std::cout << "8. Flight Price\n";
    
    int fieldChoice;
    std::cout << "Enter your choice (1-8): ";
    std::cin >> fieldChoice;

    // Define field name based on choice
    std::string fieldToUpdate;
    switch (fieldChoice) {
        case 1: fieldToUpdate = "departureTime"; break;
        case 2: fieldToUpdate = "arrivalTime"; break;
        case 3: fieldToUpdate = "status"; break;
        case 4: fieldToUpdate = "origin"; break;
        case 5: fieldToUpdate = "destination"; break;
        case 6: fieldToUpdate = "aircraftType"; break;
        case 7: fieldToUpdate = "totalSeats"; break;
        case 8: fieldToUpdate = "flightPrice"; break;
        default:
            std::cout << "Invalid selection! Please enter a number between 1-7.\n";
            return;
    }

    // Get the new value for the selected field
    std::string newValue;
    if (fieldToUpdate == "totalSeats") {
        int newSeats;
        std::cout << "Enter new value for Total Seats: ";
        std::cin >> newSeats;
        newValue = std::to_string(newSeats);
    } else {
        std::cout << "Enter new value for " << fieldToUpdate << ": ";
        std::cin.ignore();
        std::getline(std::cin, newValue);
    }

    // Call the function to update the flight in the database
    Flight::updateFlightInFile(flightNumberToUpdate, fieldToUpdate, newValue, PATH_OF_FLIGHTS_DATA_BASE);

    std::cout << "\nFlight " << flightNumberToUpdate << " updated successfully!\n";
}



void Administrator::removeFlight() {
    // Load and display available flights
    flights = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);

    if (flights.empty()) {
        std::cout << "\nNo flights available to remove.\n";
        return;
    }

    std::cout << "\n\t\t\t-------- Available Flights --------\n";
    for (const Flight& flight : flights) {
        flight.displayFlightInfo();
    }

    // Get user input for the flight number to remove
    std::string flightNumberToRemove;
    std::cout << "\nEnter the flight number you want to remove: ";
    std::cin >> flightNumberToRemove;

    // Call Flight::removeFlightFromFile to handle removal
    Flight::removeFlightFromFile(flightNumberToRemove, PATH_OF_FLIGHTS_DATA_BASE);
}


void Administrator::generateOperationalReport() {
    // Ask for the month and year for the report
    string monthYear;
    cout << "\nEnter Month and Year for Report (MM-YYYY): ";
    cin >> monthYear;  // Get input from the user for the month and year of the report

    // Extract the month and year from the user input
    stringstream ss(monthYear);
    string month, year;
    getline(ss, month, '-');  // Extract the month from the input
    getline(ss, year);  // Extract the year from the input

    // Variables to store the report data
    int totalFlightsScheduled = 0;  // Total flights scheduled for the month
    int flightsCompleted = 0;  // Flights that have been completed (status: Scheduled)
    int flightsDelayed = 0;  // Flights that are delayed
    int flightsCanceled = 0;  // Flights that are canceled
    int totalReservationsMade = 0;  // Total reservations made for the flights
    double totalRevenue = 0.0;  // Total revenue generated from the flights

    // Detailed Flight Performance Data
    map<string, double> flightRevenue;  // To track revenue by flight number
    map<string, int> flightReservations;  // To track reservations count for each flight

    // Loop through all the flights and calculate required data
    for (const Flight& flight : flights) {
        // Count the scheduled flights
        totalFlightsScheduled++;

        // Simulate conditions for completed, delayed, or canceled flights
        if (flight.getStatus() == "Scheduled") {
            flightsCompleted++;  // Increment the count for completed flights
        } else if (flight.getStatus() == "Delayed") {
            flightsDelayed++;  // Increment the count for delayed flights
        } else if (flight.getStatus() == "Canceled") {
            flightsCanceled++;  // Increment the count for canceled flights
        }

        // Get the number of reservations made for the flight (get actual reservations)
        vector<std::shared_ptr<Reservation>> reservations = Reservation::loadReservations();
        int reservationsForFlight = 0;
        for (const auto& reservation : reservations) {
            if (reservation->getFlight()->getFlightNumber() == flight.getFlightNumber()) {
                reservationsForFlight++;  // Count the reservations for this specific flight
            }
        }

        // Track total revenue for the flight (price * number of reservations)
        double flightPrice = std::stod(flight.getflightPrice());  // Convert flight price from string to double
        double revenue = reservationsForFlight * flightPrice;      // Calculate the revenue for the flight

        totalRevenue += revenue;  // Add the revenue from this flight to the total revenue
        flightRevenue[flight.getFlightNumber()] = revenue;  // Store the revenue for the flight in the map
        flightReservations[flight.getFlightNumber()] = reservationsForFlight;  // Store the reservation count

        // Update the total reservations made
        totalReservationsMade += reservationsForFlight;  // Add the number of reservations to the total
    }

    // Generate the Report
    cout << "\nGenerating Operational Report for " << monthYear << "...\n\n";
    cout << "Report Summary:\n";
    cout << "- Total Flights Scheduled: " << totalFlightsScheduled << endl;  // Display the total number of scheduled flights
    cout << "- Flights Completed: " << flightsCompleted << endl;  // Display the number of completed flights
    cout << "- Flights Delayed: " << flightsDelayed << endl;  // Display the number of delayed flights
    cout << "- Flights Canceled: " << flightsCanceled << endl;  // Display the number of canceled flights
    cout << "- Total Reservations Made: " << totalReservationsMade << endl;  // Display the total number of reservations
    cout << "- Total Revenue: $" << fixed << setprecision(2) << totalRevenue << endl;  // Display the total revenue with 2 decimal points

    // Detailed Flight Performance:
    cout << "\nDetailed Flight Performance:\n";
    int count = 1;
    for (const auto& entry : flightRevenue) {
        cout << count++ << ". Flight " << entry.first << ": ";  // Display flight number and index in the report

        // Display reservations and revenue for each flight
        cout << "Reservations: " << flightReservations[entry.first] << ", Revenue: $" << entry.second << endl;
    }
    cout << "\nReport generated successfully!\n";  // Indicate that the report has been generated
}

void Administrator::assignCrew() {
    int choice;
    do {
        std::cout << "\n\t\t\t-------- Crew Management --------" << std::endl;
        std::cout << "1. Assign new crew member to a flight" << std::endl;
        std::cout << "2. Change crew assignment" << std::endl;
        std::cout << "3. Delete crew member" << std::endl;
        std::cout << "4. Back to Manage Flights Menu" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: 
                assignNewCrew();  // Function to assign a new crew member
                break;
            case 2: 
                changeCrewAssignment();  // Function to change crew assignment
                break;
            case 3: 
                deleteCrew();  // Function to delete a crew member
                break;
            case 4: 
                return;  // Back to the Manage Flights Menu
            default:
                std::cout << "Invalid choice! Please try again.\n";
                break;
        }
    } while (choice != 4);
}

// Function to assign a new crew member to a flight
void Administrator::assignNewCrew() {
    // Load the flights from the JSON file
    vector<Flight> flights = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);

    // Prompt for flight number
    string flightNumber;
    cout << "Enter the Flight Number to assign crew: ";
    cin >> flightNumber;

    shared_ptr<Flight> selectedFlight = nullptr;
    for (auto& flight : flights) {
        if (flight.getFlightNumber() == flightNumber) {
            selectedFlight = make_shared<Flight>(flight);
            break;
        }
    }
    if (!selectedFlight) {
        std::cout << "Flight not found.\n";
        return;
    }

    // Ask for crew member details
    std::string crewName, crewRole;
    std::cout << "Enter Crew Member Name: ";
    std::cin.ignore();
    std::getline(std::cin, crewName);
    std::cout << "Enter Crew Member Role: ";
    std::getline(std::cin, crewRole);

    // Create new Crew member and assign the selected flight
    Crew newCrew(crewName, crewRole);
    newCrew.assignFlight(selectedFlight);  // Assign the selected flight to the crew member

    // Save crew data to the crew file
    newCrew.saveCrewToFile(PATH_OF_CREW_DATA_BASE);
    std::cout << "Crew member (" << crewName << ") assigned successfully to flight (" << flightNumber << ")!\n";
}

void Administrator::changeCrewAssignment() {
    std::string crewName;
    std::cout << "Enter the crew member's name to change the assignment: ";
    std::cin.ignore();
    std::getline(std::cin, crewName);

    // Get the crew member by name
    Crew* crewMember = Crew::getCrewByName(crewName);  // No need to pass the file path here

    if (!crewMember) {
        std::cout << "Crew member not found.\n";
        return;
    }

    // Load the flights from the JSON file
    vector<Flight> flights = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);

    // Prompt for flight number
    string newFlightNumber;
    cout << "Enter the Flight Number to assign crew: ";
    cin >> newFlightNumber;

    shared_ptr<Flight> selectedFlight = nullptr;
    for (auto& flight : flights) {
        if (flight.getFlightNumber() == newFlightNumber) {
            selectedFlight = make_shared<Flight>(flight);
            break;
        }
    }

    if (!selectedFlight) {
        std::cout << "Flight not found.\n";
        return;
    }

    // Assign the new flight to the crew member
    crewMember->assignFlight(selectedFlight);  // Use the pointer to modify the crew member's assignment

    // Save updated crew assignments to the file
    crewMember->saveCrewToFile(PATH_OF_CREW_DATA_BASE);
    std::cout << "Crew member (" << crewName << ") reassigned successfully to flight " << newFlightNumber << "!\n";
}


// Function to delete a crew member from the system
void Administrator::deleteCrew() {
    std::string crewName;
    std::cout << "Enter the name of the crew member to delete: ";
    std::cin.ignore();
    std::getline(std::cin, crewName);

    // Attempt to remove the crew member
    bool removed = Crew::removeCrewFromFile(crewName, PATH_OF_CREW_DATA_BASE);

    if (removed) {
        std::cout << "Crew member (" << crewName << ") has been removed successfully.\n";
    } else {
        std::cout << "Crew member " << crewName << " not found or could not be removed.\n";
    }
}


void Administrator::manageAircraft() {
    int choice;
    do {
        cout << "\n--- Aircraft Management ---" << endl;
        cout << "1. Add Aircraft" << endl;
        cout << "2. Edit Aircraft" << endl;
        cout << "3. Delete Aircraft" << endl;
        cout << "4. Back to Main Menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addAircraft(); break;
            case 2: editAircraft(); break;
            case 3: deleteAircraft(); break;
            case 4: return;  // Back to main menu
            default: cout << "Invalid choice! Please try again.\n"; break;
        }
    } while (choice != 4);
}

void Administrator::addAircraft() {
    string aircraftID, aircraftType, maintenanceDetails;
    int capacity;

    // Enter Aircraft data
    cout << "Enter Aircraft ID: ";
    cin >> aircraftID;
    cout << "Enter Aircraft Type: ";
    cin >> aircraftType;
    cout << "Enter Aircraft Capacity: ";
    cin >> capacity;

    // Enter Maintenance details
    cout << "Enter Maintenance Details: ";
    cin.ignore();  // To consume the newline character left in the input buffer
    getline(cin, maintenanceDetails);

    // Create a Maintenance object
    shared_ptr<Maintenance> maint = make_shared<Maintenance>(aircraftID, maintenanceDetails);

    // Create an Aircraft object
    Aircraft newAircraft(aircraftID, aircraftType, capacity, maint);

    // Load existing aircraft data from JSON file
    //load first and store old data then add to it the new data then save 
    //the data is aved in map aircraftRecords
    map<string, Aircraft> aircraftRecords;
    Aircraft::loadAircraftData(PATH_OF_AIR_CRAFT_DATA_BASE, aircraftRecords);

    // Add new Aircraft to the records
    aircraftRecords[aircraftID] = newAircraft;

    // Save updated data to JSON file
    Aircraft::saveAircraftData(PATH_OF_AIR_CRAFT_DATA_BASE, aircraftRecords);

    cout << "Aircraft (" << aircraftID << ") added successfully.\n";
}

void Administrator::editAircraft() {
    string aircraftID;
    cout << "Enter Aircraft ID to edit: ";
    cin >> aircraftID;

    string fieldToUpdate, newValue;
    cout << "Enter the field to update (aircraftType, capacity, maintenanceDetails): ";
    cin >> fieldToUpdate;
    cout << "Enter the new value for " << fieldToUpdate << ": ";
    cin.ignore();  // To consume the newline character
    getline(cin, newValue);

    // Edit the Aircraft in the file
    if (Aircraft::editAircraft(PATH_OF_AIR_CRAFT_DATA_BASE, aircraftID, fieldToUpdate, newValue)) {
        cout << "Aircraft (" << aircraftID << ") updated successfully.\n";
    } else {
        cout << "Failed to update aircraft " << aircraftID << ".\n";
    }
}

void Administrator::deleteAircraft() {
    string aircraftID;
    cout << "Enter Aircraft ID to delete: ";
    cin >> aircraftID;

    // Delete the Aircraft from the file
    if (Aircraft::deleteAircraft(PATH_OF_AIR_CRAFT_DATA_BASE, aircraftID)) {
        cout << "Aircraft (" << aircraftID << ") removed successfully.\n";
    } else {
        cout << "Failed to remove aircraft " << aircraftID << ".\n";
    }
}