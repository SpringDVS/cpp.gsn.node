#ifndef ACTIVITY_LOG_HPP
#define ACTIVITY_LOG_HPP

#include <list>
#include "activity.hpp"

class activity_log
{
public:
	typedef std::list<activity> activity_list;
	
	typedef activity_list::iterator iterator;
	typedef activity_list::const_iterator const_iterator;
	typedef activity_list::reverse_iterator reverse_iterator;
	typedef activity_list::value_type value_type;
	
	typedef activity& reference;
	typedef const activity& const_reference;
	
	typedef size_t size_type;
	typedef activity::serial_ptr serial_ptr;

public:
	activity_log();
	
	/** Retrieve cached hash of entire log
	 * 
	 * Whenever there is an alteration to the log
	 * the hash is updated and cached. This method
	 * retrieves that cached hash.
	 * 
	 * @return const hash512 array reference
	 */
	const hash512& hash() const;
	
	/**
	 * Iterator pointer to start of activity_list
	 * @return activity_list::iterator
	 */
	iterator begin();
	
	/**
	 * Iterator pointer to end of activity_list
	 * @return activity_list::iterator
	 */
	iterator end();
	
	/**
	 * Const iterator pointer to beginning of activity_list
	 * @return activity_list::const_iterator
	 */
	const_iterator cbegin();

	/**
	 * Const iterator pointer to end of activity_list
	 * @return activity_list::const_iterator
	 */
	const_iterator cend();
	
	/**
	 * Total size of the log in bytes
	 * 
	 * @return size_type Size in bytes
	 */
	size_type size() const;
	
	/**
	 * Create a serial form of the entire log
	 * 
	 * @return serial_ptr A pointer to a serialised form of log
	 */
	serial_ptr serialize_log() const;
	
	// Database style interface
	void begin_transaction();
	void complete_transaction();

	void write_record(const reference record);
	const_reference read_record(hash512 hash) const;
private:
	int m_num_entries;
	activity_list m_log;
	hash512 m_hash;
	
	hash512 hash_entry(activity& act) const;
	hash512 hash_partial_entry(activity& act) const;
	void hash_log();
	
};

#endif