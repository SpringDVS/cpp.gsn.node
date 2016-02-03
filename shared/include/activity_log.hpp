#ifndef ACTIVITY_LOG_HPP
#define ACTIVITY_LOG_HPP

#include <mutex>
#include "log_transaction.hpp"


class activity_log : log_utility
{
public:
	typedef std::list<activity> activity_list;
	typedef log_utility::header_type header_type;
	typedef log_utility::log_type log_type;
	
public:
	activity_log();
	
	// Database style interface
	log_transaction begin_transaction();

protected:
	header_type& header();
	log_type& log();
	void end_transaction();

private:
	log_header m_header;
	std::mutex m_mutex;
	activity_list m_log;
};

#endif