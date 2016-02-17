#ifndef ACTIVITY_COMMON_HPP
#define ACTIVITY_COMMON_HPP
#include <chrono>

#include "common.hpp"
#include "crypto.hpp"

using unix_timestamp = int64_t;
using activity_clock = std::chrono::system_clock;

enum class activity_type 
{
	undefined,
	root_verify, org_verify, trusted_verify,
	training, 
};

struct activity_header  
{
	activity_header()
	: hash({0}), timestamp(0)
	, type(activity_type::undefined), msg_size(0)
	, signature({0})
	{ };

	hash512			hash;
	unix_timestamp	timestamp;
	activity_type	type;
	std::size_t		msg_size;
	hash2048		signature;
	
};

class activity
{
public:
	typedef char			serial_type;
	typedef serial_type*	serial_ptr;
	typedef serial_type&	serial_ref;
	typedef size_t			size_type;
	typedef std::string		content_type;
	typedef activity_header header_type;
	
	/**
	 * @brief Construct a default, undefined activity
	 */
	activity();
	
	/**
	 * @brief Construct an activity with supplied header, no content
	 * @param hdr activity_header A header object
	 */
	activity(header_type hdr);
	
	/**
	 * @brief Construct an activity with supplied header and content
	 * 
	 * @param hdr activity_header A header object
	 * @param msg content_type The content message
	 */
	activity(header_type hdr, content_type msg);
	
	/**
	 * @brief Construct an activity linked into a log chain
	 * 
	 * This will generate a digest that includes a supplied
	 * hash -- this will allow it to be hashed against a 
	 * previous entry or  -- if first entry -- a salt.
	 * 
	 * @param hdr
	 * @param msg
	 * @param salt
	 */
	activity(header_type hdr, content_type msg, const hash512& salt);
	
		/**
	 * @brief Construct an activity linked into a log chain
	 * 
	 * This will generate a record against a record salt
	 * 
	 * @param hdr
	 * @param salt
	 */
	activity(header_type hdr, const hash512& salt);
	
	/**
	 * @brief Get a const reference of the activity header
	 * 
	 * @return const header_type&
	 */
	const header_type& header() const;
	
	/**
	 * @brief Get a const refernce to the content of the activity
	 * 
	 * @return const content_type&
	 */
	const content_type& content() const;
	

	/**
	 * @brief	Return the full size of header and contents
	 * 
	 * Calculate the size of the whole header and contents
	 * of the message. It will return the size in bytes.
	 * 
	 * @return	size_type The size in bytes
	 */
	size_type size() const;
	
	/**
	 * @brief	Return the partial size
	 * 
	 * Calculate the size of the header and contents
	 * of the message, subtracting the size of the hash
	 * from the final sum. This is useful for sizing
	 * specifically for hashing the contents
	 * 
	 * @return size_type The size in bytes
	 */
	size_type size_partial() const;

	/**
	 * @brief	Serialise the full contents
	 * 
	 * Convert the entire contents of the activity
	 * into a serial of bytes. This can be stored
	 * or transferred.
	 * 
	 * @return serial_ptr A pointer to the serialised contents
	 */
	serial_ptr serialise() const;

	/**
	 * @brief	Serialise the partial contents
	 * 
	 * Convert the contents of the activity and the
	 * header, excluding the hash, into a serial of
	 * bytes. This can be used for hashing the contents.
	 * 
	 * @return serial_ptr A pointer to the serialised contents
	 */
	serial_ptr serialise_partial() const;
	
	/**
	 * @brief Convert a serialised form into an activity
	 * 
	 * The object will take a pointer to the serialised
	 * activity and de-serialise it into it's contents.
	 * @param serial A pointer to an activity in serial form
	 */
	void deserialise(const serial_ptr serial);

	
	void rehash(const hash512& salt);
protected:
	header_type	 m_header;
	content_type m_msg_content;
	
	void partial_hash();

};

inline unix_timestamp get_timestamp() {
	auto ts = activity_clock::now().time_since_epoch().count();
	return ts * activity_clock::period::num / activity_clock::period::den;	
}
#endif /* ACTIVITY_COMMON_HPP */

