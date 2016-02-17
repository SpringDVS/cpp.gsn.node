#include "activity/activity_log.hpp"

activity_log::activity_log()
	: m_header()
	, m_transacting(false)
{ 
	
}

log_transaction activity_log::begin_transaction() {
	m_mutex.lock();
	m_transacting = true;
	return std::move(log_transaction(*this));
}

void activity_log::end_transaction() {
	m_transacting = false;
	m_mutex.unlock();
}

activity_log::header_type& activity_log::header() {
	return m_header;
}

activity_log::container_type& activity_log::log() {
	return m_log;
}

bool activity_log::transacting() {
	return m_transacting;
}