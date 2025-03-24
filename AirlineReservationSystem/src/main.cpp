#include <iostream>
#include <fstream>
#include <conio.h>
#include "../header/json.hpp"
#include "../header/Administrator.hpp"
#include "../header/Passenger.hpp"  // Include Passenger header
#include "../header/BookingAgent.hpp"  // Include BookingAgent header

using json = nlohmann::json;
using namespace std;
#define PATH_OF_USERS_DATA_BASE "data_base/users.json"

// Load existing users from JSON file
json loadUsers(const string& filename) {
    ifstream file(filename);
    json users;
    if (file.is_open()) {
        file >> users;
    }
    return users;
}

// Save updated users to JSON file
void saveUsers(const string& filename, const json& users) {
    ofstream file(filename);
    file << setw(4) << users << endl;  // formatted output
}

// Function to securely read a password from the user without showing the actual input
string getMaskedPassword() {
    string password;
    char ch;
    
    while ((ch = _getch()) != '\r') {  // Read until Enter key is pressed
        if (ch == '\b') {  // Handle Backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";  // Remove last '*' character
            }
        } else {
            password += ch;
            cout << '*';  // Mask input
        }
    }
    cout << endl;
    return password;
}

int main() {
    srand(time(0));  // Add this to ensure different reservation IDs
    cout << "\t\t\t\t  Welcome to Airline Reservation and Management System \n";
    cout << "Please select your role:\n";
    cout << "1. Administrator\n";
    cout << "2. Booking Agent\n";
    cout << "3. Passenger\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    string role;
    switch (choice) {
        case 1: role = "Administrator"; break;
        case 2: role = "Booking Agent"; break;
        case 3: role = "Passenger"; break;
        default: 
            cout << "Invalid choice.\n";
            return 0;
    }

    string username, password;
    json users_in_json = loadUsers(PATH_OF_USERS_DATA_BASE);

    cout << "\n--- " << role << " Login ---\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    password = getMaskedPassword();

    if (users_in_json.contains(username)) {
        if (users_in_json[username]["password"] == password && users_in_json[username]["role"] == role) {
            cout << "\nLogin successful! (Welcome back)\n";
            
            if (role == "Administrator") {
                Administrator admin(username, password);
                admin.displayMenu();
            } 
            else if (role == "Passenger") {
                Passenger passenger(username, password);  // Create Passenger object
                passenger.displayMenu();  // Call Passenger menu
            }
            else if (role == "Booking Agent") {
                BookingAgent BookingAgent(username, password);  // Create BookingAgent object
                BookingAgent.displayMenu();  // Call Passenger menu
            }

        } else {
            cout << "\nInvalid credentials or role mismatch!\n";
        }
    } else {
        cout << "\nCreating new user account...\n";
        
        users_in_json[username] = {
            {"password", password},
            {"role", role}
        };
        saveUsers(PATH_OF_USERS_DATA_BASE, users_in_json);
        
        cout << "Account created successfully!!!\n";
        
        if (role == "Administrator") {
            Administrator admin(username, password);
            admin.displayMenu();
        } 
        else if (role == "Passenger") {
            Passenger passenger(username, password);
            passenger.displayMenu();
        }
        else if (role == "Booking Agent") {
            BookingAgent BookingAgent(username, password);
            BookingAgent.displayMenu();
        }
    }

    return 0;
}
