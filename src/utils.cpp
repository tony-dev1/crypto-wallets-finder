#include "utils.h"


std::string base58_encode(const std::vector<uint8_t>& input) {
    const std::string base58_chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    std::string result = "";

    int carry;
    for (size_t i = 0; i < input.size(); ++i) {
        carry = input[i];
        for (size_t j = result.size(); j > 0; --j) {
            carry += 256 * result[j - 1];
            result[j - 1] = carry % 58;
            carry /= 58;
        }
        while (carry) {
            result.insert(result.begin(), carry % 58);
            carry /= 58;
        }
    }

    std::string encoded = "";
    for (size_t i = 0; i < result.size(); ++i) {
        encoded += base58_chars[result[i]];
    }

    return encoded;
}



std::string keccak256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA256_Final(hash, &sha256);

    std::ostringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::setw(2) << std::setfill('0') << std::hex << (int)hash[i];
    }
    return ss.str();
}

std::vector<unsigned char> ripemd160(const std::vector<unsigned char>& data) {
    unsigned char hash[RIPEMD160_DIGEST_LENGTH];
    RIPEMD160_CTX ripemd160Context;
    RIPEMD160_Init(&ripemd160Context);
    RIPEMD160_Update(&ripemd160Context, data.data(), data.size());
    RIPEMD160_Final(hash, &ripemd160Context);
    return std::vector<unsigned char>(hash, hash + RIPEMD160_DIGEST_LENGTH);
}

std::vector<unsigned char> sha256(const std::vector<unsigned char>& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256Context;
    SHA256_Init(&sha256Context);
    SHA256_Update(&sha256Context, data.data(), data.size());
    SHA256_Final(hash, &sha256Context);
    return std::vector<unsigned char>(hash, hash + SHA256_DIGEST_LENGTH);
}

std::vector<unsigned char> hmac_sha512(const std::vector<unsigned char>& key, const std::vector<unsigned char>& data) {
    unsigned int len = SHA512_DIGEST_LENGTH;
    std::vector<unsigned char> result(len);
    HMAC_CTX* ctx = HMAC_CTX_new();
    HMAC_Init_ex(ctx, key.data(), key.size(), EVP_sha512(), nullptr);
    HMAC_Update(ctx, data.data(), data.size());
    HMAC_Final(ctx, result.data(), &len);
    HMAC_CTX_free(ctx);
    return result;
}

std::vector<int> create_bech32_checksum(const std::vector<int>& data)
{
    int values[32] = { 0 };

    for (int i = 0; i < 32; i++) {
        values[i] = 0;
    }
    for (size_t i = 0; i < data.size(); ++i) {
        int d = data[i];
        int v = values[0] >> 25;
        for (int j = 0; j < 32 - 1; j++) {
            values[j] = (values[j + 1] ^ (v & 0x3f));
        }
        values[32 - 1] = d;
    }

    std::vector<int> checksum(6);
    for (size_t i = 0; i < 6; i++) {
        checksum[i] = values[i] & 31;
    }
    return checksum;
}

std::vector<int> convertBits(const std::vector<uint8_t>& input, size_t fromBits, size_t toBits) {
    std::vector<int> output;
    uint32_t accumulator = 0;
    size_t bits = 0;

    for (size_t i = 0; i < input.size(); ++i) {
        uint8_t value = input[i];
        accumulator = (accumulator << fromBits) | value;
        bits += fromBits;

        while (bits >= toBits) {
            bits -= toBits;
            output.push_back((accumulator >> bits) & ((1 << toBits) - 1));
        }
    }

    return output;
}

std::string bech32_encode(const std::vector<unsigned char>& data) {
    const std::string prefix = "bc";
    static const char* BECH32_ALPHABET = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";
    std::vector<int> encoded;
    int accumulator = 0;
    size_t bits = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        unsigned char value = data[i];
        accumulator = (accumulator << 8) | value;
        bits += 8;
        while (bits >= 5) {
            bits -= 5;
            encoded.push_back((accumulator >> bits) & 31);
        }
    }

    std::vector<int> checksum = create_bech32_checksum(encoded);
    std::string result = prefix + "1" + BECH32_ALPHABET[0];
    for (int v : encoded) {
        result.push_back(BECH32_ALPHABET[v]);
    }
    for (int v : checksum) {
        result.push_back(BECH32_ALPHABET[v]);
    }

    return result;
}


std::vector<uint8_t> mnemonic_to_seed(const std::string& mnemonic, const std::string& passphrase) {
    std::string salt = "mnemonic" + passphrase;
    std::vector<uint8_t> seed(64);

    PKCS5_PBKDF2_HMAC(
        mnemonic.c_str(), mnemonic.size(),
        reinterpret_cast<const unsigned char*>(salt.c_str()), salt.size(),
        2048, EVP_sha512(),
        seed.size(), seed.data()
    );

    return seed;

}

std::string bytearray2hex(const std::vector<uint8_t>& byteArray) {
    std::ostringstream oss;
    for (size_t i = 0; i < byteArray.size(); ++i) {
        oss << std::setw(2) << std::setfill('0') << std::hex << (int)byteArray[i];
    }
    return oss.str();
}
