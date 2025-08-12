#pragma once
#include <string>
#include <ctime>

class Block {
private:
	uint64_t id;
	time_t t_stamp;
	std::string prev_hash;
	std::string hash;
	std::string data;
	uint64_t nonce;
	std::string calc_hash_n(uint64_t nonce) const;

public:
	Block(const uint64_t id, const std::string& prev_hash, const std::string& data);

	std::string calc_hash() const;
	void mine(uint32_t dif);

	std::string get_hash() const { return hash; }
	std::string get_prev_hash() const { return prev_hash; }
	uint64_t get_nonce() const { return nonce; }
};