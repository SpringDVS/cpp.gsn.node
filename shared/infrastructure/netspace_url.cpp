#include "infrastructure/netspace_url.hpp"

netspace_url::netspace_url() {
}

netspace_url::netspace_url(std::string url) 
	: m_url(url)
{
	decode();
}

netspace_url::netspace_url(const netspace_url& orig) 
	: m_url(orig.m_url)
{ }

netspace_url::~netspace_url() {
}


void netspace_url::decode() {
	
	if(m_url.length() <= 9 
	|| m_url.substr(0, 9) != "spring://") 
		throw netspace_malformed_url();

	enum cmpt {
		gsn, glq, res, spq
	};	
	auto url = m_url.substr(9);
	std::string t;
	
	auto state = cmpt::gsn;
	auto len = url.length();
	for(auto i = 0u; i <= len; i++) {
		
		auto ch = url[i];
		
		switch(ch) {

		case '.':
			m_gsn.push_back(t);
			t.clear();
			break;

		case ':':
			m_gsn.push_back(t);
			t.clear();
			state = cmpt::glq;
			break;
			
		case '/':
			if(state == cmpt::gsn) m_gsn.push_back(t);
			else if(state == cmpt::glq) m_glq = t;
			t.clear();
			state = cmpt::res;
			break;

		case '?':
			if(state == cmpt::gsn) m_gsn.push_back(t);
			else if(state == cmpt::glq) m_glq = t;
			else if(state == cmpt::res) m_res = t;
			t.clear();
			state = cmpt::spq;
			break;

		case '\0':
			if(state == cmpt::gsn) m_gsn.push_back(t);
			else if(state == cmpt::glq) m_glq = t;
			else if(state == cmpt::res) m_res = t;
			else if(state == cmpt::spq) m_spq = t;
			t.clear();
			break;

		default:
			t.push_back(ch);
		}
		
	}
	
	if(!m_gsn.size()) throw netspace_malformed_url();
}

netspace_url::cmpt_glq netspace_url::geosub_query() {
	return m_glq;
} 

netspace_url::cmpt_nn netspace_url::national_network() {
	return m_gsn.back();
}

std::string netspace_url::query() {
	return m_spq;
}

netspace_url::cmpt_res netspace_url::resource() {
	return m_res;
}

netspace_url::cmpt_gsn netspace_url::static_route() {
	return m_gsn;
}

std::string netspace_url::url() {
	return m_url;
}





