/* 
* This file is part of the Trezor Crypto library.
* 
* Copyright (c) 2021 SatoshiLabs s.r.o. (https://www.satoshilabs.com)
* 
* Modified by TonyDev BTC for use in WalletGen.
* 
* This code is licensed under the MIT License (MIT), see LICENSE file for details.
*/



#include <iostream>
#include <Windows.h>

#include "address_checker.h"
#include "address_generator.h"
#include <limits>
#include <iomanip>


extern "C" {
    const char *mnemonic_generate(int strength);
}

std::string save_found_path = "found_wallets.txt";

std::string walletgen_path;

void set_color(int color = 7) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void print_logo() {
    std::cout << "***************************************************************************\n";
    std::cout << "*__        __    _ _      _    ____                                       *\n";
    std::cout << "*\\ \\      / /_ _| | | ___| |_ / ___| ___ _ __                             *\n";
    std::cout << "* \\ \\ /\\ / / _` | | |/ _ \\ __| |  _ / _ \\ '_ \\                            *\n";
    std::cout << "*  \\ V  V / (_| | | |  __/ |_| |_| |  __/ | | |                           *\n";
    std::cout << "*   \\_/\\_/ \\__,_|_|_|\\___|\\__|\\____|\\___|_| |_|             ____  _       *\n";
    std::cout << "*                  | |__  _   _  |_   _|__  _ __  _   _    | __ )| |_ ___ *\n";
    std::cout << "*                  | '_ \\| | | |   | |/ _ \\| '_ \\| | | |   |  _ \\| __/ __|*\n";
    std::cout << "*                  | |_) | |_| |   | | (_) | | | | |_| |   | |_) | || (__ *\n";
    std::cout << "*                  |_.__/ \\__, |   |_|\\___/|_| |_|\\__, |___|____/ \\__\\___|*\n";
    std::cout << "*                         |___/                   |___/_____|             *\n";
    std::cout << "***************************************************************************\n";
    std::cout << "Github: https://github.com/tony-dev1/walletgen\n";
    std::cout << "\n";

 

}

void print_donate_info() {
    std::cout << "---------------------------------------------------------------------------------\n";
    std::cout << "| my crypto wallets for donate:                                                 |\n";
    std::cout << "| Bitcoin:                       bc1qm2hla5zg735uqpgjtp3va4djqhh9zd8derzrwx     |\n";
    std::cout << "| EVM (ETH, BNB, MATIC, e.t.c):  0x8BCa04cA3e65fa7ECE2314e02149F6Ef4F3A0060     |\n";
    std::cout << "| Litecoin:                      ltc1qa4ermet8czres4aunf33zwe35sk6peq6tg6cc8    |\n";
    std::cout << "| USDT TRC20:                    TFu3ydp1tSDhCtrHNsVSVxriVGhHvVzUu9             |\n";
    std::cout << "---------------------------------------------------------------------------------\n";
}

int show_menu() {
    set_color(9);
 point1:
    std::cout << "\033[34m============================================================================\n";
    std::cout << "\033[38;5;214m[1]\033[94m - generate one BTC wallet                                              \033[34m=\n";
    std::cout << "\033[38;5;214m[2]\033[94m - generate one EVM wallet (ETH, BNB, MATIC e.t.c)                      \033[34m=\n";
    std::cout << "\033[38;5;214m[3]\033[94m - search BTC wallets with balance (using the Internet - slower)        \033[34m=\n";
    std::cout << "\033[38;5;214m[4]\033[94m - search BTC wallets with balance (using the database - faster)        \033[34m=\n";
    std::cout << "\033[38;5;214m[5]\033[94m - search EVM wallets with balance (using the Internet - slower)        \033[34m=\n";
    std::cout << "\033[38;5;214m[6]\033[94m - search EVM wallets with balance (using the database - faster)        \033[34m=\n";
    std::cout << "\033[38;5;214m[7]\033[94m - support the developer                                                \033[34m=\n";
    std::cout << "\033[38;5;214m[8]\033[94m - exit                                                                 \033[34m=\n";
    std::cout << "\033[34m============================================================================\n";
    set_color(7);

    std::cout << "select an action using the key: ";

    int action;
    while(!(std::cin >> action)){
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.clear();
    }
    return action;
}

