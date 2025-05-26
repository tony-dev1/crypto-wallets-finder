#include "address_generator.h"
#include <sstream>
#include <iomanip>
#include <string.h>

std::vector<std::string> load_wordlist(const std::string& filename);

std::vector<std::string> split_words(const std::string& str, char delim = ' ');

std::string join(const std::vector<std::string>& vec, const std::string& delim = " ");


wallet brute_force_seed_phrase(std::vector<std::string>& pattern,
                 const std::vector<std::string>& wordlist,
                 const std::vector<int>& missing_indices,
                 const std::string& target_address,
                 size_t depth = 0,
                 size_t& count = *(new size_t(0)));


wallet brute_find_wallet(const std::string& mnemonic, const std::string& target_address);