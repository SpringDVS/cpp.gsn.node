#include "log_transaction.hpp"

log_transaction::log_transaction(log_utility& base)
	: m_base(base)
	, m_valid(true)
{ }

log_transaction::log_transaction(log_transaction&& orig)
	: m_base(orig.m_base)
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
	m_valid = true;
	return *this;
}

const activity log_transaction::read_record(hash512 hash) {
	is_valid();	
	auto log = m_base.log();
}

void log_transaction::write_record(const log_utility::reference record) {
	is_valid();
	auto log = m_base.log();
	
	
}

hash512 log_transaction::read_hash() {
	is_valid();
	
}

const log_header& log_transaction::header() {
	return m_base.header();
}

