#include "../header/Payment.hpp"
#include <iostream>
#include <fstream>
#include "../header/json.hpp"
#include <memory>

using namespace std;
using json = nlohmann::json;

bool Payment::processPayment(shared_ptr<Reservation>& reservation) {
    int paymentMethod;
    cout << "Choose Payment Method (Cash/Card): "<<endl;
    cout << "\t1-Cash\n\t2-Card\n";
    cout << "Enter 1(Cash) or 2(Card) to choose way of payment you need: ";
    cin >> paymentMethod;

    if (paymentMethod == 1) {
        cout << "Your reservation is on hold. Please pay at the airport.\n";
        reservation->setPaymentMethod("Cash");  
        reservation->setIsPaid(false); // Mark the reservation as on hold
        return true;
    } else if (paymentMethod == 2) {
        bool paymentConfirmed = processCardPayment(reservation);  // Proceed with card payment process
        return paymentConfirmed;
    } else {
        cout << "Invalid payment method!\n";
        return false;
    }
}

bool Payment::processCardPayment(shared_ptr<Reservation>& reservation) {
    string cardNumber, cvv;
    string username = reservation->getPassengerName(); // Get the username associated with the reservation

    // Load the existing user cards JSON file
    ifstream cardFile(PATH_OF_USER_CARD_DATA_BASE);
    json allCards;
    if (!cardFile.is_open()) {
        cerr << "Error: Unable to open card database.\n";
        return false;
    }

    cardFile >> allCards;  // Load existing card data
    cardFile.close();

    // Check if the user's card exists in the file
    if (allCards.find(username) != allCards.end()) {
        // Card exists, ask for CVV
        cout << "Enter your saved card CVV: ";
        cin >> cvv;

        // Check if the entered CVV matches the saved one
        if (allCards[username]["cvv"] == cvv) {
            cout << "Payment successful with saved card!\n";
            reservation->setIsPaid(true); // Set payment as successful
            return true;
        } else {
            // Invalid CVV, prompt user to enter new card
            cout << "Invalid CVV. Please enter your new card details.\n";
        }
    } else {
        // If no saved card, ask user to input new card details
        cout << "Enter your card number: ";
        cin >> cardNumber;
        cout << "Enter card expiration date (MM/YY): ";
        string expDate;
        cin >> expDate;
        cout << "Enter cardholder name: ";
        string cardHolder;
        cin.ignore();
        getline(cin, cardHolder);
        cout << "Enter CVV: ";
        cin >> cvv;

        // Save the new card details in the reservation and update the user's data
        reservation->setPaymentMethod(cardNumber);  // Save card number as payment method
        reservation->setPaymentDetails(cvv);  // Save CVV as payment details

        // Save the card info to the user's file (in a shared JSON file)
        saveCardInfo(cardNumber, cvv, expDate, cardHolder, username);  // Save card details under username

        cout << "Payment successful!\n";
        reservation->setIsPaid(true);
        return true;
    }

    return false; // If no valid card or CVV, return false
}


void Payment::saveCardInfo(const string& cardNumber, const string& cvv, const string& expDate, const string& cardHolder, const string& username) {
    json cardData;
    cardData["cardNumber"] = cardNumber;
    cardData["cvv"] = cvv;
    cardData["expDate"] = expDate;
    cardData["cardHolder"] = cardHolder;

    // Load the existing user cards JSON file
    //load kol data al mogoda f al file w b3den b3ml update 3la al data aw add data gdeda 
    ifstream cardFile(PATH_OF_USER_CARD_DATA_BASE);
    json allCards;
    if (cardFile.is_open()) {
        cardFile >> allCards;  // Load existing card data
        cardFile.close();
    }

    // Add or update the card information under the username key

    /*
      If username already exists → it updates the card details.
      If username does NOT exist → it adds a new entry automatically.
    */    
   
    allCards[username] = cardData;

    // Save the updated JSON back to the file
    //kol mra h overwite kol al mogod
    ofstream outputFile(PATH_OF_USER_CARD_DATA_BASE);
    if (outputFile.is_open()) {
        outputFile << setw(4) << allCards << endl;  // Save the formatted JSON data
        outputFile.close();
    } else {
        cerr << "Error: Unable to save card information for user: " << username << ".\n";
    }
}
