#include "User.h"
User::User() {}
void User::setUsername(const std::string &name) { username = name; }
void User::setPassword(const std::string &pass) { password = pass; }
void User::setCondition(const std::string &cond) { condition = cond; }
void User::setEmergencyContact(const std::string &contact) { emergencyContact = contact; }
std::string User::getUsername() const { return username; }
std::string User::getPassword() const { return password; }
std::string User::getCondition() const { return condition; }
std::string User::getEmergencyContact() const { return emergencyContact; }
