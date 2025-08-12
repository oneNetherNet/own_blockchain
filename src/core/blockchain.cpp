#include "core/blockchain.h"

Blockchain::Blockchain() {
	Block genesis(0, "0", "genesis");
	genesis.mine(dif);
	chain.push_back(genesis);
}

void Blockchain::add(std::string data) {
	Block new_block(static_cast<uint64_t>(chain.size()), 
					chain[chain.size() - 1].get_hash(), 
					data
	);
	new_block.mine(dif);
	chain.push_back(new_block);
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