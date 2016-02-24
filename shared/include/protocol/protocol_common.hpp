#ifndef PROTOCOL_COMMON_HPP
#define PROTOCOL_COMMON_HPP

#include "infrastructure/net_common.hpp"

enum class dvsp_msgtype : char {
	undefined,
	gsn_register_host,
	gsn_unregister_host,
	gsn_local_area,
	gsn_root_nodes,

	gsn_request,
	gsn_response,
	gsn_payload,

	gtn_root_nodes,

	management_echo,
};

struct frame_register {
	char type;
	char len;
	std::array<char, 62> hostname;
};

struct frame_response_code {
	int response;
};

inline frame_register construct_frame_register(netnode_type type, std::string hostname) {
	frame_register fr;

	fr.type = static_cast<char>(type);
	fr.len = hostname.length();
	hostname.copy(fr.hostname.data(), hostname.length());
	return fr;
};

inline frame_response_code construct_frame_response_code(int code) {
	frame_response_code frc;
	frc.response = code;
	return frc;
}
#endif /* PROTOCOL_COMMON_HPP */

