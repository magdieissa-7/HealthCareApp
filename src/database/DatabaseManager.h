#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include "../core/Medication.h"

// DatabaseManager wraps SQLite3 to persist user and medication data.
class DatabaseManager {
public:
    explicit DatabaseManager(const std::string &dbPath);
    ~DatabaseManager();

    bool open();
    void close();

    // User operations
    bool saveUser(const std::string &username, const std::string &passwordHash);
    bool validateUser(const std::string &username, const std::string &password);

    // Medication operations
    bool saveMedication(const std::string &username, const Medication &med);
    std::vector<Medication> loadMedications(const std::string &username);

private:
    std::string dbPath;
    sqlite3    *db = nullptr;

    bool createTables();
    bool execute(const std::string &sql);
};

#endif // DATABASE_MANAGER_H
