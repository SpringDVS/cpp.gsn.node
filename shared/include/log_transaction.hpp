#ifndef LOG_TRANSACTION_HPP
#define LOG_TRANSACTION_HPP
#include "log_utility.hpp"
#include "log_segment.hpp"

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
	
	/**
	 * @brief Write a record to the back of the log
	 * 
	 * Write a pre-generated record to the end of the log
	 * and rehash the header of the log.
	 * 
	 * This 
	 * 
	 * @param record a const reference to the record to add
	 * @throws transaction_invalid_state
	 */
	void write_record(const_reference record);
	
	/**
	 * @brief Write another log's segment to the end of of this log
	 * 
	 * This will push all the entries in the segment onto this
	 * log and finally rehash the log
	 * 
	 * @note This needs to rollback
	 * @param segment A reference to a log_segment
	 * @throws transaction_invalid_state
	 */
	void write_segment(log_segment& segment);
	
	/**
	 * @brief Read record with specified hash
	 * @param hash The 512bit hash of the record to read
	 * @return record of the activity
	 * @throws transaction_invalid_state, transaction_read_fail
	 */
	const activity read_record(hash512 hash);
	
	/**
	 * @brief get header of the log
	 * @return const reference to the header
	 */
	const log_header& header() const;
	
	value_type generate_record(value_type::header_type& header);
	value_type generate_record(value_type::header_type& header, std::string content);
	
	/**
	 * @brief Generate a segment from hash
	 * 
	 * Create a log segment that starts the specified hash but does *not*
	 * include the specified hash.
	 * 
	 * If hash does not exist, it will throw
	 * 
	 * @return log_segment object
	 * @throws transaction_invalid_state, transaction_read_fail
	 */
	log_segment segment_from(hash512 hash);
	
	value_type::header_type generate_header();
	
	serial_ptr serialise() const;
	serial_ptr serialise_partial() const;
		
	const_iterator cbegin() const;
	const_iterator begin() const;
	
	const_iterator cend() const;
	const_iterator end() const;
	
	const_reference front() const;
	const_reference back() const;
	
	size_type size() const;
	
	bool log_verification() const;
	
	bool head_verification(const_reference record) const;
	bool verification_from(hash512 hash) const;

	size_type log_size() const;
	size_type log_size_partial() const;
	
	hash512 log_hash() const;

private:
	utility_type& m_base;
	log_type& m_log;
	header_type& m_header;
	
	bool m_valid;
	
	inline void is_valid() const {
		if(!m_valid) throw transaction_invalid_state();
	};
	
	void rewrite_log_hash();


};

#endif /* LOG_TRANSACTION_HPP */

