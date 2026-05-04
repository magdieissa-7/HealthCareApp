#ifndef USER_H
#define USER_H
#include <string>

class User {
public:
    User();
    void setUsername(const std::string &name);
    void setPassword(const std::string &pass);
    void setCondition(const std::string &cond);
    void setEmergencyContact(const std::string &contact);
    std::string getUsername() const;
    std::string getPassword() const;
    std::string getCondition() const;
    std::string getEmergencyContact() const;

private:
    std::string username;
    std::string password;
    std::string condition;
    std::string emergencyContact;
};

#endif // USER_H
