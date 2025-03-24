#ifndef ADMINISTRATOR_HPP
#define ADMINISTRATOR_HPP

#include "User.hpp"
#include "Flight.hpp"
#include <vector>
#include "json.hpp"
#include "Reservation.hpp"
#include "Aircraft.hpp"
#include "Maintenance.hpp"
#include "Crew.hpp"


class Administrator : public User {
public:
    Administrator(std::string username, std::string password);
    
    void displayMenu() override;
    void manageFlights();
    void addNewFlight();
    void updateFlight();
    void removeFlight();
    void viewAllFlights();
    void generateOperationalReport();
    void assignCrew(); 
    void deleteCrew();
    void changeCrewAssignment();
    void assignNewCrew();
    // Manage Aircraft
    void manageAircraft();
    void addAircraft();
    void editAircraft();
    void deleteAircraft();
};

#endif
