#include "resolution_service.hpp"
#include <boost/bind.hpp>
using nsudp = netspace_udp; // gsn resolution service
using berror = boost::system::error_code;

resolution_service::resolution_service(netspace_ios& ios, netspace_table& nstable, metaspace_gsn& msgsn)
	: m_socket(ios, nsudp::endpoint(nsudp::v4(), GSN_SERVICE_PORT))
	, m_nstable(nstable)
	, m_msgsn(msgsn)
	, m_proto(m_nstable, m_msgsn)
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
	auto inbound_addr = m_remote_ep.address();
	auto out = m_proto.process_packet(in, inbound_addr);
	auto out_shr = std::shared_ptr<char>(out->serialise());
	m_socket.async_send_to(
		boost::asio::buffer(out_shr.get(), out->size()),
		m_remote_ep,
		[this](const berror&, std::size_t) { 
	});
	start_recv();
}




