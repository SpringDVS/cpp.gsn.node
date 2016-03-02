#ifndef HTTP_SERVICE_HPP
#define HTTP_SERVICE_HPP
#include "protocol/http_frame.hpp"
#include "infrastructure/metaspace_gsn.hpp"
#include "infrastructure/netspace_table.hpp"
#include "protocol_handler.hpp"
#include<cstdio>
class http_service {
public:
	http_service(netspace_ios& service, netspace_table& nstable, metaspace_gsn& msgsn);
	http_service(const http_service&) = delete;
	~http_service();
	
	static dvsp_packet send_frame(const http_frame& frame, netspace_ipv4 address){
		char reply[4096];
		try {
		netspace_ios ios;
		netspace_tcp::resolver resolver(ios);
		auto nsaddr = ipv4_to_netspace_addr(address);
		netspace_tcp::resolver::query query(netspace_tcp::v4(), nsaddr.to_string(), "8080");

		auto it = resolver.resolve(query);

		netspace_tcp::socket s(ios);
		s.connect(*it);
		auto serial = frame.serialise();
		boost::asio::write(s, boost::asio::buffer(serial.get(), frame.size()));
		
		s.read_some(boost::asio::buffer(reply, 4096));
		
		std::string r(reply);
		auto raw = http_to_bin(reply);
		return dvsp_packet(raw.get());
		
		} catch (std::exception& e) {
			throw e;
		}
	};
private:
	
	netspace_tcp::acceptor m_acceptor;
	netspace_tcp::socket m_socket;
	
	netspace_table& m_nstable;
	metaspace_gsn& m_msgsn;
	protocol_handler m_handler;
	
	void do_accept();
};

#endif /* HTTP_SERVICE_HPP */

