#pragma once
#include <vector>
#include "core/block.h"

class Blockchain {
private:
	std::vector<Block> chain;
	uint32_t dif = 4;
public:
	Blockchain();
	
	void add(std::string data);
	bool is_valid() const;

	Block get_last() const { return chain.back(); }
	uint64_t get_len() const { return chain.size(); }
};