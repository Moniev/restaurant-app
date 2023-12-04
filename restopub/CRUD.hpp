#pragma once
#include <pqxx/pqxx>
class CRUD 
{
	private:
		pqxx::connection;

	public:
		CRUD(pqxx::connection) {
			this->connection = connection;
		}

		bool checkIfUsersExist()
		{
			
		}

		bool insertItem()
		{

		}

		bool


};