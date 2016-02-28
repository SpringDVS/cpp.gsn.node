/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <iostream>
#include "catch.hpp"
#include "infrastructure/netnode.hpp"
TEST_CASE( "Test netnode ctors", "[shared],[netnode]" ) {
	SECTION("default") {
		auto node = netspace_node();
		REQUIRE(node.type() == netnode_type::undefined);
		REQUIRE(node.address() == netspace_addr());
		REQUIRE(node.hostname() == std::string());
		REQUIRE(node.protocol() == service_protocol::dvsp);
	}
	
	SECTION("Typed") {
		auto node = netspace_node(netnode_type::root);
		REQUIRE(node.type() == netnode_type::root);
		REQUIRE(node.address() == netspace_addr());
		REQUIRE(node.hostname() == std::string());
		REQUIRE(node.protocol() == service_protocol::dvsp);
	}
	
	SECTION("Type, String, String") {
		auto node = netspace_node(netnode_type::root, 
							"root_node", 
							"192.168.1.1");
		
		
		REQUIRE(node.type() == netnode_type::root);
		REQUIRE(node.address() == netspace_addr::from_string("192.168.1.1"));
		REQUIRE(node.hostname() == std::string("root_node"));
		REQUIRE(node.protocol() == service_protocol::dvsp);
	}
	
	SECTION("Type, String, netspace_addr") {
		auto node = netspace_node(netnode_type::root, 
							"root_node", 
							netspace_addr::from_string("192.168.1.1"));
		
		
		REQUIRE(node.type() == netnode_type::root);
		REQUIRE(node.address() == netspace_addr::from_string("192.168.1.1"));
		REQUIRE(node.hostname() == std::string("root_node"));
		REQUIRE(node.protocol() == service_protocol::dvsp);
	}
	
	SECTION("Type, String, netspace_addr, service_protocol") {
		auto node = netspace_node(netnode_type::root, 
							"root_node", 
							netspace_addr::from_string("192.168.1.1"),
							service_protocol::http);
		
		
		REQUIRE(node.type() == netnode_type::root);
		REQUIRE(node.address() == netspace_addr::from_string("192.168.1.1"));
		REQUIRE(node.hostname() == std::string("root_node"));
		REQUIRE(node.protocol() == service_protocol::http);
	}
}