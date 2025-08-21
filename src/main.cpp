#include <iostream>
#include "core/blockchain.h"
#include "core/transactions.h"

int main() {
    Transaction test;

    EC_KEY* keys = test.keypair_gen();
    in test_in;
    out test_out;

    test_in.prev_txid = "prev_txid";
    test_in.out_index = 0;
    test_out.address = "0x9240fadc720beff";
    test_out.value = 3;

    std::vector<in> vin = {test_in};
    std::vector<out> vout = {test_out};

    test = Transaction(vin, vout);

    test.sign_input(0, keys);
    std::cout << test.get_sign() << std::endl;

    if (test.verify_sign_input(0, keys))
        std::cout << "i can go to bed";
    else 
        std::cout << "dermo vonuchee";

    return 0;
}