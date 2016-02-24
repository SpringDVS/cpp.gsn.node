/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   metaspace_gsn.cpp
 * Author: cfg
 * 
 * Created on 23 February 2016, 11:14
 */

#include <boost/regex.hpp>

#include "infrastructure/metaspace_gsn.hpp"


metaspace_gsn::metaspace_gsn() {
	auto rc = sqlite3_open("gsn.db", &m_db);
	if(rc) throw metaspace_db_fail(sqlite3_errmsg(m_db));
}

metaspace_gsn::metaspace_gsn(const metaspace_gsn& orig)
	: m_db(orig.m_db)
{ }

metaspace_gsn::~metaspace_gsn() {
	sqlite3_close(m_db);
}

std::string metaspace_gsn::resolve_gsn(const std::string& query) {
	
	if(!query.length()) throw metaspace_query_fail("Empty query");
	
	std::string outcode;
	std::lock_guard<std::mutex> lock(m_mutex);
	if(has_outcode(query, outcode)) {
		return from_outcode(outcode);
	}
	
	return from_county(query);
}

bool metaspace_gsn::has_outcode(const std::string& query, std::string &outcode) {
	boost::regex pc_regex("^([A-Z,a-z]{2})([0-9]{2})(.*)$");
	boost::cmatch what;
	if(boost::regex_match(query.c_str(), what, pc_regex)) {
		outcode = query.substr(0, 4);
		std::transform(outcode.begin(), outcode.end(), outcode.begin(), ::tolower);
		return true;
	}
	outcode.clear();
	return false;
}

std::string metaspace_gsn::from_outcode(const std::string& outcode) {
	const char* sql = "SELECT geosub FROM geosub_meta WHERE postcode=(?)";
	sqlite3_stmt* st;
	if(sqlite3_prepare_v2(m_db, sql, -1, &st, NULL)) {
		throw metaspace_db_fail(sqlite3_errmsg(m_db));

	}
	sqlite3_bind_text(st, 1, outcode.c_str(), outcode.length(), nullptr);
	auto rc = 0;
	
	while( ( rc = sqlite3_step(st) ) != SQLITE_DONE) {
		if(rc == SQLITE_ROW) {
			auto str = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(st, 0)));
			return std::string(str, sqlite3_column_bytes(st, 0));
		} else {
			sqlite3_reset(st);
			return std::string("");
		}
	}
	
	sqlite3_reset(st);
	
	return std::string("");
}

std::string metaspace_gsn::from_county(const std::string& county) {
	const char* sql = "SELECT geosub FROM geosub_meta WHERE county=(?) LIMIT 1";
	sqlite3_stmt* st;
	if(sqlite3_prepare_v2(m_db, sql, -1, &st, NULL)) {
		throw metaspace_db_fail(sqlite3_errmsg(m_db));

	}
	sqlite3_bind_text(st, 1, county.c_str(), county.length(), nullptr);
	auto rc = 0;
	
	while( ( rc = sqlite3_step(st) ) != SQLITE_DONE) {
		if(rc == SQLITE_ROW) {
			auto str = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(st, 0)));
			return std::string(str, sqlite3_column_bytes(st, 0));
		} else {
			sqlite3_reset(st);
			return std::string("");
		}
	}
	
	sqlite3_reset(st);
	
	return std::string("");
}






