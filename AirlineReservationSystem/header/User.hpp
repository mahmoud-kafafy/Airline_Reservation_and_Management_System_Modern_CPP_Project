#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <string>

class User {
protected:
    std::string username;
    std::string password;
    std::string role;

public:
    User(std::string username, std::string password, std::string role);
    
    virtual void displayMenu() = 0; // Pure virtual function for role-based menu

    std::string getUsername() const;
    std::string getRole() const;
    
    bool login(std::string inputUsername, std::string inputPassword);
};

#endif
