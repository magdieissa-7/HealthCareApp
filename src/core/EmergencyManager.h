#ifndef EMERGENCY_MANAGER_H
#define EMERGENCY_MANAGER_H
#include <string>

class EmergencyManager {
public:
    void setEmergencyContact(const std::string &contact);
    std::string getEmergencyContact() const;
    std::string createEmergencyMessage() const;

private:
    std::string emergencyContact;
};

#endif // EMERGENCY_MANAGER_H
