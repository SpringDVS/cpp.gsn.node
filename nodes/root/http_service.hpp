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
	
	static void send_frame(const http_frame& frame, netspace_ipv4 address){
		char reply[4096];
		try {
		netspace_ios ios;
		netspace_tcp::resolver resolver(ios);
		auto nsaddr = ipv4_to_netspace_addr(address);
		netspace_tcp::resolver::query query(netspace_tcp::v4(), nsaddr.to_string(), "80");

		auto it = resolver.resolve(query);

		netspace_tcp::socket s(ios);
		s.connect(*it);
		auto serial = frame.serialise();
		boost::asio::write(s, boost::asio::buffer(serial.get(), frame.size()));
		
		s.read_some(boost::asio::buffer(reply, 4096));
		//std::cout << reply << std::endl;
		std::string r(reply);
		
		auto n = r.find("\r\n\r\n");
		auto split = r.substr(n+4);
		FILE* fp;
		auto header = r.substr(0, n);
		char bytes[1024];
		auto i = 0;
		for(auto j = 0u; j < split.length(); j++) {
			char d[2];
			d[0] = split[j++];
			d[1] = split[j];
			bytes[i++] = strtol(d, NULL,16);
		}
		dvsp_packet p(const_cast<char*>(bytes));
		fp = fopen("response.bin", "wb");
		fwrite(bytes, 1, i, fp );
		fclose(fp);
		
		std::cout << p.header().size << std::endl;
		} catch (std::exception& e) {
			std::cout << "Error: " << e.what() << std::endl;
			std::cout << reply << std::endl;
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

