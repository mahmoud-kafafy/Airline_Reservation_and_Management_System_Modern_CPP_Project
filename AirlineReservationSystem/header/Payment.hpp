#ifndef PAYMENT_HPP
#define PAYMENT_HPP

#include <string>
#include <memory>
#include "Reservation.hpp"  // Include the header for Reservation class

#define PATH_OF_USER_CARD_DATA_BASE "data_base/user_cards.json"

class Payment {
public:
    // Processes the payment
    static bool processPayment(std::shared_ptr<Reservation>& reservation);

    // Processes card payment
    static bool processCardPayment(std::shared_ptr<Reservation>& reservation);

    // Save card info for the user
    static void saveCardInfo(const std::string& cardNumber, const std::string& cvv, const std::string& expDate, const std::string& cardHolder, const std::string& username);
};

#endif // PAYMENT_HPP

