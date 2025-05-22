/* 
* This file is part of the Trezor Crypto library.
* 
* Copyright (c) 2021 SatoshiLabs s.r.o. (https://www.satoshilabs.com)
* 
* This code is licensed under the MIT License (MIT), see LICENSE file for details.
*/


#pragma once

#include <stdio.h>
#include <string>
#include <vector>

extern "C" {
#include "trezor/bip39.h"
#include "trezor/bip32.h"
#include "trezor/sha2.h"
#include "trezor/ripemd160.h"
#include "trezor/segwit_addr.h"
#include "trezor/sha2.h"
#include "trezor/curves.h"
#include "trezor/base58.h"
#include "trezor/sha3.h"
#include "trezor/secp256k1.h"

}

#include "address_checker.h"

wallet generate_evm_wallet(std::string mnemonic);
wallet generate_bitcoin_wallet(const std::string mnemonic);
