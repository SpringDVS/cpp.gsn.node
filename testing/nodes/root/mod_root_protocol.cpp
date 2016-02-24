#include "catch.hpp"
#include "root/protocol_handler.cpp"

dvsp_packet packet_of(dvsp_msgtype type, std::string content = "") {
	dvsp_packet p;
	p.header().addr_dest = netspace_ipv4{192,168,1,1};
	p.header().addr_orig = netspace_ipv4{192,168,1,2};
	p.header().type = type;
	p.str_content(content);
	return p;
}


TEST_CASE("Test protocol_handler invalid msgtype","[node],[root],[protocol]") {
	netspace_table nstable;
	metaspace_gsn msgsn;
	protocol_handler proto(nstable, msgsn);
	auto addr = netspace_addr::from_string("192.168.1.2");
	auto in = packet_of(dvsp_msgtype::undefined);
	char* ow = reinterpret_cast<char*>(&(in.header().type));
	*ow = 101;

	auto out = proto.process_packet(in, addr);
	REQUIRE(out->header().type == dvsp_msgtype::gsn_response);
	REQUIRE(out->content_as<frame_response_code>().response == 101);
}

TEST_CASE("Test protocol_handler register (dynamic tables)", "[node],[root],[protocol]") {
	netspace_table nstable;
	metaspace_gsn msgsn;
	protocol_handler proto(nstable, msgsn);
	auto addr = netspace_addr::from_string("192.168.1.2");
		
	SECTION("Successful registration") {
		auto fr = construct_frame_register(netnode_type::org, "dvs.test");
		auto in = packet_of(dvsp_msgtype::gsn_register_host);
		in.copy_content(&fr, sizeof(fr));
		
		auto out = proto.process_packet(in, addr);
		REQUIRE(out->header().type == dvsp_msgtype::gsn_response);
		REQUIRE(out->content_as<frame_response_code>().response == 200);
		REQUIRE(nstable.size() > 0);
		
		auto it = nstable.find_addr("192.168.1.2");
		REQUIRE(it != nstable.end());
		REQUIRE((*it).hostname() == "dvs.test");
		REQUIRE((*it).type() == netnode_type::org);
		
	}
	
	SECTION("Register existing IP") {
		auto fr = construct_frame_register(netnode_type::org, "dvs.test");
		auto in = packet_of(dvsp_msgtype::gsn_register_host);
		in.copy_content(&fr, sizeof(fr));
		
		proto.process_packet(in, addr);
		auto out = proto.process_packet(in, addr);
		REQUIRE(out->header().type == dvsp_msgtype::gsn_response);
		REQUIRE(out->content_as<frame_response_code>().response == 101);
	}
	
	SECTION("Malformed node type") {
		auto fr = construct_frame_register(netnode_type::org, "dvs.test");
		auto p = static_cast<char*>(&fr.type);
		*p = 100;
		auto in = packet_of(dvsp_msgtype::gsn_register_host);
		in.copy_content(&fr, sizeof(fr));
		auto out = proto.process_packet(in, addr);
		REQUIRE(out->header().type == dvsp_msgtype::gsn_response);
		REQUIRE(out->content_as<frame_response_code>().response == 103);
	}
	
	SECTION("Bad network hop") {
		auto fr = construct_frame_register(netnode_type::org, "dvs.test");
		auto in = packet_of(dvsp_msgtype::gsn_register_host);
		in.copy_content(&fr, sizeof(fr));
		
		auto addr_bad = netspace_addr::from_string("192.168.1.3");
		auto out = proto.process_packet(in, addr_bad);
		REQUIRE(out->header().type == dvsp_msgtype::gsn_response);
		REQUIRE(out->content_as<frame_response_code>().response == 102);
	}
}