void print_wallet(wallet wal, int type) {
    set_color(8);

    std::cout << "-----------------------------------------------------------------------------\n";
    if(type == WAL_TYPE_BTC){
        std::cout << "[Bitcoin Wallet]\n";
        std::cout << "BECH32 address:   " << wal.address << "\n";
        std::cout << "P2SH address:     " << wal.address_p2sh << "\n";
        std::cout << "P2PKH address:    " << wal.address_legacy << "\n";
        std::cout << "public key:       " << wal.public_key << "\n";
        std::cout << "private key:      " << wal.private_key << "\n";
        std::cout << "mnemonic: " << wal.mnemonic << "\n";
    }
    else if(type == WAL_TYPE_EVM){
        std::cout << "[EVM/Ethereum Wallet]\n";
        std::cout << "address:     " << wal.address << "\n";
        std::cout << "public key:  " << wal.public_key << "\n";
        std::cout << "private key: " << wal.private_key << "\n";
        std::cout << "mnemonic: " << wal.mnemonic << "\n";
    }

    std::cout << "-----------------------------------------------------------------------------\n\n";
    set_color(7);
}

void save_wallet_data(wallet wal, wallet_data wal_data, int type) {
    std::string save_text = "";
    if (type == WAL_TYPE_BTC) {
        save_text = "[Bitoin]\naddress: " + wal.address + "\npublic key: " + wal.public_key + "\nprivate key: " + wal.private_key + "\nmnemonic: " + wal.mnemonic + 
            "\nbtc_amount: " + std::to_string(wal_data.btc_amount);
    }
    else {
        save_text = "[EVM]\naddress: " + wal.address + "\npublic key: " + wal.public_key + "\nprivate key: " + wal.private_key + "\nmnemonic: " + wal.mnemonic +
            "\neth_amount: " + std::to_string(wal_data.eth_amount) + "\nbnb_amount: " + std::to_string(wal_data.bnb_amount) + "\matic_amount: " + std::to_string(wal_data.matic_amount);
    }

    HANDLE hfile = CreateFileA(save_found_path.c_str(), FILE_ALL_ACCESS, NULL, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!hfile) {
        std::cout << save_text << "\n";
        return;
    }

    if (!WriteFile(hfile, save_text.c_str(), save_text.size(), nullptr, nullptr)) {
        std::cout << save_text;
    }

    CloseHandle(hfile);

}

void start_search_btc_wallets(int way = 0);
void start_search_evm_wallets(int way = 0);



int main(int argc, char** argv) {
     
    walletgen_path = std::string(argv[0]);
    int n = walletgen_path.find_last_of("\\");
    if (n != std::string::npos)
        walletgen_path = walletgen_path.substr(0, n);

    if (argc > 1) {
        if (strstr(argv[1], "gen-btc")) {
            int count = 1;
            if (argc > 2)
                count = std::atoi(argv[2]);
            for (int i = 0; i < count; ++i) {
                std::string mnemonic = std::string(mnemonic_generate(12));
                wallet wal = generate_bitcoin_wallet(mnemonic);
                print_wallet(wal, 0);
            }
        }
        else if (strstr(argv[1], "gen-evm") || strstr(argv[1], "gen-eth")) {
            int count = 1;
            if (argc > 2)
                count = std::atoi(argv[2]);
            for (int i = 0; i < count; ++i) {
                std::string mnemonic = std::string(mnemonic_generate(12));
                wallet wal = generate_evm_wallet(mnemonic);
                print_wallet(wal, 0);
            }

        }
        else if (strstr(argv[1], "search-btc")) {
            start_search_btc_wallets();
        }
        else if (strstr(argv[1], "search-evm") || strstr(argv[1], "search-eth")) {
            start_search_evm_wallets();
        }
        else {
            std::cout << "invalid parameters\n";
        }
        return 0;
    }

    std::string title = "[WalletGen] - Menu";
    SetConsoleTitleA(title.c_str());
    set_color(6);
    print_logo();
    set_color(7);

    while (true) {

        int action = show_menu();

        generate_wallet_test();

        std::cout << "\n";

        if (action == 1) {
            std::string mnemonic = std::string(mnemonic_generate(128));
            wallet wal = generate_bitcoin_wallet(mnemonic);
            print_wallet(wal, 0);
        }


        else if (action == 2) {
            std::string mnemonic = std::string(mnemonic_generate(128));
            wallet wal = generate_evm_wallet(mnemonic);
            print_wallet(wal, 1);
        }
        else if (action == 3) {
            start_search_btc_wallets(0);
        }
        else if (action == 4) {
            start_search_btc_wallets(1);
        }
        else if (action == 5) {
             start_search_evm_wallets(0);
        }
        else if (action == 6) {
            start_search_evm_wallets(1);
        }
        else if (action == 7) {
            print_donate_info();
        }
        else if (action == 8) { 
            break;
        }
        else {
            std::cout << "invalid action\n";
        }

        std::cout << "\n\n";

    }

    return 0;
}

