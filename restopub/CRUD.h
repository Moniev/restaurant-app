#pragma once
#include <pqxx/pqxx>
#include <iostream>
class CRUD
{
	public:
		CRUD();

		pqxx::connection createConnection();
		
		bool deleteUser(std::string email, pqxx::connection& _connection);
	
		bool insertUser(std::string name, std::string lastname, std::string email, std::string password, pqxx::connection& _connection);

		bool checkIfUserIsActive(std::string email, pqxx::connection& _connection);

		bool activateUser(std::string email, pqxx::connection& _connection);

		bool checkIfTableExist(std::string table_name, pqxx::connection& _connection);

		void createDatabase(pqxx::connection& _connection);

		bool validateToken(std::string token, std::string email, pqxx::connection& _connection);
};

