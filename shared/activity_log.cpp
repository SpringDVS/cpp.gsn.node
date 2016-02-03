#include "activity_log.hpp"

activity_log::activity_log()
	: m_header()
{ 
	
}

log_transaction activity_log::begin_transaction() {
	m_mutex.lock();
	return std::move(log_transaction(*this));
}

void activity_log::end_transaction() {
	m_mutex.unlock();
}

log_utility::header_type& activity_log::header() {
	return m_header;
}

log_utility::log_type& activity_log::log() {
	return m_log;
}


