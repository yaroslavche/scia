create table Person
(
    id     INTEGER      not null primary key autoincrement,
    name   VARCHAR(255) not null,
    active INTEGER default 1
);
