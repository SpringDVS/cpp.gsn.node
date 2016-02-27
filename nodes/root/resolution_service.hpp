#ifndef RESOLUTION_SERVICE_HPP
#define RESOLUTION_SERVICE_HPP
#include "protocol_handler.hpp"
#include <boost/array.hpp>
class resolution_service {
public:
	resolution_service(netspace_ios& ios, netspace_table& nstable, metaspace_gsn& msgsn);
	resolution_service(const resolution_service& orig) = delete;
	virtual ~resolution_service();

private:
	netspace_udp::socket m_socket;
	netspace_udp::endpoint m_remote_ep;
	
	netspace_table& m_nstable;
	metaspace_gsn& m_msgsn;
	
	protocol_handler m_proto;
	
	
	char m_recv[1024]; // this is not async safe!
	
	void start_recv();
	void receive(const boost::system::error_code& error, std::size_t);
};

#endif /* RESOLUTION_SERVICE_HPP */

