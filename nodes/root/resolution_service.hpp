#ifndef RESOLUTION_SERVICE_HPP
#define RESOLUTION_SERVICE_HPP
#include "protocol_handler.hpp"
#include <boost/array.hpp>
class resolution_service {
public:
	resolution_service(netspace_ios& ios);
	resolution_service(const resolution_service& orig) = delete;
	virtual ~resolution_service();

private:
	void start_recv();
	void receive(const boost::system::error_code& error, std::size_t);
	netspace_udp::socket m_socket;
	netspace_udp::endpoint m_remote_ep;
	char m_recv[1024];
};

#endif /* RESOLUTION_SERVICE_HPP */

