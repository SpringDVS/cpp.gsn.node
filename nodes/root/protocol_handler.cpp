#include "protocol_handler.hpp"

protocol_handler::protocol_handler(netspace_table& table, metaspace_gsn& msgsn)
	: m_nstable(table)
	, m_msgsn(msgsn)
{ }

protocol_handler::protocol_handler(const protocol_handler& orig)
	: m_nstable(orig.m_nstable)
	, m_msgsn(orig.m_msgsn)
{ }

protocol_handler::~protocol_handler() {
}

packet_uptr protocol_handler::process_packet(const dvsp_packet& packet, const netspace_addr& addr) {
	
	switch(packet.header().type) {
		case dvsp_msgtype::gsn_register_host:
			return register_host(packet, addr);
		case dvsp_msgtype::gsn_unregister_host:
			return unregister_host(packet, addr);
		default:
			return response(addr, 101);
	}
}

packet_uptr protocol_handler::register_host(const dvsp_packet& packet, const netspace_addr& addr) {

	// GSN management should not hop
	if(addr.to_v4().to_bytes() != packet.header().addr_orig)
		return response(addr, 102); // Network error
	
	if(m_nstable.find_addr(addr.to_string()) != m_nstable.end())
		return response(addr, 101); // Table error

	auto st = reinterpret_cast<const frame_register&>(packet.content());
	
	if(st.type >= static_cast<char>(netnode_type::_final))
		return response(addr, 103); // Malformed content
	
	std::string hostname;
	hostname.assign(st.hostname.data(), st.len);
	
	netnode n(static_cast<netnode_type>(st.type), hostname, addr.to_string());
	
	m_nstable.add_node(n);
	return response(addr, 200);
}

packet_uptr protocol_handler::unregister_host(const dvsp_packet& packet, const netspace_addr& addr) {
	// GSN management should not hop
	if(addr.to_v4().to_bytes() != packet.header().addr_orig)
		return response(addr, 102); // Network error

	auto it = m_nstable.find_addr(addr.to_string());
	if(it == m_nstable.end())
		return response(addr, 101); // Table error
	
	m_nstable.erase_node(it);
	return response(addr, 200);
}

packet_uptr protocol_handler::response(const netspace_addr& addr, int code) {
	packet_uptr p(new dvsp_packet);
	
	p->header().type = dvsp_msgtype::gsn_response;
	auto frc = construct_frame_response_code(code);
	p->copy_content(&frc, sizeof(frame_response_code));
	return p;
}
