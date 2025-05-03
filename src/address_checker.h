#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>

#include <Windows.h>
#include <Wininet.h>
#include <WinSock2.h>


#define CHECKER_ERROR_FAILED 1
#define CHECKER_ERROR_BUSY 2
#define CHECKER_ERROR_BANNED 3
#define CHECKER_ERROR_UNKNOWN 4

#define WAL_TYPE_BTC 0
#define WAL_TYPE_EVM 1


#define BTC_WAL_DATABASE_NAME "btc_database.txt"
#define EVM_WAL_DATABASE_NAME "evm_database.txt"

struct wallet_data {
    std::string address;
    int type;
    float btc_amount;  
    float eth_amount;
    float bnb_amount;
    float matic_amount;
};


static class wallets_data_base {
public:
    static bool btc_database_exists();
    static bool load_btc_database();
    static std::string get_btc_balance(std::string address);

    static bool evm_database_exists();
    static bool load_evm_database();
    static std::string get_evm_balance(std::string address);

private:

    static std::unordered_map<std::string, std::string> btc_wallets_databases[2];
    static std::unordered_map<std::string, std::string> eth_wallets_databases[2];

};


class wallet_checker {
public:
    bool initialize(int way = 0);

    wallet_data get_wallet_data(std::string address, int type);

private:
    bool check_connection();
    int way;
};


namespace coingecko_api {
    float get_btc_udst_price();
    float get_eth_usdt_price();
    float get_bnb_usdt_price();
    float get_matic_usdt_price();
}
