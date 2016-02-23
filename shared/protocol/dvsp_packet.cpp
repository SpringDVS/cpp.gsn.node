#include "protocol/dvsp_packet.hpp"

dvsp_packet::dvsp_packet()
: m_header(), m_content(nullptr)
{ }

dvsp_packet::dvsp_packet(const serial_ptr serial) 
{ deserialise(serial); }

dvsp_packet::dvsp_packet(const dvsp_packet& orig)
: m_header(orig.m_header)
{ 
	m_content = new generic_type[orig.m_header.size];
	std::copy(orig.m_content, orig.m_content+m_header.size, m_content);
}

dvsp_packet::~dvsp_packet() {
	if(m_content) delete[] m_content;
}

void dvsp_packet::clear() {
	m_header = dvsp_header();
	reset(0);
}

void dvsp_packet::str_content(std::string data) noexcept {
	reset(data.length());
	
	std::copy(data.data(), data.data() + data.length(), m_content);
}

const dvsp_packet::generic_type& dvsp_packet::content() const {
	if(!m_header.size
	|| (m_header.size && !m_content ) )
		throw dvsp_empty_content();

	return *m_content;
}

std::string dvsp_packet::to_string() const {
	if(!m_header.size
	|| (m_header.size && !m_content ) )
		throw dvsp_empty_content();
	std::string s;
	s.assign(reinterpret_cast<const char*>(m_content), m_header.size);
	return s;
}


dvsp_header& dvsp_packet::header() noexcept {
	return m_header;
}

const dvsp_header& dvsp_packet::header() const noexcept {
	return m_header;
}

dvsp_packet::serial_ptr dvsp_packet::serialise() const{
	if(m_header.size && !m_content) throw dvsp_empty_content();
	auto hdr_sz = sizeof(dvsp_header);
	auto hdr_ptr = reinterpret_cast<const serial_type*>(&m_header);
	auto serial = new serial_type[hdr_sz + m_header.size];
	
	std::copy(hdr_ptr, hdr_ptr+hdr_sz, serial);
	std::copy(m_content, m_content + m_header.size, serial+hdr_sz);
	return serial;
}

void dvsp_packet::deserialise(const serial_ptr serial) noexcept {
	auto hdr_ptr = reinterpret_cast<const dvsp_header*>(serial);
	std::copy(hdr_ptr, hdr_ptr+1, &m_header);
	reset(m_header.size);
	
	if(!m_header.size) return;
	
	auto data_ptr = serial+sizeof(dvsp_header);
	std::copy(data_ptr, data_ptr+m_header.size, m_content);
}