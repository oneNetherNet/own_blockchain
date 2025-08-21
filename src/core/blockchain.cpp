#include "core/blockchain.h"

Blockchain::Blockchain() {
	Block genesis(0, "0", "genesis");
	genesis.mine(dif);
	chain.push_back(genesis);
	hash_map["0"] = 0;
}

void Blockchain::add(std::string data) {
	Block new_block(chain.size(), 
					chain[chain.size() - 1].get_hash(), 
					data
	);
	new_block.mine(dif);
	chain.push_back(new_block);
	hash_map[new_block.get_hash()] = chain.size() - 1;
}

bool Blockchain::is_valid() const {
    for (size_t i = 1; i < chain.size(); ++i) {

        if (chain[i].get_hash() != chain[i].calc_hash())
            return false;

        if (chain[i].get_prev_hash() != chain[i - 1].get_hash())
            return false;
    }
    return true;
}

Block Blockchain::get_block(const std::string & hash) const {
	auto pair = hash_map.find(hash);
	return (pair != hash_map.end()) ? chain[pair -> second] : throw std::string("Invalid index");
}