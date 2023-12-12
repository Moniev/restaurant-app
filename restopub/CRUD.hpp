#pragma once
#include <pqxx/pqxx>
#include <iostream>

class CRUD
{
	public:
		CRUD();

		pqxx::connection createConnection();

		void createDatabase(pqxx::connection &_connection);
		
		bool insertToken(std::string token, int user_id, pqxx::connection &_connection);

		bool validateToken(std::string token, int user_id, pqxx::connection &_connection);

		bool deleteUser(int user_id, pqxx::connection &_connection);

		bool activateUser(int user_id, pqxx::connection &_connection);
	
		bool insertUser(std::string nickname, std::string name, std::string lastname, std::string email, std::string password, pqxx::connection &_connection);

		std::string getUserNickname(int user_id, pqxx::connection &_connection);

		pqxx::result getUserByEmail(std::string email, pqxx::connection  &_connection);

		int getUserId(std::string email, pqxx::connection &_connection);
		
		bool validateEmail(std::string email, pqxx::connection &_connection);

		bool validateUser(std::string email, std::string password, pqxx::connection &_connection);

		bool validateNickname(std::string nickname, pqxx::connection &_connection);

		bool insertOrder(int user_id, std::string order_date, pqxx::connection &_connection);

		bool confirmOrder(int order_id, pqxx::connection &_connection);

		bool deliverOrder(int order_id, pqxx::connection &_connection);

		bool completeOrder(int order_id, pqxx::connection &_connection);

		bool insertReservation(int user_id, std::string reservation_date, std::string leave_date, pqxx::connection &_connection);

		bool confirmReservation(int reservation_id, pqxx::connection &_connection);

		bool completeReservation(int reservation_id, pqxx::connection &_connection);

		bool abortReservation(int reservation_id, pqxx::connection &_connection);

		bool checkIfUserIsActive(int user_id, pqxx::connection &_connection);

		bool checkIfTableExist(std::string table_name, pqxx::work &_work);
};

