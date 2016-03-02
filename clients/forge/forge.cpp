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

void view_frame_network(packet_uptr packet) {
	auto frame = packet->content_as<frame_network>();
	std::cout 
		<< "Response: " << rcode_to_string(frame.response) 
		<< std::endl;
	
	if(frame.response != rcode::ok) {
		std::cout << std::endl;
		return;
	}
	
	for(auto i = 0; i < frame.total; i++) {
		auto node = packet->content_as<netspace_ipv4>(sizeof(frame_network) + 1 + (4*i));
		std::cout << ipv4_to_string(node) << std::endl;
	}
	
	std::cout << std::endl;
}

void view_frame_hostname(packet_uptr packet) {
	auto frame = packet->content_as<frame_hostname>();
	std::cout 
		<< "Response: " << rcode_to_string(frame.response) 
		<< std::endl;
	
	if(frame.response != rcode::ok) {
		std::cout << std::endl;
		return;
	}
	std::string proto = "dvsp";
	std::string hostname;
	hostname.assign(frame.hostname.data(), frame.len);
	
	if(frame.protocol == service_protocol::http)
		proto = "http";
	
	std::cout << hostname << ":" << proto << std::endl << std::endl;
}

void forge_gsn_register_host(netspace_addr target, std::string& hostname, service_protocol proto) {
	packet_uptr p(new dvsp_packet);
	p->header().addr_dest = target.to_v4().to_bytes();
	p->header().size = 0;
	p->header().type = msgtype::gsn_register_host;
	auto frame = construct_frame_register(netnode_type::org, hostname, proto);
	p->copy_content(&frame, sizeof(frame_register));
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

void forge_gsn_local_area(netspace_addr target) {
	packet_uptr p(new dvsp_packet);
	p->header().addr_dest = target.to_v4().to_bytes();
	p->header().size = 0;
	p->header().type = msgtype::gsn_local_area;
	auto out = dispatch_packet(std::move(p));
	view_frame_response(std::move(out));
}



void forge_gsn_request(netspace_addr target, std::string content) {
	packet_uptr p(new dvsp_packet);
	p->header().addr_dest = target.to_v4().to_bytes();
	p->header().size = 0;
	p->header().type = msgtype::gsn_request;
	p->str_content(content);
	auto out = dispatch_packet(std::move(p));
	view_frame_network(std::move(out));
}

void forge_gsn_hostname(netspace_addr target, std::string content) {
	packet_uptr p(new dvsp_packet);
	p->header().addr_dest = target.to_v4().to_bytes();
	p->header().size = 0;
	p->header().type = msgtype::gsn_hostname;
	auto addr = netspace_addr::from_string(content).to_v4().to_bytes();
	p->copy_content(addr.data(), 4);
	auto out = dispatch_packet(std::move(p));
	view_frame_hostname(std::move(out));
}

void run_forge(dvsp_msgtype type, netspace_addr target, std::string content, service_protocol proto) {
	switch(type) {
		case msgtype::gsn_register_host:
			std::cout << "Forging `gsn_register_host`... ";
			forge_gsn_register_host(target, content, proto);
			break;
		case msgtype::gsn_unregister_host:
			std::cout << "Forging `gsn_unregister_host`... ";
			forge_gsn_unregister_host(target);
			break;
			
		case msgtype::gsn_local_area:
			std::cout << "Forging `gsn_local_area`... ";
			forge_gsn_local_area(target);
			break;
			
		case msgtype::gsn_hostname:
			std::cout << "Forging `gsn_local_area`... ";
			forge_gsn_hostname(target, content);
			break;
		case msgtype::gsn_request:
			std::cout << "Forging `gsn_request`... ";
			forge_gsn_request(target, content);
			break;
		default:
			std::cout << "Unimplemented frame" << std::endl; 
			break;
	}
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
	
	packet->header().addr_orig = socket.local_endpoint().address().to_v4().to_bytes();
	auto ptr = packet->serialise();
	
	socket.send_to(boost::asio::buffer(ptr, packet->size()), rcv_ep);
	
	boost::array<char, 512> recv_buf;
	
	netspace_udp::endpoint snd_ep;
	
	socket.receive_from(boost::asio::buffer(recv_buf), snd_ep);
	return packet_uptr( new dvsp_packet(recv_buf.data()));
}
