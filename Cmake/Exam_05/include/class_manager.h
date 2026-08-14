#pragma once
#include <vector>
#include "models.h"
#include "database.h"

class ClassManager {
public:
    explicit ClassManager(Database& db);

    // Create a new class. Returns the new class id, or -1 on failure.
    int addClass(const std::string& name, const std::string& room);

    // Read all classes.
    std::vector<Class> listClasses();

    // Update a class by id. Returns true on success.
    bool updateClass(int id, const std::string& name, const std::string& room);

    // Delete a class by id. Students in it get class_id = NULL (FK ON DELETE SET NULL).
    // Returns true on success.
    bool deleteClass(int id);

    // Get a single class by id. Returns true and fills `out` if found.
    bool getClass(int id, Class& out);

private:
    Database& db_;
};