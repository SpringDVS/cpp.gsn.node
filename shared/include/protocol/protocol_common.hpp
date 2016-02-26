#ifndef PROTOCOL_COMMON_HPP
#define PROTOCOL_COMMON_HPP

#include "infrastructure/net_common.hpp"

enum class dvsp_msgtype : char {
	undefined,
	gsn_register_host,
	gsn_unregister_host,
			
	gsn_resolution,
	gsn_local_area,
	gsn_root_nodes,

	gsn_request,
	gsn_response,
	gsn_payload,

	gtn_root_nodes,

	management_echo,
};

enum class dvsp_rcode  : int {
	netspace_error = 101,
	network_error = 102,
	malformed_content = 103,
	ok = 200,
	fake_udp = 505,
};

struct frame_register {
	char type;
	char len;
	std::array<char, 62> hostname;
};

struct frame_response_code {
	dvsp_rcode response;
};

struct  __attribute__((packed)) frame_address {
	dvsp_rcode response;
	netspace_ipv4 addr;
};

struct __attribute__((packed)) frame_network {
	dvsp_rcode response;
	char total;
	std::uint16_t size;
};

inline frame_register construct_frame_register(netnode_type type, std::string hostname) {
	frame_register fr;

	fr.type = static_cast<char>(type);
	fr.len = hostname.length();
	hostname.copy(fr.hostname.data(), hostname.length());
	return fr;
};

inline frame_response_code construct_frame_response_code(dvsp_rcode code) {
	frame_response_code frc;
	frc.response = code;
	return frc;
}

inline frame_address construct_frame_address(netspace_addr addr) {
	frame_address f;
	f.response = dvsp_rcode::ok;
	f.addr = addr.to_v4().to_bytes();
	return f;
}

inline frame_network* construct_frame_network(const std::vector<netspace_ipv4>& nodes) {
	auto sz = sizeof(frame_network) + (nodes.size()*4)+1;
	auto heap = new std::uint8_t[sz];
	auto f = reinterpret_cast<frame_network*>(heap);

	f->response = dvsp_rcode::ok;
	f->total = nodes.size();
	f->size = sz;
	auto ptr = heap + sizeof(frame_network)+1;
	
	for(auto& n : nodes) {
		std::copy(n.data(), n.data()+4, ptr);
		ptr += 4;
	}
	return f;
}
#endif /* PROTOCOL_COMMON_HPP */

