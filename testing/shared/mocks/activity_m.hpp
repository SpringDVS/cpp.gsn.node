/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   activity_m.hpp
 * Author: cfg
 *
 * Created on 31 January 2016, 16:43
 */

#ifndef ACTIVITY_M_HPP
#define ACTIVITY_M_HPP
#include "activity_header_m.hpp"
class activity_m : public activity
{
public:
	activity_m() {
		m_header = activity_header_m();
		activity::content_type msg("foobar");
		m_header.msg_size = msg.length();
		m_msg_content = msg;
		partial_hash();
	}
};


#endif /* ACTIVITY_M_HPP */

