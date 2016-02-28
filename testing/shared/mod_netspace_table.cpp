/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <iostream>
#include "catch.hpp"
#include "infrastructure/netspace_table.hpp"
TEST_CASE( "Test netspace_table ctors", "[shared],[netspace_table]" ) {
	SECTION("Default") {
		auto t = netspace_table();
		REQUIRE(t.size() == 0);
	}
}

TEST_CASE( "Test insertion" ) {
	auto t = netspace_table();
	
	t.add_node(netspace_node());
	REQUIRE(t.size() == 1);
	
	t.add_node(netspace_node());
	REQUIRE(t.size() == 2);
}

TEST_CASE("Test netspace_table.clean/.empty", "[shared],[netspace_table]") {
	auto t = netspace_table();
	t.add_node(netspace_node());
	
	REQUIRE_FALSE(t.empty());
	t.clear();
	REQUIRE(t.empty());
}

TEST_CASE("Test netspace_table.at", "[shared],[netspace_table]") {
	auto t = netspace_table();
	t.add_node(netspace_node(netnode_type::trusted));
	
	auto n = t.at(0);
	REQUIRE(n.type() == netnode_type::trusted);
	
	REQUIRE_THROWS_AS(t.at(2), std::out_of_range);
}

TEST_CASE("Test netspace_table iterator", "[shared],[netspace_table]") {
	auto t = netspace_table();
	for(auto i = 0; i < 3; i++) {
		auto nt = static_cast<netnode_type>(i);
		t.add_node(netspace_node(nt));
	}
	
	auto j = 0;

	for(auto& n : t) {
		REQUIRE(n.type() == static_cast<netnode_type>(j++));
	}

}

TEST_CASE("Test netspace_table.find_addr", "[shared],[netspace_table]") {
	auto t = netspace_table();
	
	t.add_node(netspace_node(netnode_type::org, 
						"org",
						"192.168.1.1"));

	t.add_node(netspace_node(netnode_type::org, 
						"org",
						"192.168.1.2"));

	SECTION("netspace_addr") {
		auto addr = netspace_addr::from_string("192.168.1.1");
		auto n = t.find_addr(addr);
		REQUIRE_FALSE(n == std::end(t));
		REQUIRE((*n).address().to_string() == "192.168.1.1");
	}

	SECTION("String") {
		REQUIRE_NOTHROW(t.find_addr("192.168.1.2"));
		auto n = t.find_addr("192.168.1.2");
		
		REQUIRE_FALSE(n == std::end(t));
		REQUIRE((*n).address().to_string() == "192.168.1.2");
	}

	SECTION("netspace_addr fail") {
		auto addr = netspace_addr::from_string("192.168.1.3");
		REQUIRE(t.find_addr(addr) == std::end(t));
	}

	SECTION("String fail") {
		REQUIRE(t.find_addr("192.168.1.3") == std::end(t));
	}
}

TEST_CASE("Test netspace_table.find_host", "[shared],[netspace_table]") {
	auto t = netspace_table();
	
	t.add_node(netspace_node(netnode_type::org, 
						"org_a",
						"192.168.1.1"));

	t.add_node(netspace_node(netnode_type::org, 
						"org_b",
						"192.168.1.2"));
	
	SECTION("Successful find") {
		auto n = t.find_host("org_b");
		
		REQUIRE_FALSE(n == std::end(t));
		REQUIRE((*n).hostname() == "org_b");
	}

	SECTION("Failed find") {
		auto n = t.find_host("org_c");
		
		REQUIRE(n == std::end(t));
	}
}

TEST_CASE("Test netspace_table.find_type", "[shared],[netspace_table]") {
	auto t = netspace_table();
	t.add_node(netspace_node(netnode_type::root, 
						"root_a",
						"192.168.1.1"));

	t.add_node(netspace_node(netnode_type::org, 
						"org_b",
						"192.168.1.2"));
	
	t.add_node(netspace_node(netnode_type::org, 
						"org_c",
						"192.168.1.3"));
	
	t.add_node(netspace_node(netnode_type::org, 
						"org_d",
						"192.168.1.4"));

	t.add_node(netspace_node(netnode_type::org, 
						"org_e",
						"192.168.1.5"));
	
	t.add_node(netspace_node(netnode_type::trusted, 
						"trusted_f",
						"192.168.1.6"));

	SECTION("Successful Find First") {
		auto n = t.find_type(netnode_type::root);
		REQUIRE_FALSE(n == std::end(t));
		REQUIRE((*n).type() == netnode_type::root);
		REQUIRE((*n).hostname() == "root_a");
	}
	
	SECTION("Successful Find Loop") {
		auto it = std::begin(t);
		auto total = 0u;
		while(it != std::end(t)) {
			it = t.find_type(netnode_type::org, it);
			if(it == std::end(t)) break;
			
			REQUIRE((*it).type() == netnode_type::org);
			total++;
			it++;
		}
		
		REQUIRE(total == 4);
	}
	
	SECTION("Failed find") {
		auto n = t.find_type(netnode_type::georoot);
		REQUIRE(n == std::end(t));
	}
}