#pragma once
#include <pqxx/pqxx>
#include <iostream>

class session
{
	public:
		std::unordered_map<int, std::string> _sessions;

		pqxx::connection createConnection();

		session();

		bool validateUserSession(int user_id);

		bool addUserSession(int user_id, std::string join_date, std::string release_date);

		bool endUserSession(int user_id);

		bool extendUserSession(int user_id);
};

