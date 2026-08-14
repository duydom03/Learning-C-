-- classes table
CREATE TABLE IF NOT EXISTS classes (
    id    INTEGER PRIMARY KEY AUTOINCREMENT,
    name  TEXT    NOT NULL UNIQUE,
    room  TEXT
);

-- students table
CREATE TABLE IF NOT EXISTS students (
    id        INTEGER PRIMARY KEY AUTOINCREMENT,
    name      TEXT    NOT NULL,
    email     TEXT    UNIQUE,
    class_id  INTEGER,
    FOREIGN KEY (class_id) REFERENCES classes(id) ON DELETE SET NULL
);