#include "forge.hpp"
#include <boost/array.hpp>
using msgtype = dvsp_msgtype;
using rcode = dvsp_rcode;

packet_uptr dispatch_packet(packet_uptr packet);
std::string rcode_to_string(rcode code);

void view_frame_response(packet_uptr packet) {
	auto code = packet->content_as<frame_response_code>().response;
	std::cout 
		<< "Response: " << rcode_to_string(code) 
		<< std::endl << std::endl;
}

void forge_gsn_register_host(netspace_addr target) {
	packet_uptr p(new dvsp_packet);
	p->header().addr_dest = target.to_v4().to_bytes();
	p->header().size = 0;
	p->header().type = msgtype::gsn_register_host;
	auto out = dispatch_packet(std::move(p));
	view_frame_response(std::move(out));
}

void forge_gsn_unregister_host(netspace_addr target) {
	packet_uptr p(new dvsp_packet);
	p->header().addr_dest = target.to_v4().to_bytes();
	p->header().size = 0;
	p->header().type = msgtype::gsn_unregister_host;
	auto out = dispatch_packet(std::move(p));
	view_frame_response(std::move(out));
}

void run_forge(dvsp_msgtype type, netspace_addr target, std::string content) {
	packet_uptr p;
	switch(type) {
		case msgtype::gsn_register_host:
			std::cout << "Forging `gsn_register_host`... ";
			forge_gsn_register_host(target);
			break;
		case msgtype::gsn_unregister_host:
			std::cout << "Forging `gsn_unregister_host`... ";
			forge_gsn_unregister_host(target);
			break;
			
		default: break;
	}
	
	content.clear();
}

std::string rcode_to_string(rcode code) {
	switch(code) {
		case rcode::ok: return "ok";
		case rcode::malformed_content: return "malformed_content";
		case rcode::netspace_error: return "netspace_error";
		case rcode::network_error: return "network_error";
		default: return "undefined";
	}	
}

packet_uptr dispatch_packet(packet_uptr packet) {
	std::cout << "OK" << std::endl;
	netspace_ios io_service;
	netspace_udp::resolver resolver(io_service);

	netspace_addr addr(boost::asio::ip::address_v4(packet->header().addr_dest));
	netspace_udp::resolver::query query(netspace_udp::v4(), addr.to_string(), "55301");
	
	auto rcv_ep = *resolver.resolve(query);
	netspace_udp::socket socket(io_service);
	socket.open(netspace_udp::v4());
	
	auto ptr = packet->serialise();
	
	socket.send_to(boost::asio::buffer(ptr, packet->size()), rcv_ep);
	
	boost::array<char, 512> recv_buf;
	
	netspace_udp::endpoint snd_ep;
	
	socket.receive_from(boost::asio::buffer(recv_buf), snd_ep);
	return packet_uptr( new dvsp_packet(recv_buf.data()));
}