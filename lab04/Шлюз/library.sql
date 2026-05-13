create database if not exists lib;
use lib;
create table if not exists books (
	id int primary key auto_increment,
    title varchar(200) not null,
    author varchar(100) not null,
    year int not null,
    mark int not null
);
insert into books (title, author, year, mark) values
('Война и мир', 'Лев Толстой', 1869, '7'),
('Идиот', 'Фёдор Достоевский', 1869, '8'),
('Мастер и Маргарита', 'Михаил Булгаков', 1967, '10'),
('1984', 'Джордж Оруэлл', 1949, '3');
