#include "database.h"

#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <sstream>

Database::Database(const std::string& dbPath) : db_(nullptr) {
    int rc = sqlite3_open(dbPath.c_str(), &db_);
    if (rc != SQLITE_OK) {
        std::cerr << "[Database] Cannot open database: " << sqlite3_errmsg(db_) << std::endl;
        db_ = nullptr;
        return;
    }
    // Enable foreign keys so ON DELETE SET NULL works.
    execute("PRAGMA foreign_keys = ON;");
    initSchema();
    seedIfEmpty();
}

Database::~Database() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

Database::Database(Database&& other) noexcept : db_(other.db_) {
    other.db_ = nullptr;
}

Database& Database::operator=(Database&& other) noexcept {
    if (this != &other) {
        if (db_) {
            sqlite3_close(db_);
        }
        db_ = other.db_;
        other.db_ = nullptr;
    }
    return *this;
}

bool Database::execute(const std::string& sql) {
    if (!db_) {
        std::cerr << "[Database] No open connection." << std::endl;
        return false;
    }
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "[Database] SQL error: " << (errMsg ? errMsg : "unknown") << std::endl;
        if (errMsg) sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool Database::query(const std::string& sql, const RowCallback& callback) {
    if (!db_) {
        std::cerr << "[Database] No open connection." << std::endl;
        return false;
    }
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db_, sql.c_str(),
        [](void* data, int argc, char** argv, char** colNames) -> int {
            const RowCallback& cb = *static_cast<const RowCallback*>(data);
            cb(argc, argv, colNames);
            return 0;
        },
        const_cast<RowCallback*>(&callback),
        &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "[Database] Query error: " << (errMsg ? errMsg : "unknown") << std::endl;
        if (errMsg) sqlite3_free(errMsg);
        return false;
    }
    return true;
}

long long Database::lastInsertRowid() const {
    if (!db_) return 0;
    return sqlite3_last_insert_rowid(db_);
}

int Database::changes() const {
    if (!db_) return 0;
    return sqlite3_changes(db_);
}

sqlite3* Database::raw() const {
    return db_;
}

void Database::initSchema() {
    // Check if 'classes' table exists; if not, run schema.sql.
    bool tableExists = false;
    query("SELECT name FROM sqlite_master WHERE type='table' AND name='classes';",
        [&tableExists](int argc, char** argv, char** /*colNames*/) {
            if (argc > 0 && argv[0]) tableExists = true;
        });
    if (!tableExists) {
        std::ifstream file("schema.sql");
        if (file) {
            std::stringstream ss;
            ss << file.rdbuf();
            execute(ss.str());
        } else {
            std::cerr << "[Database] schema.sql not found; creating tables inline." << std::endl;
            execute(
                "CREATE TABLE IF NOT EXISTS classes ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name TEXT NOT NULL UNIQUE,"
                "room TEXT);"
                "CREATE TABLE IF NOT EXISTS students ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name TEXT NOT NULL,"
                "email TEXT UNIQUE,"
                "class_id INTEGER,"
                "FOREIGN KEY (class_id) REFERENCES classes(id) ON DELETE SET NULL);"
            );
        }
    }
}

void Database::seedIfEmpty() {
    // If classes table is empty, load seed.sql.
    bool hasClasses = false;
    query("SELECT COUNT(*) FROM classes;",
        [&hasClasses](int argc, char** argv, char** /*colNames*/) {
            if (argc > 0 && argv[0]) {
                hasClasses = (std::stoi(argv[0]) > 0);
            }
        });
    if (!hasClasses) {
        std::ifstream file("seed.sql");
        if (file) {
            std::stringstream ss;
            ss << file.rdbuf();
            execute(ss.str());
        } else {
            std::cerr << "[Database] seed.sql not found; skipping seed." << std::endl;
        }
    }
}
