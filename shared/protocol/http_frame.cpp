#include <sstream>
#include "protocol/http_frame.hpp"

http_frame::http_frame(dvsp_packet frame, std::string hostname)
	: m_dvsp_frame(frame), m_hostname(hostname), m_address(frame.header().addr_dest)
{ 
	generate_header();
}

http_frame::http_frame(const http_frame& orig)
	: m_dvsp_frame(orig.m_dvsp_frame), m_hostname(orig.m_hostname), m_header(orig.m_header), m_address(orig.m_address)
{ }

http_frame::~http_frame() {
}

const netspace_ipv4& http_frame::address() const {
	return m_address;
}

const std::string& http_frame::hostname() const {
	return m_hostname;
}

std::unique_ptr<char[]> http_frame::serialise() const {
	auto serial = new char[size()];
	auto ptr = serial;
	
	m_header.copy(ptr, m_header.length());
	ptr += m_header.length();
	auto frame_serial = m_dvsp_frame.serialise();
	std::copy(frame_serial, frame_serial+m_dvsp_frame.size(), ptr);
	
	return std::unique_ptr<char[]>(serial);
}

std::string http_frame::header() const {
	return m_header;
}

http_frame::size_type http_frame::size() const {
	return m_header.length() + m_dvsp_frame.size();
}


void http_frame::generate_header() {
	std::stringstream ss;
	
	ss	<< "POST /spring/spring_hook.php HTTP/1.1\r\n"
		<< "Host: " << m_hostname << "\r\n"
		<< "User-Agent: SpringDVS\r\n"
		<< "Content-Type: application/octet-stream\r\n"
		<< "Content-Length: " << std::to_string(m_dvsp_frame.size()) << "\r\n\r\n";
	
	m_header = ss.str();
}




