#pragma once

#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <openssl/sha.h>

std::vector<std::string> bip39_words;

bool load_bip39_words()
{
	std::string file_path = "bip39-words.txt";
	std::ifstream file(file_path);;
	if (!file.is_open())
		return false;

	std::string line;
	while (std::getline(file, line)) {
		bip39_words.push_back(line);
	}
	
	return true;
}


std::vector<int> bits_to_indices(const std::vector<bool>& bits) {
	std::vector<int> indices;
	for (size_t i = 0; i < bits.size(); i += 11) {
		int index = 0;
		for (size_t j = 0; j < 11 && (i + j) < bits.size(); ++j) {
			index = (index << 1) | bits[i + j];
		}
		indices.push_back(index);
	}
	return indices;
}

std::string generate_mnemonic(int length)
{
	std::string mnemonic = "";
	std::random_device rd;
	std::vector<uint8_t> entropy(16);

	for (auto& byte : entropy) {
		byte = rd() % 256;
	}

	std::vector<uint8_t> hash(32);
	SHA256(entropy.data(), entropy.size(), hash.data());
	uint8_t checksumBits = hash[0];

	std::vector<bool> bits;
	for (const auto& byte : entropy) {
		for (int i = 7; i >= 0; --i) {
			bits.push_back((byte >> i) & 1);
		}
	}

	for (int i = 7; i >= 4; --i) {
		bits.push_back((checksumBits >> i) & 1);
	}

	std::vector<int> word_indices = bits_to_indices(bits);


	for (int i = 0; i < 12; ++i) {
		int index = word_indices[i];
		mnemonic += bip39_words[index];
		if (i < 11) {
			mnemonic += " ";
		}
	}
	return mnemonic;
}
