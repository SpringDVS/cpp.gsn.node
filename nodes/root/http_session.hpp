#ifndef ROOT_SESSION_HPP
#define ROOT_SESSION_HPP
#include "infrastructure/net_common.hpp"
#include "protocol_handler.hpp"
class http_session 
: public std::enable_shared_from_this<http_session>
{
public:
	typedef char serial_type;
	typedef char* serial_ptr;
public:
	http_session(netspace_tcp::socket s, protocol_handler& proto);
	http_session(const http_session&) = delete;
	
	void start();
private:
	enum { max_size = 1024 };
	netspace_tcp::socket m_socket;
	serial_type m_data[1024];
	protocol_handler& m_handler;
	
	
	void read();
	void write(std::size_t length);
};

#endif /* ROOT_SESSION_HPP */

