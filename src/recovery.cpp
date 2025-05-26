#include "recovery.h"

std::vector<std::string> load_wordlist(const std::string& filename) {
    std::vector<std::string> words;
    std::ifstream file(filename);
    std::string word;
    while (file >> word) {
        words.push_back(word);
    }
    return words;
}

std::vector<std::string> split_words(const std::string& str, char delim) {
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> tokens;
    while (getline(ss, item, delim)) tokens.push_back(item);
    return tokens;
}

std::string join(const std::vector<std::string>& vec, const std::string& delim) {
    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i != vec.size() - 1) oss << delim;
    }
    return oss.str();
}

wallet brute_force_seed_phrase(std::vector<std::string>& pattern,
                 const std::vector<std::string>& wordlist,
                 const std::vector<int>& missing_indices,
                 const std::string& target_address,
                 size_t depth,
                 size_t& count) {

    if (depth == missing_indices.size()) {
        std::string mnemonic = join(pattern);
        wallet wal = generate_bitcoin_wallet(mnemonic, false);

        ++count;
        if (count % 1000 == 0) {
            std::cout << "[~] Checked: " << count << " - " << mnemonic;

            if(target_address[0] == '1'){
                std::cout << wal.address_legacy;
            }
            else if(target_address[0] == '3'){
                std::cout << wal.address_p2sh;
            }
            else{
                std::cout << wal.address;
            }

            std::cout << std::endl;
        }

        if (wal.address == target_address ||
            wal.address_legacy == target_address ||
            wal.address_p2sh == target_address) {

            std::cout << "Found!" << std::endl;
            std::cout << "Mnemonic: " << mnemonic << std::endl;
            std::cout << "Address: " << wal.address << std::endl;
            std::exit(0);
        }

        return wal;
    }

    int idx = missing_indices[depth];
    for (const std::string& word : wordlist) {
        pattern[idx] = word;
        brute_force_seed_phrase(pattern, wordlist, missing_indices, target_address, depth + 1, count);
    }
}



wallet brute_find_wallet(const std::string& mnemonic, const std::string& target_address) {
            uint8_t seed[64];
            mnemonic_to_seed(mnemonic.c_str(), "", seed, NULL);
    HDNode root;
    hdnode_from_seed(seed, sizeof(seed), SECP256K1_NAME, &root);
    uint8_t sha[32], hash160[20];
    HDNode node; 

        for (int index = 0; index < 10000000; ++index) {
            std::string generated_address = "";

            if(target_address[0] == '1'){
                node = root;
                hdnode_private_ckd_prime(&node, 44);
                hdnode_private_ckd_prime(&node, 0);
                hdnode_private_ckd_prime(&node, 0);
                hdnode_private_ckd(&node, 0);
                hdnode_private_ckd(&node, index);

                hdnode_fill_public_key(&node);

                sha256_Raw(node.public_key, 33, sha);
                ripemd160(sha, 32, hash160);

                uint8_t legacy_addr[21];
                legacy_addr[0] = 0x00;
                memcpy(legacy_addr + 1, hash160, 20);

                char address_legacy[64];
                base58_encode_check(legacy_addr, 21, HASHER_SHA2D, address_legacy, sizeof(address_legacy));
          
                generated_address = address_legacy;

                std::cout << "address: " << generated_address << " | path: m/44'/0'/0'/0/" << index << "\n";
            }
            else if(target_address[0] == '3'){
                node = root;
                hdnode_private_ckd_prime(&node, 49);
                hdnode_private_ckd_prime(&node, 0);
                hdnode_private_ckd_prime(&node, 0);
                hdnode_private_ckd(&node, 0);
                hdnode_private_ckd(&node, index);
                hdnode_fill_public_key(&node);

                sha256_Raw(node.public_key, 33, sha);
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
                generated_address = address_p2sh;

                std::cout << "address: " << generated_address << " | path: m/49'/0'/0'/0/" << index << "\n";
            }
            else if(target_address[0] == 'b'){
                node = root;
                hdnode_private_ckd_prime(&node, 84);
                hdnode_private_ckd_prime(&node, 0);
                hdnode_private_ckd_prime(&node, 0);
                hdnode_private_ckd(&node, 0);
                hdnode_private_ckd(&node, index);
                hdnode_fill_public_key(&node);
                uint8_t sha[32], hash160[20];
                sha256_Raw(node.public_key, 33, sha);
                ripemd160(sha, 32, hash160); 
                char address_bech32[100];
                segwit_addr_encode(address_bech32, "bc", 0, hash160, 20);
                generated_address = address_bech32;

                std::cout << "address: " << generated_address << " | path: m/84'/0'/0'/0/" << index << "\n";
            }

            if (strcmp(generated_address.c_str(), target_address.c_str()) == 0) {
                wallet found;
                found.mnemonic = mnemonic;
                found.private_key = bytes_to_hex_string(node.private_key, 32);
                found.public_key = bytes_to_hex_string(node.public_key, 33);
                found.address = generated_address;

                 std::cout << "Found!" << std::endl;
                 std::cout << "Mnemonic: " << mnemonic << std::endl;
                 std::cout << "Address: " << target_address << std::endl;
                 if(target_address[0] == '1'){
                    std::cout << "Path: m/44'/0'/0'/0/" << index << std::endl;
                 }
                 else if(target_address[0] == '3'){
                    std::cout << "Path: m/49'/0'/0'/0/" << index << std::endl;
                 }
                 else{
                    std::cout << "Path: m/84'/0'/0'/0/" << index << std::endl;
                 }

                return found;
            }
        }
    std::cout << "Address not found.\n";
}
