#include "DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager(const std::string &dbPath) : dbPath(dbPath) {}

DatabaseManager::~DatabaseManager()
{
    close();
}

bool DatabaseManager::open()
{
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "[DatabaseManager] Cannot open DB: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    return createTables();
}

void DatabaseManager::close()
{
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DatabaseManager::createTables()
{
    const std::string usersTable =
        "CREATE TABLE IF NOT EXISTS users ("
        "  id       INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username TEXT    NOT NULL UNIQUE,"
        "  password TEXT    NOT NULL"
        ");";

    const std::string medsTable =
        "CREATE TABLE IF NOT EXISTS medications ("
        "  id       INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username TEXT    NOT NULL,"
        "  name     TEXT    NOT NULL,"
        "  dosage   TEXT    NOT NULL,"
        "  time     TEXT    NOT NULL"
        ");";

    return execute(usersTable) && execute(medsTable);
}

bool DatabaseManager::execute(const std::string &sql)
{
    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "[DatabaseManager] SQL error: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool DatabaseManager::saveUser(const std::string &username,
                                const std::string &passwordHash)
{
    std::string sql = "INSERT OR IGNORE INTO users (username, password) VALUES ('" +
                      username + "', '" + passwordHash + "');";
    return execute(sql);
}

bool DatabaseManager::validateUser(const std::string &username,
                                    const std::string &password)
{
    std::string sql = "SELECT COUNT(*) FROM users WHERE username='" +
                      username + "' AND password='" + password + "';";
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return count > 0;
}

bool DatabaseManager::saveMedication(const std::string &username,
                                      const Medication &med)
{
    std::string sql = "INSERT INTO medications (username, name, dosage, time) VALUES ('" +
                      username + "', '" + med.getName() + "', '" +
                      med.getDosage() + "', '" + med.getTime() + "');";
    return execute(sql);
}

std::vector<Medication> DatabaseManager::loadMedications(const std::string &username)
{
    std::vector<Medication> results;
    std::string sql = "SELECT name, dosage, time FROM medications WHERE username='" +
                      username + "';";
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string name   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string dosage = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string time   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        results.emplace_back(name, dosage, time);
    }
    sqlite3_finalize(stmt);
    return results;
}