void start_search_btc_wallets(int way)
{
    wallet_checker checker;
    if (!checker.initialize(way)) {
        MessageBoxA(GetConsoleWindow(), "Failed to initialize checker. check your internet connection", "Message", MB_ICONERROR);
        return;
    }

    if (way == 0) {
        if (coingecko_api::get_btc_udst_price() == 0.0f) {
            MessageBoxA(GetConsoleWindow(), "Failed to connect to the Coingecko API", "Message", MB_ICONWARNING);
            return;
        }
    }
    else {
        if (!wallets_data_base::btc_database_exists()) {
            set_color(4);
            std::cout << "[Error]\nFailed to load BTC wallets data base from '" << BTC_WAL_DATABASE_NAME << "'.\nDownload this file to https://github.com/tony-dev-btc/walletgen/releases/tag/database\nAnd move to " << walletgen_path << "\n";
            set_color(7);
            return;
        }

        std::cout << "Loading the BTC database from " BTC_WAL_DATABASE_NAME << "...\n";
        wallets_data_base::load_btc_database();
    }

    unsigned long long checked_mnemonics = 0;
    float found_balance = 0.0f;
    int speed = 0, speed_check = 0, mnemonics_by_sec = 0;
    std::cout << std::fixed << std::setprecision(4);
 
    while (true) {

        std::string mnemonic = std::string(mnemonic_generate(128));
        wallet wal = generate_bitcoin_wallet(mnemonic);
        wallet_data wal_data = checker.get_wallet_data(wal, WAL_TYPE_BTC);
        std::cout << "\033[38;5;214mmnemonic:\033[0m  " << wal.mnemonic << "\n\033[38;5;214maddresses:\033[0m " << wal.address << " | " << wal.address_legacy << " | " << wal.address_p2sh
        << "\n\033[38;5;214mbalance:\033[0m   " << wal_data.btc_amount << " btc\n";
        std::cout << "\033[94m===============================================================================================================================\033[0m\n";

        if (wal_data.btc_amount > 0) {
            save_wallet_data(wal, wal_data, WAL_TYPE_BTC);
            found_balance += wal_data.btc_amount * coingecko_api::get_btc_udst_price();

            std::string new_title = "[WalletGen | " + std::string(TYPE) + "] - Found BTC wallet / [Checked mnemonics: " + std::to_string(checked_mnemonics) + " | Found balance: " + std::to_string(found_balance) +
                "$ | Speed: " + std::to_string(speed) + "/s]";
            SetConsoleTitleA(new_title.c_str());
            print_wallet(wal, WAL_TYPE_BTC);
            system("pause");
        }
        if (time(0) - speed_check > 1) {
            speed = mnemonics_by_sec;
            mnemonics_by_sec = 0;
            speed_check = time(0);
        }


        if(checked_mnemonics % 100 == 0){
            std::string new_title = "[WalletGen | " + std::string(TYPE) + "] - Seaching for BTC wallets / [Checked mnemonics: " + std::to_string(checked_mnemonics) + " | Found balance: " + std::to_string(found_balance) + 
                "$ | Speed: " + std::to_string(speed) + "/s]";
            SetConsoleTitleA(new_title.c_str());
            }

            checked_mnemonics++;
            mnemonics_by_sec++;

        }

    }
   

