#include "addres_generator.h"

BIP32Node derive_master_key(const std::vector<uint8_t>& seed)
{
    unsigned int len = 64;
    std::vector<unsigned char> I(len);

    HMAC(EVP_sha512(),
        "Bitcoin seed", strlen("Bitcoin seed"),
        seed.data(), seed.size(),
        I.data(), &len);

    BIP32Node node;
    node.privkey = std::vector<unsigned char>(I.begin(), I.begin() + 32);
    node.chain_code = std::vector<unsigned char>(I.begin() + 32, I.end());

    node.pubkey = private_to_public_key(node.privkey);
    return node;
}

BIP32Node derive_key(const BIP32Node& parent_node, const std::string& path)
{
    std::vector<unsigned char> privkey = parent_node.privkey;
    std::vector<unsigned char> chain_code = parent_node.chain_code;

    size_t start = 0;
    while (start < path.size()) {
        size_t end = path.find('/', start);
        if (end == std::string::npos) {
            end = path.size();
        }

        std::string part = path.substr(start, end - start);
        start = end + 1;

        bool is_hardened = part.back() == '\'';
        int index = std::stoi(part.substr(0, part.size() - (is_hardened ? 1 : 0)));
        if (is_hardened) {
            index += 0x80000000;
        }

        std::vector<unsigned char> index_bytes = {
            static_cast<unsigned char>(index >> 24),
            static_cast<unsigned char>(index >> 16),
            static_cast<unsigned char>(index >> 8),
            static_cast<unsigned char>(index)
        };

        std::vector<unsigned char> data = { 0x00 };
        data.insert(data.end(), privkey.begin(), privkey.end());
        data.insert(data.end(), index_bytes.begin(), index_bytes.end());

        std::vector<unsigned char> I = hmac_sha512(chain_code, data);

        privkey = std::vector<unsigned char>(I.begin(), I.begin() + 32);
        chain_code = std::vector<unsigned char>(I.begin() + 32, I.end());
    }

    BIP32Node child;
    child.privkey = privkey;
    return child;
}


std::vector<unsigned char> private_to_public_key(const std::vector<unsigned char>& privkey) {
    EC_KEY* ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (ec_key == nullptr) {
        return {};
    }

    BIGNUM* priv_bn = BN_bin2bn(privkey.data(), privkey.size(), nullptr);
    if (!EC_KEY_set_private_key(ec_key, priv_bn)) {
        return {};
    }

    const EC_GROUP* group = EC_KEY_get0_group(ec_key);
    EC_POINT* pub_point = EC_POINT_new(group);
    if (!EC_POINT_mul(group, pub_point, priv_bn, nullptr, nullptr, nullptr)) {
        return {};
    }

    unsigned char pubkey[33];
    size_t pubkey_len = 0;
    if (!EC_POINT_point2oct(group, pub_point, point_conversion_form_t::POINT_CONVERSION_COMPRESSED, pubkey, sizeof(pubkey), nullptr)) {
        return {};
    }
    
    pubkey_len = 33;
    std::vector<unsigned char> pubkey_vec(pubkey, pubkey + pubkey_len);

    EC_POINT_free(pub_point);
    BN_free(priv_bn);
    EC_KEY_free(ec_key);

    return pubkey_vec;
}


wallet generate_evm_wallet(const std::string& mnemonic) {
    std::vector<unsigned char> seed = mnemonic_to_seed(mnemonic);

    BIP32Node master_key = derive_master_key(seed);
    std::string path = "44'/60'/0'/0/0";
    BIP32Node child_key = derive_key(master_key, path);

    std::string private_key = bytearray2hex(child_key.privkey);
    std::string public_key = keccak256(private_key.substr(2));
    std::string address = public_key.substr(public_key.size() - 40);
    address = "0x" + address;

    wallet wal;
    wal.address = address;
    wal.public_key = public_key;
    wal.private_key = private_key;
    wal.mnemonic = mnemonic;
    return wal;
}

wallet generate_bitcoin_wallet(const std::string& mnemonic) {
  
    std::vector<unsigned char> seed = mnemonic_to_seed(mnemonic);
    BIP32Node master_key = derive_master_key(seed);
    std::string path = "84'/0'/0'/0/0";
    BIP32Node child_key = derive_key(master_key, path);

    std::vector<unsigned char> pubKey = private_to_public_key(child_key.privkey);
    std::vector<unsigned char> sha256Hash = sha256(pubKey);
    std::vector<unsigned char> ripemd160Hash = ripemd160(sha256Hash);
    std::string address = bech32_encode(ripemd160Hash);

    wallet wal;
    wal.address = address;
    wal.public_key = bytearray2hex(pubKey);
    wal.private_key = bytearray2hex(child_key.privkey);
    wal.mnemonic = mnemonic;
    return wal;
}
