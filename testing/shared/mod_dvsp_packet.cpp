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