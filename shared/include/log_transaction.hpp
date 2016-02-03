#ifndef LOG_TRANSACTION_HPP
#define LOG_TRANSACTION_HPP
#include "log_utility.hpp"

class log_transaction {
public:
	typedef log_utility utility_type;
	typedef utility_type::container_type log_type;
	typedef utility_type::header_type header_type;
	typedef utility_type::value_type value_type;
	
	typedef value_type& reference;
	typedef const reference const_reference;

	typedef log_type::iterator iterator;
	typedef log_type::const_iterator const_iterator;
	
	
public:
	log_transaction(utility_type& base);
	log_transaction(const log_transaction& orig) = delete;
	log_transaction(log_transaction&& orig);
	
	virtual ~log_transaction();
	
	log_transaction& operator=(log_transaction&& orig);
	
	void write_record(const_reference record);
	const activity read_record(hash512 hash);
	
	const log_header& header();
	
	iterator begin();
	const_iterator cbegin();
	
	iterator end();
	const_iterator cend();
	
	reference front();
	const_reference cfront();
	
	reference back();
	const_reference cback();

private:

	inline void is_valid() {
		if(!m_valid) throw transaction_invalid_state();
	};

	utility_type& m_base;
	log_type& m_log;
	header_type& m_header;
	
	bool m_valid;
};

#endif /* LOG_TRANSACTION_HPP */

