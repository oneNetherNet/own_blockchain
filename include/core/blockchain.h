#pragma once
#include <vector>
#include "core/block.h"
#include <unordered_map>

class Blockchain {
private:
	std::vector<Block> chain;
	std::unordered_map<std::string, uint32_t> hash_map;
	uint32_t dif = 3;

public:
	Blockchain();
	
	void add(std::string data);
	bool is_valid() const;

	Block get_last() const { return chain.back(); }
	uint64_t get_len() const { return chain.size(); }
	Block get_block(const std::string& hash) const;
};