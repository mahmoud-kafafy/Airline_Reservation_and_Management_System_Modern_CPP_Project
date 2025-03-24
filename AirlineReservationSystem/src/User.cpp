#include "../header/User.hpp"

User::User(std::string username, std::string password, std::string role) 
    : username(username), password(password), role(role) {}

std::string User::getUsername() const { return username; }
std::string User::getRole() const { return role; }

bool User::login(std::string inputUsername, std::string inputPassword) {
    return (inputUsername == username && inputPassword == password);
}
