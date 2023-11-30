
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <crow.h>
#include <inja/inja.hpp>
#include <pqxx/pqxx>

using namespace inja; using namespace crow; using namespace std;

int main()
{

    crow::SimpleApp app;
    inja::Environment env{ "" };

    CROW_ROUTE(app, "/")([]() {
        inja::Environment env{ "" };
        inja::json data;
        std::string result;
        try {
            result = env.render_file("./templates/home.html", data);
        }
        catch (inja::FileError) {
            result = env.render_file("./templates/404.html", data);
        }
        return crow::response(result);
        });

    CROW_ROUTE(app, "/about")([]() {
        inja::Environment env{ "" };
        inja::json data;
        string result;
        try {
            result = env.render_file("./templates/about.html", data);
        }
        catch (inja::FileError) {
            result = env.render_file("./templates/404.html", data);
        }
        return crow::response(result);
        });

    CROW_ROUTE(app, "/admin")([]() {
        inja::Environment env{ "" };
        inja::json data;
        string result;
        try {
            result = env.render_file("./templates/admin.html", data);
        }
        catch (inja::FileError) {
            result = env.render_file("./templates/404.html", data);
        }
        return crow::response(result);
        });

    CROW_ROUTE(app, "/home")([]() {
        inja::Environment env{ "" };
        inja::json data;
        std::string result;
        try {
            result = env.render_file("./templates/home.html", data);
        } 
        catch(inja::FileError) {
            result = env.render_file("./templates/404.html", data);
        } 
        return crow::response(result);
        });

    CROW_ROUTE(app, "/menu")([]() {
        inja::Environment env{ "" };
        inja::json data;
        string result;
        try {
            result = env.render_file("./templates/menu.html", data);
        }
        catch (inja::FileError) {
            result = env.render_file("./templates/404.html", data);
        }
        return crow::response(result);
        });

    CROW_ROUTE(app, "/reserve")([]() {
        inja::Environment env{ "" };
        inja::json data;
        std::string result;
        try {
            result = env.render_file("./templates/reserve.html", data);
        }
        catch (inja::FileError) {
            result = env.render_file("./templates/404.html", data);
        }
        return crow::response(result);
        });

    CROW_ROUTE(app, "/404")([]() {
        inja::Environment env{ "" };
        inja::json data;
        std::string result;
        try {
            result = env.render_file("./templates/404.html", data);
        }
        catch (inja::FileError) {
            result = env.render_file("./templates/404.html", data);
        }

        return crow::response(result);
        });

    CROW_ROUTE(app, "/load_menu/<string>/<int>/").methods("GET"_method)([](std::string reservation, int quantity) {
        inja::json data;
        return crow::response();
        });


    app.port(18080).multithreaded().run();
}
