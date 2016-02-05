#ifndef LOG_TRANSACTION_HPP
#define LOG_TRANSACTION_HPP
#include "log_utility.hpp"

class log_transaction {
public:
	typedef log_utility utility_type;
	typedef utility_type::container_type log_type;
	typedef utility_type::header_type header_type;
	typedef utility_type::value_type value_type;
	
	typedef utility_type::size_type size_type;
	typedef utility_type::serial_type serial_type;
	typedef utility_type::serial_ptr serial_ptr;
	
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
	
	const log_header& header() const;
	
	value_type generate_record(value_type::header_type& header);
	value_type generate_record(value_type::header_type& header, std::string content);
	
	value_type::header_type generate_header();
	
	serial_ptr serialise() const;
	serial_ptr serialise_partial() const;
		
	const_iterator cbegin() const;
	const_iterator cend() const;
	
	const_reference cfront() const;
	const_reference cback() const;
	
	size_type size() const;
	
	bool log_verification() const;
	
	bool head_verification(const_reference record) const;
	bool verification_from(hash512 hash) const;

	size_type log_size() const;
	size_type log_size_partial() const;
	
	hash512 log_hash() const;

private:

	inline void is_valid() const {
		if(!m_valid) throw transaction_invalid_state();
	};

	utility_type& m_base;
	log_type& m_log;
	header_type& m_header;
	
	bool m_valid;
};

#endif /* LOG_TRANSACTION_HPP */

