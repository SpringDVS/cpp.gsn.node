#ifndef LOG_TRANSACTION_HPP
#define LOG_TRANSACTION_HPP
#include "log_utility.hpp"

class log_transaction {
public:
	log_transaction(log_utility& base);
	log_transaction(const log_transaction& orig) = delete;
	log_transaction(log_transaction&& orig);
	
	virtual ~log_transaction();
	
	log_transaction& operator=(log_transaction&& orig);
	
	void write_record(const log_utility::reference record);
	const activity read_record(hash512 hash);
	hash512 read_hash();
	
	const log_header& header(); 

private:
	inline void is_valid() {
		if(!m_valid) throw transaction_invalid_state();
	};
	log_utility& m_base;
	bool m_valid;
};

#endif /* LOG_TRANSACTION_HPP */

