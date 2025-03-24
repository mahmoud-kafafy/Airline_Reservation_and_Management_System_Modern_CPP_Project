#include "../header/BookingAgent.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

    using namespace std;
    using json = nlohmann::json;
    
    // ************************** CONSTRUCTOR **************************
    /*
     * Initializes a Passenger object.
     * Calls `Reservation::loadReservations()` to load all reservations
     * for this passenger from the `reservations.json` file.
     */
    BookingAgent::BookingAgent(string username, string password)
        : User(username, password, "BookingAgent") { 
        // Calls the parent constructor (User) to initialize the username, password, and role as "Passenger"
        reservations = Reservation::loadReservations(); // Load the reservations from the file into the reservations vector
    }
    
    // ************************** DISPLAY MENU **************************
    /*
     * Displays the main menu for the passenger.
     * Allows selection of:
     * 1. Searching for flights.
     * 2. Viewing reservations.
     * 3. Checking in.
     * 4. Canceling a reservation.
     * 5. Logging out.
     */
    // Display menu
    void BookingAgent::displayMenu() {
        int choice;
        do {
            cout << "\n--- Booking Agent Menu ---" << endl;
            cout << "1. Search Flights" << endl;
            cout << "2. Book a Flight" << endl;
            cout << "3. View User Reservation" << endl;
            cout << "4. Modify Reservation" << endl;
            cout << "5. Cancel Reservation" << endl;  
            cout << "6. Logout" << endl; // Confirm payment if cash was used
            cout << "Enter choice: ";
            cin >> choice; 
    
            switch (choice) {
                case 1: searchFlights(); break;
                case 2: bookFlight(); break;
                case 3: viewSpecificReservations(); break;
                //case 4: cancelReservation(); break;
                case 5: cancelReservation(); break;
                case 6: cout << "Logging out...\n"; break;
                default: cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 6); // Loop until the passenger logs out (choice == 6)
    }
    
    // ************************** SEARCH FLIGHTS **************************
    /*
     * Allows the passenger to search for available flights by:
     * - Origin
     * - Destination
     * - Departure Date
     * 
     * Searches the flight database (`flights.json`) and displays matching flights.
     */
    void BookingAgent::searchFlights() {
        string origin, destination, departureDate;
        cout << "\n--- Search Flights ---" << endl;
        cout << "Enter Origin: ";
        cin.ignore(); // To clear any leftover newline character in the input buffer
        getline(cin, origin); // Get the origin from the passenger
        cout << "Enter Destination: ";
        getline(cin, destination); // Get the destination from the passenger
    
        // Load all flights from the database file
        vector<Flight> flights = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);
    
        // Store flights that match search criteria
        vector<Flight> availableFlights;
        for (const Flight& flight : flights) {
            if (flight.getOrigin() == origin && flight.getDestination() == destination) {
                availableFlights.push_back(flight); // Add matching flights to the availableFlights vector
            }
        }
    
        // If no flights match, display message and return to menu
        if (availableFlights.empty()) {
            cout << "No flights found for the given criteria.\n";
            return;
        }
    
        // Display available flights
        cout << "\nAvailable Flights:\n";
        for (size_t i = 0; i < availableFlights.size(); ++i) {
            cout << i + 1 << ". Flight Number: " << availableFlights[i].getFlightNumber() << endl;
            availableFlights[i].displayFlightInfo(); // Display details of each available flight
        }
    
        // Allow user to book a flight
        //bookFlight(); // Calls the bookFlight method to handle flight booking
    }
    

    // ************************** BOOK FLIGHT **************************
    
    string BookingAgent::generateRandomReservationID() {
        // Generate a random capital letter (A-Z)
        char randomChar = 'A' + rand() % 26;  // Random letter between 'A' and 'Z'
    
        // Generate a random number between 100 and 999
        int randomNumber = rand() % 900 + 100;  // This will give a random number between 100 and 999
    
        // Combine the letter and number to form the reservation ID
        return string(1, randomChar) + to_string(randomNumber);
    }
    
    /*
     * Allows the passenger to book a flight by:
     * - Selecting a flight number.
     * - Choosing a seat.
     * - Entering payment details.
     * 
     * The reservation is then saved in `reservations.json`.
     */
    void BookingAgent::bookFlight() {
        string flightNumber, seatNumber, paymentMethod, paymentDetails;
        cout << "\nEnter the Flight Number you wish to book (or '0' to cancel): ";
        cin >> flightNumber;
        if (flightNumber == "0") return;
    
        // Load all flights from the database file
        vector<Flight> flights = Flight::loadFlights(PATH_OF_FLIGHTS_DATA_BASE);
    
        shared_ptr<Flight> selectedFlight = nullptr;
        for (const Flight& flight : flights) {
            if (flight.getFlightNumber() == flightNumber) {
                selectedFlight = make_shared<Flight>(flight);
                break;
            }
        }
    
        if (!selectedFlight) {
            cout << "Invalid flight number. Booking cancelled.\n";
            return;
        }
    
         // Ask for seat number
         cout << "Enter Seat Number (e.g., 14C): ";
         cin >> seatNumber;
     
         // Check if the entered seat number is within the total seats available
         if (stoi(seatNumber) > selectedFlight->getTotalSeats()) {
             cout << "Invalid seat number. Please choose a seat number within the available range. "<<selectedFlight->getTotalSeats()<<"\n";
             return;
         }
     
         // Check if the seat number is already reserved
         bool seatReserved = false;
         vector<shared_ptr<Reservation>> reservations = Reservation::loadBookingAgentReservations();
         for (const auto& reservation : reservations) {
             if (reservation->getFlight()->getFlightNumber() == flightNumber && reservation->getSeatNumber() == seatNumber) {
                 seatReserved = true;
                 break;
             }
         }
     
         if (seatReserved) {
             cout << "Seat number " << seatNumber << " is already reserved. Please choose another seat.\n";
             return;
         }
        // Create a new reservation object
        srand(time(0));  // Add this to ensure different reservation IDs
        string reservationID =  BookingAgent::generateRandomReservationID();  // Generate a random reservation ID
        shared_ptr<Reservation> newReservation = make_shared<Reservation>(reservationID, username, selectedFlight, seatNumber, "", "");
    
        // Process the payment
        bool paymentProcessed = Payment::processPayment(newReservation);  // Payment handling
    
        if (paymentProcessed) {
            if (newReservation->getIsPaid()) {
                // If payment was successful, add reservation to the list and save
                reservations.push_back(newReservation);
                Reservation::saveReservations(reservations);
                Reservation::saveBookingAgentReservations(reservations); 
                cout << "Reservation successful!\n";
                newReservation->displayReservation(); // Display the reservation details and passcode (boarding pass)
            } else {
                // If payment is by cash, the reservation is on hold
                cout << "Reservation placed on hold. Please confirm payment at the airport.\n";
                reservations.push_back(newReservation);  // Save the reservation even if payment is not completed
                Reservation::saveReservations(reservations);  // Save to file with isPaid = false
                Reservation::saveBookingAgentReservations(reservations);  // Save to file with isPaid = false
                newReservation->displayHoldingReservation();  // Display reservation details with payment on hold
            }
        } else {
            cout << "Payment failed. Reservation not completed.\n";
        }
    }
    
    
    
    // ************************** VIEW RESERVATIONS **************************
    /*
     * Displays all reservations made by the passenger.
     * Loads reservations from `reservations.json` if necessary.
     */
    void BookingAgent::viewAllReservations() {
        if (reservations.empty()) {
            cout << "\nNo reservations found.\n"; // If no reservations exist
            return;
        }
    
        cout << "\n--- My Reservations ---\n";
        for (const auto& reservation : reservations) {
            reservation->displayReservation(); // Display each reservation
        }
    }
    
    
    // ************************** VIEW RESERVATIONS **************************
    /*
     * Displays all reservations made by the current passenger (based on username).
     * Filters reservations based on the logged-in passenger's name.
     */
    void BookingAgent::viewSpecificReservations() {
        if (reservations.empty()) {
            cout << "\nNo reservations found.\n"; // If no reservations exist
            return;
        }
    
        cout << "\n--- My Reservations ---\n";
        bool found = false;  // To track if we find any reservations for the current user
    
        // Iterate through the reservations and display only those that belong to the logged-in user
        for (const auto& reservation : reservations) {
            if (reservation->getPassengerName() == username) { // Compare reservation's passenger name to the logged-in user's username
                reservation->displayReservation(); // Display reservation if it belongs to the logged-in user
                found = true;
            }
        }
    
        // If no reservations were found for the logged-in user, display a message
        if (!found) {
            cout << "No reservations found for " << username << ".\n";
        }
    }
    
    
    // ************************** CHECK-IN **************************
    /*
     * Allows a passenger to check-in for a flight using their Reservation ID.
     * If the reservation is found, it generates a boarding pass.
     */
    void BookingAgent::checkIn() {
        string reservationID;
        cout << "\n--- Check-In ---" << endl;
        cout << "Enter Reservation ID: ";
        cin >> reservationID; // Get the reservation ID from the passenger
    
        // Search for the reservation by ID
        for (auto& reservation : reservations) {
            if (reservation->getReservationID() == reservationID) {
                // Check if the payment has been completed by checking the "isPaid" field from the loaded data
                if (reservation->getIsPaid()) { // Ensure this field is correctly set from your JSON data
                    cout << "\n\t\t✅Check-In Successful!\n";
                    reservation->printBoardingPass(); // Print the boarding pass for the reservation
                    return;
                } else {
                    cout << "You haven't completed the payment process yet.\n";
                    return;
                }
            }
        }
    
        // If reservation ID is not found, display an error message
        cout << "Invalid Reservation ID.\n";
    }
    
    
    
    // ************************** REMOVE RESERVATION **************************
    /*
     * Allows a passenger to remove a reservation by providing the reservation ID.
     * It will call the removeReservation function from the Reservation class.
     */
    void BookingAgent::cancelReservation() {
        string reservationID;
        cout << "\n--- Remove Reservation ---" << endl;
        cout << "Enter Reservation ID to remove: ";
        cin >> reservationID; // Get the reservation ID to remove
    
        // Call the removeReservation function from Reservation class
        Reservation::removeReservation(reservationID);
    
        // After removing, update the reservations list by reloading from the file
        reservations = Reservation::loadReservations();
    }
    
    // Confirm cash payment method (airport confirmation)
    void BookingAgent::confirmCashPayment() {
        string reservationID;
        cout << "\nEnter Reservation ID to confirm payment at the airport: ";
        cin >> reservationID;
    
        // Find the reservation and update its status
        for (auto& reservation : reservations) {
            if (reservation->getReservationID() == reservationID && !reservation->getIsPaid()) {
                cout << "Payment confirmed at the airport!\n";
                reservation->setIsPaid(true);  // Set payment as completed
                Reservation::saveReservations(reservations); // Save updated reservation
                Reservation::saveBookingAgentReservations(reservations); // Save updated reservation
                cout << "Reservation is now confirmed and passcode is generated.\n";
                reservation->printBoardingPass();  // Print passcode (boarding pass)
                return;
            }
        }
        cout << "Reservation ID not found or payment already confirmed.\n";
    }
