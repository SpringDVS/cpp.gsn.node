#include "log_transaction.hpp"

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
{
	orig.m_valid = false;	
}
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
}

void log_transaction::write_record(const_reference record) {
	is_valid();
	m_log.push_back(record);
}

const log_header& log_transaction::header() {
	is_valid();
	return m_header;
}

log_transaction::iterator log_transaction::begin() {
	is_valid();
	return m_log.begin();
}

log_transaction::const_iterator log_transaction::cbegin() {
	is_valid();
	return m_log.cbegin();
}

log_transaction::iterator log_transaction::end() {
	is_valid();
	return m_log.end();
}

log_transaction::const_iterator log_transaction::cend() {
	is_valid();
	return m_log.cend();
}

log_transaction::reference log_transaction::back() {
	is_valid();
	return m_log.back();
}

log_transaction::const_reference log_transaction::cback() {
	is_valid();
	return m_base.log().back();
}



