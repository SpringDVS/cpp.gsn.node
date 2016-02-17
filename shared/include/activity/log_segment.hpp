#ifndef LOG_SEGMENT_HPP
#define LOG_SEGMENT_HPP

#include "activity/log_utility.hpp"


class log_segment {
public:
	typedef log_utility utility_type;
	typedef utility_type::container_type container_type;
	
	typedef utility_type::value_type value_type;
	typedef value_type::header_type header_type;
	
	typedef utility_type::size_type size_type;
	typedef value_type& reference;
	typedef const reference const_reference;

	
	typedef utility_type::serial_type serial_type;
	typedef utility_type::serial_ptr serial_ptr;
	
	typedef container_type::iterator iterator;
	typedef container_type::const_iterator const_iterator;
	
public:
	log_segment(hash512 hash, container_type&& segment);
	log_segment(const log_segment& orig);
	log_segment(const serial_ptr serial);
	virtual ~log_segment();
        
	serial_ptr serialise();	
	size_type log_size();
	
	size_type size();
	const log_header& header();
	
	const_iterator cbegin();
	const_iterator begin();
	
	const_iterator cend();
	const_iterator end();
	

private:
    container_type m_segment;
	log_header m_header;

	void deserialise(const serial_ptr serial);
};

#endif /* LOG_SEGMENT_HPP */

