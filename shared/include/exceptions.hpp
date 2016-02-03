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
#endif /* EXCEPTIONS_HPP */