void start_search_evm_wallets(int way)
{
    wallet_checker checker;
    if (!checker.initialize(way)) {
        MessageBoxA(GetConsoleWindow(), "Failed to initialize checker. check your internet connection", "Message", MB_ICONERROR);
        return;
    }
    if (way == 0) {
        if (coingecko_api::get_btc_udst_price() == 0.0f) {
            MessageBoxA(GetConsoleWindow(), "Failed to connect to the Coingecko API", "Message", MB_ICONWARNING);
            return;
        }
    }
    else {
        if (!wallets_data_base::evm_database_exists()) {
            set_color(4);
            std::cout << "[Error]\nFailed to load EVM wallets data base from '" << EVM_WAL_DATABASE_NAME << "'.\nDownload this file to https://github.com/tony-dev1/walletgen/releases/tag/database\n And move to " << walletgen_path << "\n";
            set_color(7);
            return;
        }

        std::cout << "Loading the ETH database from " EVM_WAL_DATABASE_NAME << "...\n";
        wallets_data_base::load_evm_database();
    }

    unsigned long long checked_mnemonics = 0;
    float found_balance = 0.0f;
    int speed = 0, speed_check = 0, mnemonics_by_sec = 0;

    while (true) {

        std::string mnemonic = std::string(mnemonic_generate(128));
        wallet wal = generate_evm_wallet(mnemonic);
        wallet_data wal_data = checker.get_wallet_data(wal, WAL_TYPE_EVM);
        wal = generate_evm_wallet(mnemonic, false);
            wal_data = checker.get_wallet_data(wal, WAL_TYPE_EVM);
        std::cout << "\033[38;5;214mmnemonic:\033[0m " << wal.mnemonic << "\n\033[38;5;214maddress:\033[0m  " << wal.address << "\n\033[38;5;214mbalance:\033[0m  ";

        if (wal_data.eth_amount == 0 && wal_data.bnb_amount == 0 && wal_data.matic_amount == 0) {
         std::cout << "(empty)\n\033[94m==========================================================================================\033[0m\n";
        }
        else {
            std::cout << "(with balance)\n";
            save_wallet_data(wal, wal_data, WAL_TYPE_BTC);
            found_balance += (wal_data.eth_amount * coingecko_api::get_eth_usdt_price()) + (wal_data.bnb_amount * coingecko_api::get_bnb_usdt_price()) +
                (wal_data.matic_amount * coingecko_api::get_matic_usdt_price());

            std::string new_title = "[WalletGen] - Found EVM wallet / [Checked mnemonics: " + std::to_string(checked_mnemonics) + " | Found balance: " + std::to_string(found_balance) +
                "$ | Speed: " + std::to_string(speed) + "/s]";
            SetConsoleTitleA(new_title.c_str());
            print_wallet(wal, WAL_TYPE_EVM);
            system("pause");
        }

        if (time(0) - speed_check > 1) {
            speed = mnemonics_by_sec;
            mnemonics_by_sec = 0;
            speed_check = time(0);
        }


        if(checked_mnemonics % 100 == 0){
        std::string new_title = "[WalletGen | " + std::string(TYPE) + "] - Seaching for EVM wallets / [Checked mnemonics: " + std::to_string(checked_mnemonics) + " | Found balance: " + std::to_string(found_balance) + 
            "$ | Speed: " + std::to_string(speed) + "/s]";
        SetConsoleTitleA(new_title.c_str());

        }
        checked_mnemonics++;
        mnemonics_by_sec++;
    }

}
