#include "activity/log_transaction.hpp"

log_transaction::log_transaction(utility_type& base)
	: m_base(base)
	, m_log(base.log())
	, m_header(base.header())
	, m_valid(true)
{ }

log_transaction::log_transaction(log_transaction&& orig)
	: m_base(orig.m_base)
	, m_log(orig.m_log)
	, m_header(orig.m_header)
	, m_valid(true)
{ orig.m_valid = false; }

log_transaction::~log_transaction() {
	if(!m_valid) return;
	
	m_base.end_transaction();
}

log_transaction& log_transaction::operator=(log_transaction&& orig) {
	orig.m_valid = false;
	
	m_base = orig.m_base;
	m_log = orig.m_log;
	m_header = orig.m_header;
	
	m_valid = true;
	return *this;
}

const activity log_transaction::read_record(hash512 hash) {
	is_valid();
	for(const auto& act : m_log) {
		if(act.header().hash == hash)
			return act;
	}
	
	throw transaction_read_fail("Record does not exist");
}

void log_transaction::write_record(const_reference record) {
	is_valid();
	m_log.push_back(record);
	m_header.hash = log_hash();
	m_header.num_records++;
}

void log_transaction::write_segment(log_segment& segment) {
	is_valid();
	for(auto& r : segment) {
		m_log.push_back(r);
		m_header.num_records++;
	}
	
	rewrite_log_hash();
}

const log_header& log_transaction::header() const {
	is_valid();
	return m_header;
}

log_transaction::const_iterator log_transaction::cbegin() const {
	is_valid();
	return m_log.cbegin();
}

log_transaction::const_iterator log_transaction::begin() const {
	is_valid();
	return m_log.cbegin();
}

log_transaction::const_iterator log_transaction::cend() const {
	is_valid();
	return m_log.cend();
}

log_transaction::const_iterator log_transaction::end() const {
	is_valid();
	return m_log.cend();
}

log_transaction::const_reference log_transaction::back() const {
	is_valid();
	return m_log.back();
}

log_transaction::const_reference log_transaction::front() const {
	is_valid();
	return m_log.front();
}

log_transaction::value_type::header_type log_transaction::generate_header() {
	is_valid();
	return value_type::header_type();
}

log_transaction::value_type log_transaction::generate_record(value_type::header_type& header) {
	is_valid();
	if(!size()) return value_type(header);
	
	return value_type(header, back().header().hash);
}

log_transaction::value_type log_transaction::generate_record(value_type::header_type& header, std::string content) {
	is_valid();
	if(!size()) return value_type(header, content);
	return value_type(header, content, back().header().hash);
}

log_transaction::size_type log_transaction::size() const {
	is_valid();
	return m_log.size();
}

log_transaction::size_type log_transaction::log_size() const {
	is_valid();
	size_type sz = sizeof(log_header);
	for(auto& r : m_log) {
		sz += r.size();
	}
	return sz;
}

log_transaction::size_type log_transaction::log_size_partial() const {
	is_valid();
	size_type sz = 0;
	for(auto& r : m_log) {
		sz += r.size();
	}
	return sz;
}

bool log_transaction::log_verification() const {
	is_valid();
	auto hash = log_hash();
	
	return hash == m_header.hash;
}

bool log_transaction::head_verification(const_reference record) const {
	is_valid();
	
	auto check = value_type(record.header(), record.content(), back().header().hash);
	return check.header().hash == record.header().hash;
}

log_transaction::serial_ptr log_transaction::serialise() const {
	is_valid();
	auto hdr_ptr = reinterpret_cast<serial_ptr>(&m_header);
	auto hdr_sz = sizeof(log_header);
	
	auto heap = new serial_type[log_size()];
	auto ptr = heap;
	
	std::copy(hdr_ptr, hdr_ptr + hdr_sz, ptr);
	ptr += hdr_sz;
	
	for(auto& r : m_log) {
		auto s = r.serialise();
		std::copy(s, s + r.size(), ptr);
		ptr += r.size();
		delete s;
	}

	return heap;
}

log_transaction::serial_ptr log_transaction::serialise_partial() const {
	is_valid();
	auto heap = new serial_type[log_size_partial()];
	auto ptr = heap;
	for(auto& r : m_log) {
		auto s = r.serialise();
		std::copy(s, s + r.size(), ptr);
		ptr += r.size();
		delete s;
	}

	return heap;
}

hash512 log_transaction::log_hash() const {
	is_valid();
	auto serial = serialise_partial();
	auto hash = hash_sha512(serial, log_size_partial());
	delete serial;
	return hash;
}

log_segment log_transaction::segment_from(hash512 hash) {
	is_valid();
	auto inc = false;
	log_type c;
	
	for(const auto& r : m_log) {
		if(inc) c.push_back(r);
		if(r.header().hash == hash) inc = true;
	}
	
	if(!inc) throw transaction_read_fail("Record does not exist");
	
	return log_segment(m_header.hash, std::move(c));
}

void log_transaction::rewrite_log_hash() {
	m_header.hash = log_hash();
}

