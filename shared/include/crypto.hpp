#ifndef CRYPTO_HPP
#define CRYPTO_HPP

#include <array>
#include <iomanip>

#include "common.hpp"

using hash512 = std::array<unsigned char, 64>;
using hash2048 = std::array<unsigned char, 256>;

hash512 hash_sha512(char* data, size_t size);
hash2048 hash_sha2048(char* data, size_t size);

template<typename HASH>
std::string sha_string(const HASH& hash) {
	std::stringstream ss;
	for(auto& ch : hash) {
		ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ch);
	}
	
	return ss.str();
}
#endif /* CRYPTO_HPP */

