-- Seed data: 3 classes, 12 students
INSERT INTO classes (name, room) VALUES
    ('CSE101', 'Room A1'),
    ('CSE102', 'Room B2'),
    ('CSE103', 'Room C3');

INSERT INTO students (name, email, class_id) VALUES
    ('Alice Nguyen',   'alice@school.com',   1),
    ('Bob Tran',       'bob@school.com',     1),
    ('Charlie Le',     'charlie@school.com', 1),
    ('Diana Pham',     'diana@school.com',   1),
    ('Ethan Vo',       'ethan@school.com',   2),
    ('Fiona Hoang',    'fiona@school.com',   2),
    ('George Dang',    'george@school.com',  2),
    ('Hannah Bui',     'hannah@school.com',  2),
    ('Ian Truong',     'ian@school.com',     3),
    ('Jane Lam',       'jane@school.com',    3),
    ('Kevin Mac',      'kevin@school.com',   3),
    ('Lily Ngo',       'lily@school.com',    3);
