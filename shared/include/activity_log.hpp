#ifndef ACTIVITY_LOG_HPP
#define ACTIVITY_LOG_HPP

#include <mutex>
#include "log_transaction.hpp"


class activity_log : log_utility
{
public:
	typedef log_utility::value_type value_type;
	typedef log_utility::container_type container_type;
	typedef log_utility::header_type header_type;
	
public:
	activity_log();
	
	// Database style interface
	log_transaction begin_transaction();
	bool transacting();

protected:
	
	// Accessible to transaction object
	header_type& header() override;
	container_type& log() override;
	void end_transaction() override;

private:
	log_header m_header;
	std::mutex m_mutex;
	container_type m_log;
	bool m_transacting;
};

#endif