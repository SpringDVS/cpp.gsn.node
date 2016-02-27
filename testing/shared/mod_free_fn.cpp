#include "catch.hpp"

#include "infrastructure/net_common.hpp"
TEST_CASE("ipv4_to_netspace_addr", "[free]") {
	netspace_ipv4 ipv4{{192,168,1,1}};
	auto addr = ipv4_to_netspace_addr(ipv4);
	
	REQUIRE(addr.to_string() == "192.168.1.1");
}

TEST_CASE("ipv4_to_string", "[free]") {
	netspace_ipv4 ipv4{{192,168,1,1}};
	auto addr = ipv4_to_string(ipv4);
	
	REQUIRE(addr == "192.168.1.1");
}
