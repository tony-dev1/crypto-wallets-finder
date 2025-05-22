/* 
* This file is part of the Trezor Crypto library.
* 
* Copyright (c) 2021 SatoshiLabs s.r.o. (https://www.satoshilabs.com)
* 
* This code is licensed under the MIT License (MIT), see LICENSE file for details.
*/


#include "address_generator.h"
#include <sstream>
#include <iomanip>
#include <string.h>

extern "C" {

    void mnemonic_to_seed(const char *mnemonic, const char *passphrase,
        uint8_t seed[512 / 8],
        void (*progress_callback)(uint32_t current,
                                  uint32_t total));

    int hdnode_from_seed(const uint8_t *seed, int seed_len, const char *curve,
                                    HDNode *out);

    int hdnode_private_ckd(HDNode *inout, uint32_t i);

}


std::string bytes_to_hex_string(const uint8_t* data, size_t len) {
    std::stringstream ss;
    for (size_t i = 0; i < len; i++) {
        ss << std::setw(2) << std::setfill('0') << std::hex << (int)data[i];
    }
    return ss.str();
}

wallet generate_evm_wallet(std::string mnemonic){

    uint8_t seed[64];
    HDNode node;

    mnemonic_to_seed(mnemonic.c_str(), "", seed, NULL);
    hdnode_from_seed(seed, sizeof(seed), SECP256K1_NAME, &node);

    HDNode node_eth = node;
    hdnode_private_ckd_prime(&node_eth, 44);
    hdnode_private_ckd_prime(&node_eth, 60);
    hdnode_private_ckd_prime(&node_eth, 0);
    hdnode_private_ckd(&node_eth, 0);
    hdnode_private_ckd(&node_eth, 0);
    hdnode_fill_public_key(&node_eth);


    uint8_t pub_key[65];
    const ecdsa_curve *curve = &secp256k1;
    ecdsa_uncompress_pubkey(curve, node_eth.public_key, pub_key);
	
    uint8_t keccak256_hash[32];
    keccak_256(pub_key + 1, 64, keccak256_hash);
    std::string eth_address = "0x" + bytes_to_hex_string(keccak256_hash + 12, 20);
	
    wallet wal;
    wal.address = eth_address;
    wal.public_key = bytes_to_hex_string(node_eth.public_key, 33);
    wal.private_key = bytes_to_hex_string(node_eth.private_key, 32);
    wal.mnemonic = mnemonic;
    return wal;
}



wallet generate_bitcoin_wallet(const std::string mnemonic){

    uint8_t seed[64];
    HDNode node;

    mnemonic_to_seed(mnemonic.c_str(), "", seed, NULL);
    hdnode_from_seed(seed, sizeof(seed), SECP256K1_NAME, &node);
	
    HDNode node84 = node;
    hdnode_private_ckd_prime(&node84, 84);
    hdnode_private_ckd_prime(&node84, 0);
    hdnode_private_ckd_prime(&node84, 0);
    hdnode_private_ckd(&node84, 0);
    hdnode_private_ckd(&node84, 0);
    hdnode_fill_public_key(&node84);


    uint8_t sha[32], hash160[20];
    sha256_Raw(node84.public_key, 33, sha);
    ripemd160(sha, 32, hash160); 

    char address_bech32[100];
    segwit_addr_encode(address_bech32, "bc", 0, hash160, 20);

    HDNode node44 = node;
    hdnode_private_ckd_prime(&node44, 44);
    hdnode_private_ckd_prime(&node44, 0);
    hdnode_private_ckd_prime(&node44, 0);
    hdnode_private_ckd(&node44, 0);
    hdnode_private_ckd(&node44, 0);
    hdnode_fill_public_key(&node44);

    sha256_Raw(node44.public_key, 33, sha);
    ripemd160(sha, 32, hash160);

    uint8_t legacy_addr[21];
    legacy_addr[0] = 0x00;
    memcpy(legacy_addr + 1, hash160, 20);

    char address_legacy[64];
    base58_encode_check(legacy_addr, 21, HASHER_SHA2D, address_legacy, sizeof(address_legacy));

    HDNode node49 = node;
    hdnode_private_ckd_prime(&node49, 49);
    hdnode_private_ckd_prime(&node49, 0);
    hdnode_private_ckd_prime(&node49, 0);
    hdnode_private_ckd(&node49, 0);
    hdnode_private_ckd(&node49, 0);
    hdnode_fill_public_key(&node49);

    sha256_Raw(node49.public_key, 33, sha);
    ripemd160(sha, 32, hash160);

 
    uint8_t redeem_script[22];
    redeem_script[0] = 0x00; 
    redeem_script[1] = 0x14;
    memcpy(redeem_script + 2, hash160, 20);

    sha256_Raw(redeem_script, 22, sha);
    ripemd160(sha, 32, hash160);

    uint8_t sh_addr[21];
    sh_addr[0] = 0x05;
    memcpy(sh_addr + 1, hash160, 20);

    char address_p2sh[64];
    base58_encode_check(sh_addr, 21, HASHER_SHA2D, address_p2sh, sizeof(address_p2sh));


    wallet wal;
    wal.mnemonic = mnemonic;
    wal.private_key = bytes_to_hex_string(node84.private_key, 32);
    wal.public_key = bytes_to_hex_string(node84.public_key, 33);
    wal.address = address_bech32;
    wal.address_legacy = address_legacy;
    wal.address_p2sh = address_p2sh;
    return wal;
}
