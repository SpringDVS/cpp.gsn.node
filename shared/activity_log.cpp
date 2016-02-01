#include "activity_log.hpp"

activity_log::activity_log()
	: m_num_entries(0)
	, m_hash({0})
{
}

hash512 activity_log::hash_entry(activity& act) const {
	auto raw = act.serialise();
	auto hash = std::move(hash_sha512(raw, act.size()));
	delete raw;
	return hash;
}

hash512 activity_log::hash_partial_entry(activity& act) const {
	return act.header().hash;
}


const hash512& activity_log::hash() const {
	return m_hash;
}

activity_log::serial_ptr activity_log::serialize_log() const {
	auto heap = new activity::serial_type[size()];
	for(auto& a : m_log) {
		auto s = a.serialise();
		std::copy(s, s+a.size(), heap);
	}
	return heap;
}

activity_log::iterator activity_log::begin() {
	return m_log.begin();
}

activity_log::const_iterator activity_log::cbegin() {
	return m_log.cbegin();
}

activity_log::iterator activity_log::end() {
	return m_log.end();
}

activity_log::const_iterator activity_log::cend() {
	return m_log.cend();
}

activity_log::size_type activity_log::size() const {
	auto total = 0;
	for(auto& a : m_log) {
		total += a.size();
	}
	
	return total;
}

void activity_log::hash_log() {
	auto ptr = serialize_log();
	auto hash = hash_sha512(ptr, size());
	delete ptr;
	m_hash = hash;
}
