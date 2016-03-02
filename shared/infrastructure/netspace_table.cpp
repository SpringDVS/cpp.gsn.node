#include "infrastructure/netspace_table.hpp"

netspace_table::netspace_table() {
}

netspace_table::netspace_table(const netspace_table& orig) 
	: m_table(orig.m_table)
{ }

netspace_table::~netspace_table() {
}

void netspace_table::add_node(const netspace_node& node) noexcept {
	m_table.push_back(node);
}

void netspace_table::erase_node(const_iterator it) noexcept {
	if(it == std::end(m_table)) return;
	m_table.erase(it);
}

netspace_table::iterator netspace_table::find_addr(const netspace_addr& addr) noexcept {
	return std::find_if(std::begin(m_table), std::end(m_table), [&addr](netspace_node& v) {
		return v.address() == addr;
	});
}

netspace_table::iterator netspace_table::find_addr(const std::string& addr) noexcept {
	return find_addr(netspace_addr::from_string(addr));
}

netspace_table::iterator netspace_table::find_host(const std::string& host) noexcept {
	return std::find_if(std::begin(m_table), std::end(m_table), [&host](netspace_node& v) {
		return v.hostname() == host;
	});
}

netspace_table::iterator netspace_table::find_type(netnode_type nt) {
	return std::find_if(std::begin(m_table), std::end(m_table), [nt](netspace_node& v) {
		return v.type() == nt;
	});
}

netspace_table::iterator netspace_table::find_type(netnode_type nt, iterator start) {
	return std::find_if(start, std::end(m_table), [nt](netspace_node& v) {
		return v.type() == nt;
	});
}

netspace_node& netspace_table::at(size_type index) {
	if(index >= m_table.size()) throw std::out_of_range("Index out of range");
	
	return m_table.at(index);
}

netspace_table::size_type netspace_table::size() const {
	return m_table.size();
}

bool netspace_table::empty() const {
	return m_table.empty();
}


void netspace_table::clear() noexcept {
	m_table.clear();
}

netspace_table::iterator netspace_table::begin() {
	return m_table.begin();
}

netspace_table::iterator netspace_table::end() {
	return m_table.end();
}

netspace_table::iterator netspace_table::find_suid(const std::string& suid) noexcept {
	return std::find_if(std::begin(m_table), std::end(m_table), [&suid](netspace_node& v) {
		return v.suid() == suid;
	});
}
