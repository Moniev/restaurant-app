#include "CRUD.h"
#include <iostream>
#include <fmt/format.h>


CRUD::CRUD() {}

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

bool CRUD::activateUser(std::string email, pqxx::connection& _connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT * FROM clients WHERE email='{0}';", email);
    pqxx::result _result(non_tranaction.exec(query));
    if (_result.size() > 0) {
        std::string update_statement = fmt::format("UPDATE clients SET active=true WHERE email='{0}';", email);
        pqxx::work _work(_connection);
        _work.exec(update_statement);
        try {
            _work.commit();
            return true;
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    return false;
}

bool CRUD::deleteUser(std::string email, pqxx::connection& _connection)
{
    std::string insertion = fmt::format("", email);
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

bool CRUD::insertUser(std::string name, std::string lastname, std::string email, std::string password, pqxx::connection& _connection)
{
    std::string insertion = fmt::format("INSERT INTO clients\
                                        (name, lastname, email, password)\
                                        VALUES('{0}', '{1}', '{2}', '{3}');", name, lastname, email, password);
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

bool CRUD::checkIfUserIsActive(std::string email, pqxx::connection& _connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT * FROM clients WHERE email='{0}';", email);
    pqxx::result _result(non_tranaction.exec(query));
    if (_result.size() > 0) {
        return _result[0][4].as<bool>();
    }
    return false;
}

bool CRUD::checkIfTableExist(std::string table_name, pqxx::connection& _connection)
{
    pqxx::nontransaction non_transaction(_connection);
    std::string query = fmt::format("SELECT EXISTS(\
        SELECT 1\
        FROM pg_tables\
        WHERE tablename = '{}')", table_name);
    pqxx::result _result(non_transaction.exec(query));
    return _result[0][0].as<bool>();
}

bool CRUD::validateToken(std::string token, std::string email, pqxx::connection& _connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT C.id, T.token FROM clients C JOIN tokens T ON C.id = T.user_id WHERE C.email='{0}';", email);
    pqxx::result _result(non_tranaction.exec(query));
    if (_result.size() > 0) {
        return _result[0][1].as<std::string>() == token;
    }
    return false;
}

void CRUD::createDatabase(pqxx::connection& _connection)
{
    std::string create_enters_table = "CREATE TABLE enters("\
        "id SERIAL PRIMARY KEY,"\
        "user_id INT NOT NULL,"\
        "enter_date TIMESTAMP NOT NULL);";

    std::string create_reservations_table = "CREATE TABLE reservations("\
        "id SERIAL PRIMARY KEY,"\
        "user_id INT NOT NULL,"\
        "enter_date TIMESTAMP NOT NULL,"\
        "leave_date TIMESTAMP NOT NULL);";
    ;

    std::string create_orders_table = "CREATE TABLE orders("\
        "id SERIAL PRIMARY KEY,"\
        "user_id INT NOT NULL,"\
        "item TEXT NOT NULL,"\
        "order_date TIMESTAMP NOT NULL,"\
        "finalization_date TIMESTAMP NOT NULL);";

    std::string create_clients_table = "CREATE TABLE clients("\
        "id SERIAL PRIMARY KEY,"\
        "name           TEXT    NOT NULL,"\
        "lastname           TEXT    NOT NULL,"\
        "email        TEXT NOT NULL,"\
        "active BOOLEAN NOT NULL"\
        "password         TEXT);";

    std::string create_tokens_table = "CREATE TABLE tokens("\
        "id SERIAL PRIMARY KEY,"\
        "user_id INT NOT NULL,"\
        "token TEXT NOT NULL);";

    pqxx::work _work(_connection);

    if (!CRUD::checkIfTableExist("enters", _connection)) {
        _work.exec(create_enters_table);
    }
    if (!CRUD::checkIfTableExist("reservations",_connection)) {
        _work.exec(create_reservations_table);
    }
    if (!CRUD::checkIfTableExist("orders", _connection)) {
        _work.exec(create_orders_table);
    }
    if (!CRUD::checkIfTableExist("clients", _connection)) {
        _work.exec(create_clients_table);
    }
    if (!CRUD::checkIfTableExist("tokens", _connection)) {
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