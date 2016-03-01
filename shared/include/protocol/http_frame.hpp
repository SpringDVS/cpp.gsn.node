/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   http_frame.hpp
 * Author: cfg
 *
 * Created on 28 February 2016, 14:31
 */

#ifndef HTTP_FRAME_HPP
#define HTTP_FRAME_HPP
#include "protocol/dvsp_packet.hpp"

class http_frame {
public:
	typedef std::size_t size_type;
public:
	http_frame(dvsp_packet frame, std::string hostname);
	http_frame(const http_frame& orig);
	virtual ~http_frame();
	
	const netspace_ipv4& address() const;
	const std::string& hostname() const;
	std::string header() const;
	size_type size() const;
	
	std::unique_ptr<char[]> serialise() const;
private:
	dvsp_packet m_dvsp_frame;
	std::string m_hostname;
	std::string m_header;
	netspace_ipv4 m_address;
	
	void generate_header();
};

#endif /* HTTP_FRAME_HPP */
