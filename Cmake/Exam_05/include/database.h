#pragma once
#include <string>
#include <vector>
#include <functional>
#include "models.h"

// Forward declaration of SQLite types
struct sqlite3;
struct sqlite3_stmt;

/**
 * RAII wrapper around an SQLite database connection.
 * Non-copyable, movable.
 */
class Database {
public:
    // Callback type for processing each row of a query result.
    // columnValues[i] is the text value of column i (may be nullptr for NULL).
    // columnNames[i] is the name of column i.
    using RowCallback = std::function<void(int argc, char** columnValues, char** columnNames)>;

    // Open (or create) the database file. Runs schema.sql if tables are missing
    // and seed.sql if the database is empty.
    explicit Database(const std::string& dbPath = "school.db");
    ~Database();

    // Non-copyable
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    // Movable
    Database(Database&& other) noexcept;
    Database& operator=(Database&& other) noexcept;

    // Execute a SQL statement that does not return rows (INSERT/UPDATE/DELETE/CREATE).
    // Returns true on success, false on error (prints error to stderr).
    bool execute(const std::string& sql);

    // Execute a SQL query and invoke the callback for each result row.
    // Returns true on success, false on error.
    bool query(const std::string& sql, const RowCallback& callback);

    // Get the rowid of the most recent successful INSERT.
    long long lastInsertRowid() const;

    // Get the number of rows changed by the most recent UPDATE/DELETE/INSERT.
    int changes() const;

    // Access the raw sqlite3 handle (for advanced use by Manager classes).
    sqlite3* raw() const;

private:
    void initSchema();
    void seedIfEmpty();

    sqlite3* db_;
};