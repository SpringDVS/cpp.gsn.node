#include <openssl/sha.h>
#include "crypto.hpp"

hash512 hash_sha512(char* data, size_t size) {
	hash512 hash;
	SHA512_CTX c;

	SHA512_Init(&c);
	SHA512_Update(&c, data, size);
	SHA512_Final(hash.data(), &c);
	return hash;
}