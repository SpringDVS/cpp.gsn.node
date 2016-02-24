#ifndef PROTOCOL_HANDLER_HPP
#define PROTOCOL_HANDLER_HPP
#include "infrastructure/netspace_table.hpp"
#include "infrastructure/netspace_url.hpp"
#include "protocol/dvsp_packet.hpp"
#include "infrastructure/metaspace_gsn.hpp"

class protocol_handler {
public:
	protocol_handler(netspace_table& table, metaspace_gsn& msgsn);
	protocol_handler(const protocol_handler& orig);
	virtual ~protocol_handler();
	
	packet_uptr process_packet(const dvsp_packet& packet, const netspace_addr& addr);

private:

	netspace_table& m_nstable;
	metaspace_gsn& m_msgsn;
	
	packet_uptr register_host(const dvsp_packet& packet, const netspace_addr& addr);
	packet_uptr unregister_host(const dvsp_packet& packet, const netspace_addr& addr);
	
	packet_uptr response(const netspace_addr& addr, int code);
};

#endif /* PROTOCOL_HANDLER_HPP */

