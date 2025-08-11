#include "core/block.h"
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>

std::string sha256(std::string str) {
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str.c_str(), str.size());
	SHA256_Final(hash, &sha256);

	std::stringstream ss;
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
	}
	return ss.str();
}

Block::Block(const uint64_t id, const std::string& prev_hash, const std::string& data) :
	id(id), prev_hash(prev_hash), data(data) {
	t_stamp = std::time(nullptr);
	hash = calc_hash();
}

std::string Block::calc_hash() const {
	std::stringstream ss;
	ss << id << t_stamp << prev_hash << data << nonce;
	return sha256(ss.str());
}