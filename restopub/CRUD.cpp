#include "CRUD.hpp"
#include <fmt/format.h>

#ifndef CRUD
CRUD::CRUD() {
}

pqxx::connection CRUD::createConnection()
{
    pqxx::connection c("dbname = discord_bot user = postgres password = 1488Moniev! \
      hostaddr = 127.0.0.1 port = 5432");
    if (c.is_open()) {
        std::cout << "connected to database!" << std::endl;
    }
    else {
        std::cout << "connection is failed!" << std::endl;
    }
    return c;
}

bool insertToken(int user_id, std::string token, pqxx::connection &_connection)
{
    std::string insertion = fmt::format("INSERT INTO tokens\
                                         (user_id, token)\
                                         VALUES('{0}', '{1}')", user_id, token);
    pqxx::work _work(_connection);
    _work.exec(insertion);
    try {
        _work.commit();
        return true;
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool CRUD::insertReservation(int user_id, std::string reservation_date, std::string leave_date, pqxx::connection &_connection)
{
    std::string insertion = fmt::format("INSERT INTO reservations\
                                         (user_id, reservation_date, leave_date)\
                                         VALUES('{0}', '{1}', '{2}');", user_id, reservation_date, leave_date);
    pqxx::work _work(_connection);
    _work.exec(insertion);
    try {
        _work.commit();
        return true;
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool CRUD::activateUser(int user_id, pqxx::connection &_connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT * FROM clients WHERE email='{0}';", user_id);
    pqxx::result _result(non_tranaction.exec(query));
    if (_result.size() > 0) {
        std::string update_statement = fmt::format("UPDATE clients SET active=true WHERE email='{0}';", user_id);
        pqxx::work _work(_connection);
        _work.exec(update_statement);
        try {
            _work.commit();
            return true;
        }
        catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
    return false;
}

bool CRUD::deleteUser(int user_id, pqxx::connection& _connection)
{
    std::string deletion = fmt::format("", user_id);
    pqxx::work _work(_connection);
    _work.exec(deletion);
    try {
        _work.commit();
        return true;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}


bool CRUD::insertUser(std::string nickname, std::string name, std::string lastname, std::string email, std::string password, pqxx::connection &_connection)
{
    std::string insertion = fmt::format("INSERT INTO clients\
                                         (nickname, name, lastname, email, password)\
                                         VALUES('{0}', '{1}', '{2}', '{3}', {4});", nickname, name, lastname, email, password);
    pqxx::work _work(_connection);
    _work.exec(insertion);
    try {
        _work.commit();
        return true;
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}


bool CRUD::checkIfUserIsActive(int user_id, pqxx::connection &_connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT * FROM clients WHERE id='{0}';", user_id);
    pqxx::result _result(non_tranaction.exec(query));
    if (_result.size() > 0) {
        return _result[0][4].as<bool>();
    }
    return false;
}

std::string CRUD::getUserNickname(int user_id, pqxx::connection &_connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT name FROM clients WHERE id='{0}';", user_id);
    pqxx::result _result(non_tranaction.exec(query));
    if (_result.size() > 0) {
        return _result[0][0].as<std::string>();
    }
    return "guest";
}

pqxx::result CRUD::getUserByEmail(std::string email, pqxx::connection &_connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT * FROM clients WHERE email='{0}';", email);
    pqxx::result _result(non_tranaction.exec(query));
    return _result;
}

int CRUD::getUserId(std::string email, pqxx::connection &_connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT id FROM clients WHERE email='{0}';", email);
    pqxx::result _result(non_tranaction.exec(query));
    if (_result.size() > 0) {
        return _result[0][0].as<int>();
    }
    return 0;
}

bool CRUD::checkIfTableExist(std::string table_name, pqxx::work &_work) {
    std::string query = fmt::format("SELECT EXISTS(\
                                     SELECT 1\
                                     FROM pg_tables\
                                     WHERE tablename = '{}')", table_name);
    pqxx::result _result(_work.exec(query));
    return _result[0][0].as<bool>();
}

bool CRUD::validateToken(std::string token, int user_id, pqxx::connection &_connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT C.id, T.token FROM clients C JOIN tokens T ON C.id = T.user_id WHERE C.id='{0}';", user_id);
    pqxx::result _result(non_tranaction.exec(query));
    if (_result.size() > 0) {
        return _result[0][1].as<std::string>() == token;
    }
    return false;
}

bool CRUD::validateNickname(std::string nickname, pqxx::connection &_connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT * FROM clients WHERE nickname='{0}'", nickname);
    pqxx::result _result(non_tranaction.exec(query));
    return (_result.size() == 0);
}

bool CRUD::validateUser(std::string email, std::string password, pqxx::connection  &_connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT id FROM clients WHERE email='{0}' AND password='{1}'", email, password);
    pqxx::result _result(non_tranaction.exec(query));
    return (_result.size() > 0);
}

bool CRUD::validateEmail(std::string email, pqxx::connection &_connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT * FROM clients WHERE email='{0}'", email);
    pqxx::result _result(non_tranaction.exec(query));
    return (_result.size() == 0);
}

void CRUD::createDatabase(pqxx::connection& _connection) {
    std::string create_enters_table = "CREATE TABLE enters("\
        "id SERIAL PRIMARY KEY,"\
        "user_id INT NOT NULL,"\
        "enter_date TIMESTAMP NOT NULL);";

    std::string create_reservations_table = "CREATE TABLE reservations("\
        "id SERIAL PRIMARY KEY,"\
        "user_id INT NOT NULL,"\
        "confirmed BOOLEAN NOT NULL,"\
        "started BOOLEAN NOT NULL,"\
        "completed BOOLEAN NOT NULL,"\
        "enter_date TIMESTAMP NOT NULL,"\
        "leave_date TIMESTAMP NOT NULL);";

    std::string create_items_table = "CREATE TABLE items("\
        "id SERIAL PRIMARY KEY,"\
        "item_name TEXT NOT NULL,"\
        "price INT NOT NULL);";

    std::string create_orders_table = "CREATE TABLE orders("\
        "id SERIAL PRIMARY KEY,"\
        "user_id INT NOT NULL,"\
        "item TEXT NOT NULL,"\
        "confirmed BOOLEAN NOT NULL,"\
        "delivered BOOLEAN NOT NULL,"\
        "completed BOOLEAN NOT NULL,"\
        "order_date TIMESTAMP NOT NULL,"\
        "finalization_date TIMESTAMP NOT NULL);";

    std::string create_delivery_towns_table = "CREATE TABLE delivery_towns("\
        "id SERIAL PRIMARY KEY,"\
        "town TEXT NOT NULL,"\
        "postal_code TEXT NOT NULL);";

    std::string create_deliveries_table = "CREATE TABLE deliveries("\
        "id SERIAL PRIMARY KEY,"\
        "town TEXT NOT NULL,"\
        "adress TEXT NOT NULL,"\
        "postal_code TEXT NOT NULL);";

    std::string create_clients_table = "CREATE TABLE clients("\
        "id SERIAL PRIMARY KEY,"\
        "nickname TEXT NOT NULL,"\
        "name TEXT NOT NULL,"\
        "lastname TEXT NOT NULL,"\
        "email TEXT NOT NULL,"\
        "active BOOLEAN DEFAULT FALSE,"\
        "password TEXT NOT NULL);";

    std::string create_tokens_table = "CREATE TABLE tokens("\
        "id SERIAL PRIMARY KEY,"\
        "user_id INT NOT NULL,"\
        "token TEXT NOT NULL);";

    pqxx::work _work(_connection);

    if (!checkIfTableExist("items", _work)) {
        _work.exec(create_items_table);
    }
    if (!checkIfTableExist("delivery_towns", _work)) {
        _work.exec(create_delivery_towns_table);
    }
    if (!checkIfTableExist("deliveries", _work)) {
        _work.exec(create_deliveries_table);
    }
    if (!checkIfTableExist("enters", _work)) {
        _work.exec(create_enters_table);
    }
    if (!checkIfTableExist("reservations", _work)) {
        _work.exec(create_reservations_table);
    }
    if (!checkIfTableExist("orders", _work)) {
        _work.exec(create_orders_table);
    }
    if (!checkIfTableExist("clients", _work)) {
        _work.exec(create_clients_table);
    }
    if (!checkIfTableExist("tokens", _work)) {
        _work.exec(create_tokens_table);
    }

    try {
        _work.commit();
        std::cout << "Database successfully created!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cout << "Unable to create database!" << std::endl;
    }
}

#endif