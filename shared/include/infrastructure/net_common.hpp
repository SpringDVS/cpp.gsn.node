/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   net_common.hpp
 * Author: cfg
 *
 * Created on 19 February 2016, 16:39
 */

#ifndef NET_COMMON_HPP
#define NET_COMMON_HPP

#include <boost/asio.hpp>
#include "common.hpp"

enum class netnode_type : char {
	undefined, root, org, trusted, georoot, _final
};


using netspace_addr = boost::asio::ip::address;
using netspace_ipv4 = boost::asio::detail::array<std::uint8_t, 4>;
using netspace_tcp = boost::asio::ip::tcp;
using netspace_ios = boost::asio::io_service;

#define NS_PORT 55400


#endif /* NET_COMMON_HPP */

