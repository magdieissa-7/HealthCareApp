#include "EmergencyManager.h"
void EmergencyManager::setEmergencyContact(const std::string &contact) { emergencyContact = contact; }
std::string EmergencyManager::getEmergencyContact() const { return emergencyContact; }
std::string EmergencyManager::createEmergencyMessage() const {
    return "Emergency alert: The patient needs help immediately. Contact: " + emergencyContact;
}
