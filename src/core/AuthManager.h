#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H
#include <string>

// AuthManager handles all authentication logic.
// This keeps credential checking OUT of the GUI (LoginWindow),
// satisfying the requirement that GUI classes must not contain business logic.
class IAuthManager {
public:
    virtual ~IAuthManager() = default;
    virtual bool authenticate(const std::string &username, const std::string &password) = 0;
};

class AuthManager : public IAuthManager {
public:
    // In a real app this would query DatabaseManager.
    // Hardcoded here for demonstration; logic lives here, NOT in LoginWindow.
    bool authenticate(const std::string &username, const std::string &password) override;
};

#endif // AUTH_MANAGER_H
