#include <iostream>
#include <crow.h>
#include <crow/middlewares/session.h>
#include <crow/middlewares/cors.h>
#include <inja/inja.hpp>
#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>
#include <fmt/format.h>
#include <dotenv.h>

using namespace inja; using namespace crow; using namespace std; using namespace dotenv;


pqxx::connection createConnection(){
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

bool checkIfTableExist(std::string table_name, pqxx::work& _work) {
    std::string query = fmt::format("SELECT EXISTS(\
        SELECT 1\
        FROM pg_tables\
        WHERE tablename = '{}')", table_name);
    pqxx::result _result(_work.exec(query));
    return _result[0][0].as<bool>();
}

void createDatabase(pqxx::connection& _connection) {
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
    ;

    std::string create_orders_table = "CREATE TABLE orders("\
        "id SERIAL PRIMARY KEY,"\
        "user_id INT NOT NULL,"\
        "item TEXT NOT NULL,"\
        "confirmed BOOLEAN NOT NULL,"\
        "delivered BOOLEAN NOT NULL,"\
        "completed BOOLEAN NOT NULL,"\
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

bool insertReservation(std::string name, std::string lastname, std::string email, std::string password, pqxx::connection& _connection)
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


bool insertUser(std::string name, std::string lastname, std::string email, std::string password, pqxx::connection& _connection)
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

bool deleteUser(int user_id, pqxx::connection& _connection)
{
    std::string deletion = fmt::format("", user_id);
    pqxx::work _work(_connection);
    _work.exec(deletion);
    try {
        _work.commit();
        return true;
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return false;
    }
}

bool checkIfUserIsActive(int user_id, pqxx::connection& _connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT * FROM clients WHERE id='{0}';", user_id);
    pqxx::result _result(non_tranaction.exec(query));
    if (_result.size() > 0) {
        return _result[0][4].as<bool>();
    }
    return false;
}

bool validateToken(std::string token, int user_id, pqxx::connection& _connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT C.id, T.token FROM clients C JOIN tokens T ON C.id = T.user_id WHERE C.id='{0}';", user_id);
    pqxx::result _result(non_tranaction.exec(query));
    if (_result.size() > 0) {
        return _result[0][1].as<std::string>() == token;
    }
    return false;
}

bool activateUser(int user_id, pqxx::connection& _connection)
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
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    return false;
}

bool validateUser(std::string email, std::string password, pqxx::connection& _connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT id FROM clients WHERE email='{0}' AND password='{1}'", email, password);
    pqxx::result _result(non_tranaction.exec(query));
    return (_result.size() > 0);
}

bool validateEmail(std::string email, pqxx::connection& _connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT * FROM clients WHERE email='{0}'", email);
    pqxx::result _result(non_tranaction.exec(query));
    return (_result.size() == 0);
}

bool createUser(std::string nickname, std::string email, std::string password, std::string name, std::string lastname, pqxx::connection& _connection)
{
    pqxx::work _work(_connection);
    std::string query = fmt::format("INSERT INTO clients()", email);
    pqxx::result _result(_work.exec(query));
    return (_result.size() > 0);
}

bool authenticateBearer(std::string& token, std::string nickname)
{
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{ "secret" })
            .with_issuer("auth0");
        try {
            verifier.verify(decoded);
            return true;
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    return false;
}


std::string getUserNickname(int user_id, pqxx::connection &_connection)
{
    pqxx::nontransaction non_tranaction(_connection);
    std::string query = fmt::format("SELECT name FROM clients WHERE id='{0}';", user_id);
    pqxx::result _result(non_tranaction.exec(query));
    if (_result.size() > 0) {
        return _result[0][0].as<std::string>();
    }
    return "guest";
}



int main()
{

    pqxx::connection _connection = createConnection();
    createDatabase(_connection);

    using Session = crow::SessionMiddleware<crow::InMemoryStore>;
    crow::App<crow::CookieParser, crow::CORSHandler, Session> app{ Session{

        crow::InMemoryStore{}} };
    

    auto& cors = app.get_middleware<crow::CORSHandler>();
    inja::Environment env{ "" };

    CROW_ROUTE(app, "/").methods("GET"_method)([&](const crow::request& req) 
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);

            std::string session_id = ctx.get_cookie("resto_session");
            int user_id = session.get(session_id, 0);
            std::string user_nickname = user_id != 0 ? getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });

            inja::json data;
            std::string result;

            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;

            try {
                result = env.render_file("./templates/home.html", data);
            }
            catch (inja::FileError) { 
                result = env.render_file("./templates/404.html", data);
            }
            return crow::response(result);
        });

    CROW_ROUTE(app, "/about").methods("GET"_method)([&](const crow::request& req) 
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);

            std::string session_id = ctx.get_cookie("resto_session");
            int user_id = session.get(session_id, 0);
            std::string user_nickname = user_id != 0 ? getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });
            
            inja::json data;
            std::string result;
            
            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            try {
                result = env.render_file("./templates/about.html", data);
            }
            catch (inja::FileError) {
                result = env.render_file("./templates/404.html", data);
            }
            return crow::response(result);
        });

    CROW_ROUTE(app, "/admin").methods("GET"_method)([&](const crow::request& req)
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);

            std::string session_id = ctx.get_cookie("resto_session");
            int user_id = session.get(session_id, 0);
            std::string user_nickname = user_id != 0 ? getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });

            inja::json data;
            std::string result;

            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            try {
                result = env.render_file("./templates/admin.html", data);
            }
            catch (inja::FileError) {
                result = env.render_file("./templates/404.html", data);
            }
            return crow::response(result);
        });

    CROW_ROUTE(app, "/home").methods("GET"_method, "POST"_method)([&](const crow::request& req) 
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);

            std::string session_id = ctx.get_cookie("resto_session");
            int user_id = session.get(session_id, 0);
            std::string user_nickname = user_id != 0 ? getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });

            inja::json data;
            std::string result;

            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            try {
                result = env.render_file("./templates/home.html", data);
            } 
            catch(inja::FileError) {
                result = env.render_file("./templates/404.html", data);
            } 
            return crow::response(result);
        });

    CROW_ROUTE(app, "/menu").methods("GET"_method)([&](const crow::request& req) 
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);

            std::string session_id = ctx.get_cookie("resto_session");
            int user_id = session.get(session_id, 0);
            std::string user_nickname = user_id != 0 ? getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });

            inja::json data;
            std::string result;

            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            try {
                result = env.render_file("./templates/menu.html", data);
            }
            catch (inja::FileError) {
                result = env.render_file("./templates/404.html", data);
            }
            return crow::response(result);
        });

    CROW_ROUTE(app, "/reserve_table/<string>/<string>").methods("GET"_method)([&](const crow::request& req, std::string table, std::string token)
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);

            std::string session_id = ctx.get_cookie("resto_session");
            int user_id = session.get(session_id, 0);
            std::string user_nickname = user_id != 0 ? getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });

            inja::json data;
            std::string result;

            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            try {
                result = env.render_file("./templates/reserve.html", data);
            }
            catch (inja::FileError) {
                result = env.render_file("./templates/404.html", data);
            }
            return crow::response(result);
        });

    CROW_ROUTE(app, "/order/<string>/<string>/").methods("GET"_method)([&](const crow::request& req, std::string id, std::string token) 
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);

            std::string session_id = ctx.get_cookie("resto_session");
            int user_id = session.get(session_id, 0);
            std::string user_nickname = user_id != 0 ? getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });

            inja::json data;
            std::string result;

            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            try {
                result = env.render_file("./templates/reserve.html", data);
            }
            catch (inja::FileError) {
                result = env.render_file("./templates/404.html", data);
            }
            return crow::response(result);
        });

    CROW_ROUTE(app, "/login").methods("GET"_method, "POST"_method)([&](const crow::request& req) 
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);

            std::string session_id = ctx.get_cookie("resto_session");
            int user_id = session.get(session_id, 0);
            std::string user_nickname = user_id != 0 ? getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });

            inja::json data;
            std::string result;

            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            try {
                result = env.render_file("./templates/login.html", data);
            }
            catch (inja::FileError) {
                result = env.render_file("./templates/404.html", data);
            }
            return crow::response(result);
        });

    CROW_ROUTE(app, "/register").methods("POST"_method, "GET"_method)([&](const crow::request& req) 
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);

            std::string session_id = ctx.get_cookie("resto_session");
            int user_id = session.get(session_id, 0);
            std::string user_nickname = user_id != 0 ? getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });

            inja::json data;
            std::string result;

            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            try {
                result = env.render_file("./templates/register.html", data);
            }
            catch (inja::FileError) {
                result = env.render_file("./templates/404.html", data);
            }
            return crow::response(result);
        });

    CROW_ROUTE(app, "/404").methods("GET"_method)([&](const crow::request& req) 
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);

            std::string session_id = ctx.get_cookie("resto_session");
            int user_id = session.get(session_id, 0);
            std::string user_nickname = user_id != 0 ? getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });

            inja::json data;
            std::string result;

            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            try {
                result = env.render_file("./templates/404.html", data);
            }
            catch (inja::FileError) {
                result = env.render_file("./templates/404.html", data);
            }
            return crow::response(result);
        });

    CROW_ROUTE(app, "/activation").methods("POST"_method)([&](const crow::request& req) 
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);

            std::string session_id = ctx.get_cookie("resto_session");
            int user_id = session.get(session_id, 0);
            std::string user_nickname = user_id != 0 ? getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });

            inja::json data;
            std::string result;

            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            try {
                result = env.render_file("./templates/activation.html", data);
            }
            catch (inja::FileError) {
                result = env.render_file("./templates/404.html", data);
            }
            return crow::response(result);
        });

    CROW_ROUTE(app, "/load_menu/<string>/<int>/").methods("GET"_method)([&](const crow::request& req, std::string reservation, int quantity) 
        {
            auto& session = app.get_context<Session>(req);
            inja::json data;
            return crow::response();
        });

    CROW_ROUTE(app, "/validate_login/<string>/<string>/").methods("GET"_method, "POST"_method)([&](const crow::request& req, std::string email, std::string password) 
        {
            auto& session = app.get_context<Session>(req);
            auto& ctx = app.get_context<crow::CookieParser>(req);
            inja::json data;
            if (validateUser(email, password, _connection))
            {
                auto token = jwt::create()
                    .set_issuer(email)
                    .set_type("JWS")
                    .set_issued_at(std::chrono::system_clock::now())
                    .set_expires_at(std::chrono::system_clock::now() + std::chrono::days{ 1 })
                    .set_payload_claim("sample", jwt::claim(std::string("test")))
                    .sign(jwt::algorithm::hs256{ "secret" });
                ctx.set_cookie("resto_session", password).max_age(30).path("/").httponly();
                
                return crow::response(token);
            }
            else {
                return crow::response();
            }
        });

    CROW_ROUTE(app, "/register_user/<string>/<string>/<string>/<string>/").methods("POST"_method)([&](const crow::request& req, std::string name, std::string lastname, std::string email, std::string password) 
        {
            auto& session = app.get_context<Session>(req);
            inja::json data;
            if (validateEmail(email, _connection)) {
                insertUser(name, lastname, email, password, _connection);
                
            }
            return crow::response();
        });

    CROW_ROUTE(app, "/activate_account/<string>/<int>/").methods("POST"_method)([&](const crow::request& req, std::string activation_token, int user_id)
        {
            auto& session = app.get_context<Session>(req);
            inja::json data;
            if (validateToken(activation_token, user_id, _connection)) {
                activateUser(user_id, _connection);
            }
            return crow::response();
        });

    CROW_ROUTE(app, "/make_order/<string>/<int>/<string>/").methods("POST"_method)([&](const crow::request& req, std::string user_token, int user_id, std::string order_date)
        {
            auto& session = app.get_context<Session>(req);
            inja::json data;
            return crow::response();
        });

    CROW_ROUTE(app, "/confirm_order/<string>/<int>/").methods("POST"_method)([&](const crow::request& req, std::string admin_token, int order_id)
        {
            auto& session = app.get_context<Session>(req);
            inja::json data;
            return crow::response();
        });

    CROW_ROUTE(app, "/deliver_order/<string>/<int>/").methods("POST"_method)([&](const crow::request& req, std::string admin_token, int order_id)
        {
            auto& session = app.get_context<Session>(req);
            inja::json data;
            return crow::response();
        });

    CROW_ROUTE(app, "/complete_order/<string>/<int>/").methods("POST"_method)([&](const crow::request& req, std::string admin_token, int order_id)
        {
            auto& session = app.get_context<Session>(req);
            inja::json data;
            return crow::response();
        });

    CROW_ROUTE(app, "/abort_order/<string>/<int>/").methods("POST"_method)([&](const crow::request& req, std::string admin_token, int order_id)
        {
            auto& session = app.get_context<Session>(req);
            inja::json data;
            return crow::response();
        });

    CROW_ROUTE(app, "/make_reservation/<string>/<string>/<string>/").methods("POST"_method)([&](const crow::request& req, std::string user_token, std::string reservation_date, std::string leave_date)
        {
            auto& session = app.get_context<Session>(req);
            inja::json data;
            return crow::response();
        });


    CROW_ROUTE(app, "/confirm_reservation/<string>/<int>/").methods("POST"_method)([&](const crow::request& req, std::string admin_token, int reservation_id)
        {
            auto& session = app.get_context<Session>(req);
            inja::json data;
            return crow::response();
        });

    CROW_ROUTE(app, "/start_reservation/<string>/<int>/").methods("POST"_method)([&](const crow::request& req, std::string admin_token, int reservation_id)
        {
            auto& session = app.get_context<Session>(req);
            inja::json data;
            return crow::response();
        });

    CROW_ROUTE(app, "/complete_reservation/<string>/<int>/").methods("POST"_method)([&](const crow::request& req, std::string admin_token, int reservation_id)
        {
            auto& session = app.get_context<Session>(req);
            inja::json data;
            return crow::response();
        });

    CROW_ROUTE(app, "/abort_reservation/<string>/<int>/").methods("POST"_method)([&](const crow::request& req, std::string admin_token, int reservation_id)
        {
            auto& session = app.get_context<Session>(req);
            inja::json data;
            return crow::response();
        });

    app.port(18080).multithreaded().run();
}
