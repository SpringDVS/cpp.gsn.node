#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/basic_stream_socket.hpp>

#include "http_session.hpp"

http_session::http_session(netspace_tcp::socket s, protocol_handler& proto)
	: m_socket(std::move(s))
	, m_handler(proto)
{ }

void http_session::start() {
	read();
}

void http_session::read() {
	auto self(shared_from_this());
	m_socket.async_read_some(boost::asio::buffer(m_data, max_size),
		[this, self](boost::system::error_code e, std::size_t) {
			if(e) return;
			std::cout << "Receiving HTTP data:" << std::endl;
			for(auto i = 0; i < 100; i++) {
				std::cout << m_data[i];
			}
			
			std::cout << std::endl;
			//m_handler.process_packet(dvsp_packet(m_data), m_socket.remote_endpoint().address());
		});
}

void http_session::write(std::size_t length) {
	auto self(shared_from_this());
	boost::asio::async_write(m_socket, boost::asio::buffer(m_data, length),
	[this, self](boost::system::error_code e, std::size_t) {
		if(!e) {
			read();
		}
	});
}
