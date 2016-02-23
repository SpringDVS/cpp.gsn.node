#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>
class exception_base : std::exception 
{
public:	
	exception_base(const std::string& m) : m_msg(m)
	{ }

	const char* what() const noexcept {
		return m_msg.c_str();
	}
private:
	const std::string& m_msg;
};



#define EXCEPTION(ename)						\
class ename : public exception_base				\
{												\
public:											\
	ename(const std::string& m)					\
	: exception_base(m)							\
	{ }											\
}

#define EXCEPTION_STATIC(ename, msg)			\
class ename : public exception_base				\
{												\
public:											\
	ename()										\
	: exception_base(msg)						\
	{ }											\
}

EXCEPTION_STATIC(transaction_invalid_state, "Transaction object in invalid state");
EXCEPTION(transaction_read_fail);
EXCEPTION(transaction_write_fail);
EXCEPTION(netspace_invalid_addr);
EXCEPTION_STATIC(netspace_malformed_url, "Bad URL");
EXCEPTION(metaspace_db_fail);
EXCEPTION(metaspace_query_fail);
EXCEPTION(metaspace_query_noresult);
EXCEPTION_STATIC(dvsp_empty_content, "Empty packet content");

#endif /* EXCEPTIONS_HPP */

