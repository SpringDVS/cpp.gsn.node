/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   entry.cpp
 * Author: cfg
 *
 * Created on 27 February 2016, 10:18
 */

#include <cstdlib>
#include <boost/program_options.hpp>

#include "forge.hpp"

using namespace std;
using msgtype = dvsp_msgtype;
namespace po = boost::program_options;

msgtype parse_msgtype(std::string);


int main(int argc, char** argv) {
	
	//netspace_ipv4 destination{{127,0,0,1}};
	
	std::string content;
	auto ptype = msgtype::gsn_register_host;
	auto target = netspace_addr::from_string("127.0.0.1");
	
	po::options_description desc("Options");
	desc.add_options()
			("type",po::value<string>(),"Forge packet of type")
			("target",po::value<string>(),"Change target machine")
			("content",po::value<string>(),"String content")
			("help","Display this message");
	
	po::variables_map vars;
	po::store(po::parse_command_line(argc, argv, desc), vars);
	
	if(vars.count("help")) {
		cout << desc << std::endl;
		return 1;
	}
	
	if(vars.count("type")) {
		ptype = parse_msgtype(vars["type"].as<string>());
		if(ptype == msgtype::undefined) {
			cout << "Undefined or unimplemented packet type\n" << endl;
			return 1;
		}
	}
	
	if(vars.count("target")) {
		target = netspace_addr::from_string(vars["target"].as<string>());
	}
	
	if(vars.count("content")) {
		content = vars["content"].as<string>();
	}
	
	run_forge(ptype, target, content);
	return 0;
}

msgtype parse_msgtype(std::string str) {
		if(str == "gsn_unregister_host")
			return msgtype::gsn_unregister_host;
		else if(str == "gsn_resolution")
			return msgtype::gsn_resolution;
		else if(str == "gsn_local_area")
			return msgtype::gsn_local_area;
		else if(str == "gtn_root_nodes")
			return msgtype::gtn_root_nodes;
		else
			return msgtype::undefined;
}
