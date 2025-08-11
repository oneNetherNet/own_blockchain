#include <openssl/ssl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include "core/block.h"

int main() {
    Block test(0, "0", "genesis");
    std::cout << "genesis hash: " << test.get_hash();
    return 0;
}