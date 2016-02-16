#include "activity.hpp"

activity::activity()
	: m_header(), m_msg_content("")
{ }

activity::activity(header_type hdr)
	: m_header(hdr), m_msg_content("")
{ 
	partial_hash();
}

activity::activity(header_type hdr, content_type msg)
	: m_header(hdr), m_msg_content(msg)
{ 
	m_header.msg_size = m_msg_content.length();
	partial_hash();
}

activity::activity(header_type hdr, content_type msg, const hash512& salt)
	: m_header(hdr), m_msg_content(msg)
{
	m_header.msg_size = m_msg_content.length();
	rehash(salt);
}

activity::activity(header_type hdr, const hash512& salt)
	: m_header(hdr), m_msg_content("")
{
	m_header.msg_size = m_msg_content.length();
	rehash(salt);
}

const activity::content_type& activity::content() const {
	return m_msg_content;
}

const activity::header_type& activity::header() const {
	return m_header;
}

activity::size_type activity::size() const {
	return sizeof(activity_header) + m_msg_content.length();
}

activity::size_type activity::size_partial() const {
	return size() - sizeof(header_type::hash);
}

activity::serial_ptr activity::serialise() const {
	auto serial = new serial_type[size()];
	auto ptr = const_cast<header_type*>(&m_header);
	auto hdrptr = reinterpret_cast<const serial_ptr>(ptr);
	std::copy(hdrptr, hdrptr+sizeof(m_header), serial);
	m_msg_content.copy(serial+sizeof(header_type), m_msg_content.length());

	return serial;
};

activity::serial_ptr activity::serialise_partial() const {
	auto serial = new serial_type[size_partial()];
	
	auto size_hash = sizeof(m_header.hash);
	auto size_hdr = sizeof(header_type) - size_hash;
	auto const_ptr = const_cast<header_type*>(&m_header);
	
	serial_ptr ptr = reinterpret_cast<serial_type*>(const_ptr) + size_hash;
	std::copy(ptr, ptr+size_hdr, serial);
	m_msg_content.copy(serial+size_hdr, m_msg_content.length());
	return serial;		
}

void activity::deserialise(const serial_ptr serial) {
	auto hdr = reinterpret_cast<header_type*>(serial);
	std::copy(hdr, hdr+1, &m_header);
	m_msg_content.assign(serial + sizeof(header_type), m_header.msg_size);
}

void activity::partial_hash() {
	auto ptr = serialise_partial();
	m_header.hash = hash_sha512(ptr, size_partial());
	delete ptr;
}

void activity::rehash(const hash512& salt) {
	m_header.hash = salt;
	auto ptr = serialise();
	m_header.hash = hash_sha512(ptr, size());
}


