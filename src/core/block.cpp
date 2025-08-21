#include "core/block.h"
#include <iomanip>
#include <sstream>
#include <thread>
#include <atomic>
#include <vector>
#include <openssl/sha.h>

std::string sha256(std::string str) {
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str.c_str(), str.size());
	SHA256_Final(hash, &sha256);

	std::stringstream ss;
	for (size_t i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
	}
	return ss.str();
}

Block::Block(const uint64_t id, const std::string& prev_hash, const std::string& data) :
	id(id), prev_hash(prev_hash), data(data) {
	t_stamp = std::time(nullptr);
	hash = calc_hash();
}

void Block::mine(uint32_t difficulty) {
    std::string target(difficulty, '0');
    std::atomic<bool> found(false);
    const uint32_t num_threads = std::thread::hardware_concurrency();

    auto worker = [&](uint64_t start) {
        uint64_t local_nonce = start;
        std::string local_hash;
        while (!found) {
            local_hash = calc_hash_n(local_nonce);
            if (local_hash.substr(0, difficulty) == target) {
                found = true;
                nonce = local_nonce;
                hash = local_hash;
                break;
            }
            local_nonce += num_threads;
            if (!local_nonce) t_stamp = std::time(nullptr);
        }
    };

    std::vector<std::thread> threads;
    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back(std::thread(worker, i));
    }
    for (auto& t : threads) {
        t.join();
    }
}

std::string Block::calc_hash() const {
	std::stringstream ss;
	ss << id << t_stamp << prev_hash << data << nonce;
	return sha256(ss.str());
}

std::string Block::calc_hash_n(uint64_t nonce) const {
    std::stringstream ss;
    ss << id << t_stamp << prev_hash << data << nonce;
    return sha256(ss.str());
}

nlohmann::json Block::serialize() const{
    return {
        {"id", id},
        {"time", t_stamp},
        {"prev", prev_hash},
        {"hash", hash},
        {"data", data},
        {"nonce", nonce}
    };
}

Block Block::deserialize(const nlohmann::json& j) const{
    Block new_block(j["id"], j["prev"], j["data"]);
    new_block.t_stamp = j["time"];
    new_block.hash = j["hash"];
    new_block.nonce = j["nonce"];
    return new_block;
}