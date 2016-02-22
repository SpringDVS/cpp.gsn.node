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

	gtn_root_nodes,

	management_echo,
};
#endif /* PROTOCOL_COMMON_HPP */

