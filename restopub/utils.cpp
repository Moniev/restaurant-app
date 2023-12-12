#include "utils.hpp"
#include <jwt-cpp/jwt.h>

namespace utils {
    bool authenticateBearer(std::string token, std::string nickname)
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
}