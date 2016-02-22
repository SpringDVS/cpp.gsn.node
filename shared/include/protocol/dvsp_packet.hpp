#ifndef DVSP_PACKET_HPP
#define DVSP_PACKET_HPP
#include "protocol_common.hpp"

struct dvsp_header {
	dvsp_msgtype type;
	std::uint32_t size;
	netspace_ipv4 addr_orig;
	netspace_ipv4 addr_dest;
	
	dvsp_header()
		: type(dvsp_msgtype::undefined)
		, size(0)
		, addr_orig{0}
		, addr_dest{0}
		{ }
};

class dvsp_packet
{
public:
	typedef char serial_type;
	typedef serial_type* serial_ptr;
	
public:
	dvsp_packet();
	dvsp_packet(const serial_ptr serial);
	dvsp_packet(const dvsp_packet& orig);
	virtual ~dvsp_packet();
	
	dvsp_header& header() noexcept;
	const dvsp_header& header() const noexcept;
	
	std::string& content() noexcept;
	const std::string& content() const noexcept;
	
	serial_ptr serialise() const noexcept;

private:
	dvsp_header m_header;
	std::string m_content;
	
	void deserialise(const serial_ptr serial) noexcept;

};

#endif /* DVSP_PACKET_HPP */

