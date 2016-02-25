/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <iostream>
#include "catch.hpp"
#include "infrastructure/netspace_url.hpp"
TEST_CASE( "Test netspaced_url ctors", "[shared],[netspace_url]" ) {

	SECTION("Fail") {
		REQUIRE_THROWS_AS(netspace_url("srp://org.nn"), netspace_malformed_url);
		REQUIRE_THROWS_AS(netspace_url(""), netspace_malformed_url);
	}

	SECTION("gsn") {
		netspace_url url;
		REQUIRE_NOTHROW(url = netspace_url("spring://org.nn"));
		auto gsn = url.static_route();
		REQUIRE(gsn.size() == 2);
		REQUIRE(gsn[0] == "org");
		REQUIRE(gsn[1] == "nn");
		REQUIRE(url.top_network() == "nn");
	}
	
	SECTION("gsn:glq") {
		netspace_url url;
		REQUIRE_NOTHROW(url = netspace_url("spring://org.nn:gsquery"));
		auto gsn = url.static_route();
		REQUIRE(gsn.size() == 2);
		REQUIRE(gsn[0] == "org");
		REQUIRE(gsn[1] == "nn");
		REQUIRE(url.geosub_query() == "gsquery");
		REQUIRE(url.top_network() == "nn");
	}

	SECTION("gsn/res") {
		netspace_url url;
		REQUIRE_NOTHROW(url = netspace_url("spring://org.gsn.nn/resource"));
		auto gsn = url.static_route();
		REQUIRE(gsn.size() == 3);
		REQUIRE(gsn[0] == "org");
		REQUIRE(gsn[1] == "gsn");
		REQUIRE(gsn[2] == "nn");
		REQUIRE(url.resource() == "resource");
		REQUIRE(url.top_network() == "nn");
	}

	SECTION("gsn?query") {
		netspace_url url;
		REQUIRE_NOTHROW(url = netspace_url("spring://org.gsn.nn?query"));
		auto gsn = url.static_route();
		REQUIRE(gsn.size() == 3);
		REQUIRE(gsn[0] == "org");
		REQUIRE(gsn[1] == "gsn");
		REQUIRE(gsn[2] == "nn");
		REQUIRE(url.query() == "query");
		REQUIRE(url.top_network() == "nn");
	}

	SECTION("gsn:glq?query") {
		netspace_url url;
		REQUIRE_NOTHROW(url = netspace_url("spring://org.gsn.nn:glq?query"));
		auto gsn = url.static_route();
		REQUIRE(gsn.size() == 3);
		REQUIRE(gsn[0] == "org");
		REQUIRE(gsn[1] == "gsn");
		REQUIRE(gsn[2] == "nn");
		REQUIRE(url.geosub_query() == "glq");
		REQUIRE(url.query() == "query");
		REQUIRE(url.top_network() == "nn");
	}

	SECTION("gsn:glq/res") {
		netspace_url url;
		REQUIRE_NOTHROW(url = netspace_url("spring://org.gsn.nn:glq/resource"));
		auto gsn = url.static_route();
		REQUIRE(gsn.size() == 3);
		REQUIRE(gsn[0] == "org");
		REQUIRE(gsn[1] == "gsn");
		REQUIRE(gsn[2] == "nn");
		REQUIRE(url.geosub_query() == "glq");
		REQUIRE(url.resource() == "resource");
		REQUIRE(url.top_network() == "nn");
	}
	
	SECTION("gsn:glq/res?query") {
		netspace_url url;
		REQUIRE_NOTHROW(url = netspace_url("spring://org.gsn.nn:glq/resource?query"));
		auto gsn = url.static_route();
		REQUIRE(gsn.size() == 3);
		REQUIRE(gsn[0] == "org");
		REQUIRE(gsn[1] == "gsn");
		REQUIRE(gsn[2] == "nn");
		REQUIRE(url.geosub_query() == "glq");
		REQUIRE(url.query() == "query");
		REQUIRE(url.resource() == "resource");
		REQUIRE(url.top_network() == "nn");
	}
}

TEST_CASE("Test netspace_url reconstruction", "[shared],[netspace_url]") {
	netspace_url url("spring://org.gsn.nn:glq/resource?query");
	
	SECTION("Carbon-copy reconstruction") {
		REQUIRE(url.to_string() == "spring://org.gsn.nn:glq/resource?query");
	}
	
	SECTION("Static route change") {
		url.static_route().pop_back();
		REQUIRE(url.to_string() == "spring://org.gsn:glq/resource?query");
		url.static_route().pop_back();
		REQUIRE(url.to_string() == "spring://org:glq/resource?query");
		url.static_route().push_back("esusx");
		REQUIRE(url.to_string() == "spring://org.esusx:glq/resource?query");
	}
	
	SECTION("Geosub query change") {
		url.geosub_query() = "";
		REQUIRE(url.to_string() == "spring://org.gsn.nn/resource?query");
		url.geosub_query() = "foobar";
		REQUIRE(url.to_string() == "spring://org.gsn.nn:foobar/resource?query");
	}

	SECTION("Resource change") {
		url.resource() = "";
		REQUIRE(url.to_string() == "spring://org.gsn.nn:glq?query");
		url.resource() = "barfoo";
		REQUIRE(url.to_string() == "spring://org.gsn.nn:glq/barfoo?query");
	}
	
	SECTION("Query change") {
		url.query() = "";
		REQUIRE(url.to_string() == "spring://org.gsn.nn:glq/resource");
		url.query() = "farboo";
		REQUIRE(url.to_string() == "spring://org.gsn.nn:glq/resource?farboo");
	}
}