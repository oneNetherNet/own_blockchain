#include <openssl/ssl.h>
#include <nlohmann/json.hpp>
#include <iostream>

int main() {
    std::cout << "OpenSSL version: " << OpenSSL_version(OPENSSL_VERSION) << std::endl;
    nlohmann::json test = {{"Kate", 12}, 
                           {"Ann", 19},
                           {"Alice", 21}
    };
    std::cout << "JSON: " << test.dump() << std::endl;
    return 0;
}