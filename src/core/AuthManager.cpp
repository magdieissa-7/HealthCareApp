#include "AuthManager.h"

bool AuthManager::authenticate(const std::string &username, const std::string &password) {

    return (username == "admin" && password == "1234");
}
