#include "protocol/dvsp_packet.hpp"

dvsp_packet::dvsp_packet()
: m_header(), m_content("")
{ }

dvsp_packet::dvsp_packet(const serial_ptr serial) 
{ deserialise(serial); }

dvsp_packet::dvsp_packet(const dvsp_packet& orig)
: m_header(orig.m_header), m_content(orig.m_content)
{ }

dvsp_packet::~dvsp_packet() {
}


std::string& dvsp_packet::content() noexcept {
	return m_content;
}

const std::string& dvsp_packet::content() const noexcept {
	return m_content;
}

dvsp_header& dvsp_packet::header() noexcept {
	return m_header;
}

const dvsp_header& dvsp_packet::header() const noexcept {
	return m_header;
}

dvsp_packet::serial_ptr dvsp_packet::serialise() const noexcept{
	auto hdr_sz = sizeof(dvsp_header);
	auto hdr_ptr = reinterpret_cast<const serial_type*>(&m_header);
	auto serial = new serial_type[hdr_sz + m_content.length()];
	
	std::copy(hdr_ptr, hdr_ptr+hdr_sz, serial);
	std::copy(m_content.data(), m_content.data() + m_content.length(), serial+hdr_sz);
	return serial;
}

void dvsp_packet::deserialise(const serial_ptr serial) noexcept {
	auto hdr_ptr = reinterpret_cast<const dvsp_header*>(serial);
	std::copy(hdr_ptr, hdr_ptr+1, &m_header);
	m_content.assign(serial+sizeof(dvsp_header), m_header.size);
}