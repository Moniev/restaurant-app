#include <iostream>
#include <crow.h>
#include <crow/middlewares/session.h>
#include <crow/middlewares/cors.h>
#include <inja/inja.hpp>
#include <pqxx/pqxx>
#include <jwt-cpp/jwt.h>
#include <fmt/format.h>
#include <dotenv.h>
#include "CRUD.hpp"
#include "utils.hpp"

using namespace inja; using namespace crow; using namespace std; using namespace dotenv;

int main()
{
    CRUD crud;
    pqxx::connection _connection = crud.createConnection();
    crud.createDatabase(_connection);

    using Session = crow::SessionMiddleware<crow::InMemoryStore>;
    crow::App<crow::CookieParser, crow::CORSHandler, Session> app{ Session{crow::CookieParser::Cookie("session").max_age(24 * 60 * 60).path("/"), 
        4,

        crow::InMemoryStore{}} };
    

    auto& cors = app.get_middleware<crow::CORSHandler>();
    inja::Environment env{ "" };

    CROW_ROUTE(app, "/").methods("GET"_method)([&](const crow::request& req) 
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);


            std::string session_id = ctx.get_cookie("resto_session");
            int user_id = session.get(session_id, 0);

            std::string user_nickname = user_id != 0 ? crud.getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });

            inja::json data;
            std::string result;

            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            cout << user_nickname << endl;
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

            std::string user_nickname = user_id != 0 ? crud.getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });
            
            inja::json data;
            std::string result;
            
            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            cout << user_nickname << endl;
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
            cout << session_id << endl;
            int user_id = session.get(session_id, 0);
            cout << user_id << endl;
            std::string user_nickname = user_id != 0 ? crud.getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });

            inja::json data;
            std::string result;

            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            cout << user_nickname << endl;
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
            std::string user_nickname = user_id != 0 ? crud.getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });

            inja::json data;
            std::string result;

            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            cout << user_nickname << endl;
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
            cout << session_id << endl;
            int user_id = session.get(session_id, 0);
            cout << user_id << endl;
            std::string user_nickname = user_id != 0 ? crud.getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });

            inja::json data;
            std::string result;

            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            cout << user_nickname << endl;
            try {
                result = env.render_file("./templates/menu.html", data);
            }
            catch (inja::FileError) {
                result = env.render_file("./templates/404.html", data);
            }
            return crow::response(result);
        });

    CROW_ROUTE(app, "/reserve").methods("GET"_method)([&](const crow::request& req)
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);

            std::string session_id = ctx.get_cookie("resto_session");
            cout << session_id << endl;
            int user_id = session.get(session_id, 0);
            cout << user_id << endl;
            std::string user_nickname = user_id != 0 ? crud.getUserNickname(user_id, _connection) : "guest";

            session.apply("views", [](int v) {
                return v + 1;
                });

            inja::json data;
            std::string result;

            int views = session.get("views", 0);

            data["views"] = views;
            data["current_user"] = user_nickname;
            cout << user_nickname << endl;
            try {
                result = env.render_file("./templates/menu.html", data);
            }
            catch (inja::FileError) {
                result = env.render_file("./templates/404.html", data);
            }
            return crow::response(result);
        });

    CROW_ROUTE(app, "/reserve_table/<string>/<string>").methods("POST"_method)([&](const crow::request& req, std::string table, std::string token)
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);

            std::string session_id = ctx.get_cookie("resto_session");
            cout << session_id << endl;
            int user_id = session.get(session_id, 0);
            cout << user_id << endl;
            std::string user_nickname = user_id != 0 ? crud.getUserNickname(user_id, _connection) : "guest";

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

    CROW_ROUTE(app, "/order/<string>/<string>/").methods("POST"_method)([&](const crow::request& req, std::string id, std::string token) 
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);

            std::string session_id = ctx.get_cookie("resto_session");
            int user_id = session.get(session_id, 0);
            std::string user_nickname = user_id != 0 ? crud.getUserNickname(user_id, _connection) : "guest";

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
            std::string user_nickname = user_id != 0 ? crud.getUserNickname(user_id, _connection) : "guest";

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

    CROW_ROUTE(app, "/register").methods("POST"_method)([&](const crow::request& req) 
        {
            auto& ctx = app.get_context<crow::CookieParser>(req);
            auto& session = app.get_context<Session>(req);

            std::string session_id = ctx.get_cookie("resto_session");
            int user_id = session.get(session_id, 0);
            std::string user_nickname = user_id != 0 ? crud.getUserNickname(user_id, _connection) : "guest";

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
            std::string user_nickname = user_id != 0 ? crud.getUserNickname(user_id, _connection) : "guest";

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
            std::string user_nickname = user_id != 0 ? crud.getUserNickname(user_id, _connection) : "guest";

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

    CROW_ROUTE(app, "/validate_login/<string>/<string>/").methods("POST"_method)([&](const crow::request& req, std::string email, std::string password) 
        {
            auto& session = app.get_context<Session>(req);
            auto& ctx = app.get_context<crow::CookieParser>(req);
            inja::json data;
            if (crud.validateUser(email, password, _connection))
            {
                pqxx::result user = crud.getUserByEmail(email, _connection);
                std::string user_nickname = user[0][1].as<std::string>();
                int user_id = user[0][0].as<int>();
                auto token = jwt::create()
                    .set_issuer(user_nickname)
                    .set_type("JWT")
                    .set_id(user_nickname)
                    .set_issued_at(std::chrono::system_clock::now())
                    .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{ 36000 })
                    .set_payload_claim("resto__", jwt::claim(std::string("resto__jwt")))
                    .sign(jwt::algorithm::hs256{ "courvogne__salt" });
                session.set(token, user_id);
                ctx.set_cookie("resto_session", token).max_age(60).path("/").httponly();
                
                return crow::response(token);
            }
            else {
                return crow::response(500);
            }
        });

    CROW_ROUTE(app, "/logout_user/").methods("POST"_method)([&](const crow::request& req)
        {
            auto& session = app.get_context<Session>(req);
            auto& ctx = app.get_context<crow::CookieParser>(req);

            std::string session_id = ctx.get_cookie("resto_session");

            if(session.get(session_id, 0) != 0){
                session.remove(session_id);
                return crow::response(200);
            }
            else {
                return crow::response(500);
            }
        });

    CROW_ROUTE(app, "/register_user/<string>/<string>/<string>/<string>/<string>/").methods("POST"_method)([&](const crow::request& req, std::string nickname, std::string name, std::string lastname, std::string email, std::string password)
        {
            auto& session = app.get_context<Session>(req);
            inja::json data;
            if (crud.validateEmail(email, _connection) && crud.validateNickname(nickname, _connection)) {
                if (crud.insertUser(nickname, name, lastname, email, password, _connection)) {
                    return crow::response(200);
                }
            }
            return crow::response(500);
        });

    CROW_ROUTE(app, "/activate_account/<string>/<int>/").methods("POST"_method)([&](const crow::request& req, std::string activation_token, int user_id)
        {
            auto& session = app.get_context<Session>(req);
            inja::json data;
            if (crud.validateToken(activation_token, user_id, _connection)) {
                if (crud.activateUser(user_id, _connection)) {
                    return crow::response(200);
                }
            }
            return crow::response(500);
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
