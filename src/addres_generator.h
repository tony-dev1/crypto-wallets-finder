#pragma once

#include "utils.h"

struct wallet {
	std::string address;
	std::string public_key;
	std::string private_key;
	std::string mnemonic;
};


struct BIP32Node {
    std::vector<unsigned char> privkey;
    std::vector<unsigned char> chain_code;
    uint32_t index = 0;
    uint32_t fingerprint = 0;
    std::vector<unsigned char> pubkey;
    std::vector<unsigned char> pubkey_hash;
};

BIP32Node derive_master_key(const std::vector<uint8_t>& seed);
BIP32Node derive_key(const BIP32Node& parent_node, const std::string& path);
std::vector<unsigned char> private_to_public_key(const std::vector<unsigned char>& privkey);

wallet generate_evm_wallet(const std::string& mnemonic);
wallet generate_bitcoin_wallet(const std::string& mnemonic);
