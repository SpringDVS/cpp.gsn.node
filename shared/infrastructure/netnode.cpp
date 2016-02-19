/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   netnode.cpp
 * Author: cfg
 * 
 * Created on 19 February 2016, 16:09
 */

#include "infrastructure/netnode.hpp"

netnode::netnode()
	: m_type(netnode_type::undefined), m_addr(),  m_hostname() 
{ }
netnode::netnode(netnode_type type)
	: m_type(type), m_addr(),  m_hostname() 
{ }

netnode::netnode(netnode_type type, std::string host, std::string address) 
	: m_type(type), m_addr(netspace_addr::from_string(address)), m_hostname(host)
{ }

netnode::netnode(netnode_type type, std::string host, netspace_addr address) 
	: m_type(type), m_addr(address), m_hostname(host)
{ }

netnode::netnode(const netnode& orig)
	: m_type(orig.m_type), m_addr(orig.m_addr), m_hostname(orig.m_hostname)
{ }

netnode::~netnode() 
{ }


netnode_type netnode::type() {
	return m_type;
}

void netnode::set_type(netnode_type t) {
	m_type = t;
}

const netspace_addr& netnode::address() const {
	return m_addr;
}

void netnode::set_address(netspace_addr addr) {
	m_addr = addr;
}

std::string netnode::hostname() {
	return m_hostname;
}

void netnode::set_hostname(std::string host) {
	m_hostname = host;
}






