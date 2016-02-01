#include "catch.hpp"
#include "crypto.hpp"
#define SHA512_CONTROL "8ae6ae71a75d3fb2e0225deeb004faf95d816a0a58093eb4cb5a3aa0f197050d7a4dc0a2d5c6fbae5fb5b0d536a0a9e6b686369fa57a027687c3630321547596"

TEST_CASE( "Test SHA-512 hash", "[shared],[crypto]" ) {
	char data[11] = "HelloWorld";
	std::string control = SHA512_CONTROL;
	
	auto hash = hash_sha512(data, 10);
	auto hash_str = sha_string(hash);
	
	REQUIRE(hash.size() == 64);
	REQUIRE(hash_str == control);
}