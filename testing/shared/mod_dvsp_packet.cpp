#include "catch.hpp"
#include "protocol/dvsp_packet.hpp"


TEST_CASE( "Test dvsp_packet Ctor", "[shared],[dvsp_packet]" ) {
	SECTION("Default") {
		dvsp_packet p;
		
		REQUIRE( p.header().type == dvsp_msgtype::undefined );
		REQUIRE( p.header().size == 0 );
		REQUIRE( p.header().addr_orig == netspace_ipv4{0} );
		REQUIRE( p.header().addr_dest == netspace_ipv4{0} );
	}
}

TEST_CASE("Modification of dvsp_packet", "[shared],[dvsp_packet]") {
	dvsp_packet p;
	
	p.header().addr_dest = netspace_ipv4{192,168,0,1};
	p.header().addr_orig = netspace_ipv4{192,168,0,2};
	p.header().size = 3;
	p.header().type = dvsp_msgtype::gsn_request;
	p.str_content("Foo");
	
	auto h = p.header();
	
	REQUIRE( h.addr_dest == netspace_ipv4({192,168,0,1}) );
	REQUIRE( h.addr_orig == netspace_ipv4({192,168,0,2}) );
	REQUIRE( h.type == dvsp_msgtype::gsn_request );
	REQUIRE( p.to_string() == "Foo" );
	
	/* //const correctness
	const dvsp_packet cp;
	cp.header().type = dvsp_msgtype::gsn_request;
	*/
}

struct bin_data {
	std::uint64_t ui64;
	long long ul;
	std::uint8_t ui8;
};

TEST_CASE("Add binary content to dvsp_packet", "[shared],[dvsp_packet]") {

	
	bin_data bd;
	bd.ui64 = 342237;
	bd.ul = 123.32;
	bd.ui8 = 67;
	
	dvsp_packet p;
	p.copy_content(&bd, sizeof(bin_data));
	REQUIRE(p.header().size > 0);
	auto bdc = reinterpret_cast<const bin_data&>(p.content());
	REQUIRE(bdc.ui64 == bd.ui64);
	REQUIRE(bdc.ul == bd.ul);
	REQUIRE(bdc.ui8 == bd.ui8);
}

TEST_CASE("Empty Content", "[shared],[dvsp_packet]") {
	
	dvsp_packet p;
	SECTION("Content accessors") {
		REQUIRE_THROWS_AS(p.content(), dvsp_empty_content);
		REQUIRE_THROWS_AS(p.to_string(), dvsp_empty_content);
	}
	
	SECTION("Serialise with invalid header size") {
		p.header().size = 1024;
		REQUIRE_THROWS_AS(p.serialise(), dvsp_empty_content);
		REQUIRE_THROWS_AS(p.content(), dvsp_empty_content);
		REQUIRE_THROWS_AS(p.to_string(), dvsp_empty_content);
	}
}
TEST_CASE("De/Serialise dvsp_packet", "[shared],[dvsp_packet]") {
	dvsp_packet p;
	p.header().addr_dest = netspace_ipv4{192,168,0,1};
	p.header().addr_orig = netspace_ipv4{192,168,0,2};
	p.header().type = dvsp_msgtype::gsn_request;
	p.str_content("Foo");
	
	auto serial = p.serialise();
	
	dvsp_packet d(serial);
	auto h = d.header();
	
	REQUIRE( h.addr_dest == netspace_ipv4({192,168,0,1}) );
	REQUIRE( h.addr_orig == netspace_ipv4({192,168,0,2}) );
	REQUIRE( h.size == 3 );
	REQUIRE( h.type == dvsp_msgtype::gsn_request );
	REQUIRE( p.to_string() == "Foo" );
}