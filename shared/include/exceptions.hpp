#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>


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

class transaction_invalid_state : public exception_base 
{
public:
	transaction_invalid_state() 
	: exception_base("Transaction object in invalid state")
	{ }

private:
		
};

class transaction_read_fail : public exception_base 
{
public:
	transaction_read_fail(const std::string& m)
	: exception_base(m)
	{ }
};

class transaction_write_fail : public exception_base 
{
public:
	transaction_write_fail(const std::string& m)
	: exception_base(m)
	{ }
};

class netspace_invalid_addr : public exception_base
{
public:
	netspace_invalid_addr(const std::string& m)
	: exception_base(m)
	{ }
};

class netspace_malformed_url : public exception_base 
{
public:
	netspace_malformed_url() 
	: exception_base("Bad URL") 
	{ }

};

class dvsp_empty_content : public exception_base 
{
public:
	dvsp_empty_content() 
	: exception_base("Empty Packet Content") 
	{ }

};
#endif /* EXCEPTIONS_HPP */

