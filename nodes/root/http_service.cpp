#include "http_service.hpp"
#include "http_session.hpp"
using tcp = netspace_tcp;
using ecode = boost::system::error_code;
http_service::http_service(netspace_ios& service, netspace_table& nstable, metaspace_gsn& msgsn)
	: m_acceptor(service, tcp::endpoint(tcp::v4(), 80))
	, m_socket(service)
	, m_nstable(nstable)
	, m_msgsn(msgsn)
	, m_handler(m_nstable, m_msgsn)
{ 
	do_accept();
}

http_service::~http_service() {

}

void http_service::do_accept() {
	m_acceptor.async_accept(m_socket, [this](ecode ec){
		if(ec) {
			std::cout << "Error code" << std::endl;
			do_accept();
			return;
		}
		
		std::make_shared<http_session>(std::move(m_socket), m_handler)->start();
		do_accept();
	});
}
