/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   activity_log_m.hpp
 * Author: cfg
 *
 * Created on 16 February 2016, 11:06
 */

#ifndef ACTIVITY_LOG_M_HPP
#define ACTIVITY_LOG_M_HPP
#include "activity_log.hpp"
class activity_log_m : public activity_log 
{
public:
	activity_log_m(unsigned int num) 
		: activity_log()
	{
		auto ts = get_timestamp();
		auto seed = hash_sha512(reinterpret_cast<char*>(&ts), sizeof(ts));
		auto t = begin_transaction();
		for(auto i = 0u; i < num; i++) {
			auto hash = hash_sha512(reinterpret_cast<char*>(seed.data()), seed.size());
			activity::header_type hdr;
			hdr.hash = hash;
			hdr.type = activity_type::root_verify;
			seed = hash;
			auto act = t.generate_record(hdr, std::string(reinterpret_cast<const char*>(hash.data())));
			t.write_record(act);
		}
	}
		
	activity_log_m(activity_log& alog, int to) 
		: activity_log()
	{
		auto tother = alog.begin_transaction();
		auto t = begin_transaction();
		
		for(const auto& r : tother) {
			auto act = r;
			if(to-- > 0)
				t.write_record(act);
			else
				break;
		}
	}
};

#endif /* ACTIVITY_LOG_M_HPP */

