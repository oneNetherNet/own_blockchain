#pragma once
#include <string>
#include <vector>
#include <openssl/ec.h>

struct in{
	std::string pub_key;
	std::string sign;
	std::string prev_txid;
	uint32_t out_index;
	explicit in() : pub_key(""), sign(""), prev_txid(""), out_index(NULL) {}
};

struct out {
	uint64_t value;
	std::string address;
	explicit out() : value(NULL), address("") {}
};

class Transaction {
private:
	std::vector<in> vin;
	std::vector<out> vout;
	std::string txid;

public:
	Transaction() {}
	Transaction(std::vector<in> vin, std::vector<out> vout) : vin(vin), vout(vout), txid("") {}

	EC_KEY* keypair_gen() const;
	std::string get_pub_hex(EC_KEY* key) const;
	std::string get_prv_hex(EC_KEY* key) const;
	std::string get_txid() const;

	std::string sign(const std::vector<unsigned char>& msg, EC_KEY* prv_key) const;
	bool verify_sign(const std::vector<unsigned char>& msg, const std::string& sign, EC_KEY* pub_key) const;
	void sign_input(int index, EC_KEY* prv_key);
	bool verify_sign_input(int index, EC_KEY* pub_key) const;
	std::string get_sign() const { return vin[0].sign; }
	std::string serialize() const;
	std::string serialize_for_sign(int index) const;
};