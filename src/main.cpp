#include <openssl/ssl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include "core/blockchain.h"

int main() {
    Blockchain test;
    test.add("Alice sends Bob 1 NET");
    test.add("Bob sends Eva 1 NET");
    std::cout << test.get_len() << std::endl << test.get_last().get_nonce() << std::endl;
    if (test.is_valid()) std::cout << "is valid";
    else std::cout << "isn't valid";
    return 0;
}