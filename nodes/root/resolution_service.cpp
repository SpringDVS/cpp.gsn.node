#include "resolution_service.hpp"
#include <boost/bind.hpp>
using nsudp = netspace_udp; // gsn resolution service

resolution_service::resolution_service(netspace_ios& ios)
	: m_socket(ios, nsudp::endpoint(nsudp::v4(), GSN_SERVICE_PORT))
{ start_recv(); }

resolution_service::~resolution_service() {
}

void resolution_service::start_recv() {
	m_socket.async_receive_from(
		boost::asio::buffer(&m_recv, 1024), m_remote_ep,
		boost::bind(
			&resolution_service::receive,this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void resolution_service::receive(const boost::system::error_code& error, std::size_t) {
	if(error) return;

	dvsp_packet in(m_recv);

	switch(in.header().type) {
		case dvsp_msgtype::gsn_register_host:
			std::cout << "gsn_register_host" << std::endl;
			break;
		case dvsp_msgtype::gsn_unregister_host:
			std::cout << "gsn_unregister_host" << std::endl;
			break;
		default:
			std::cout << "Unhandled" << std::endl;
			break;
	}
}




