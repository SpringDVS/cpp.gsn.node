#ifndef NETSPACE_URL_HPP
#define NETSPACE_URL_HPP
#include "infrastructure/net_common.hpp"

class netspace_url {
public:
	typedef std::vector<std::string> url_cmpt;
	typedef std::vector<std::string> cmpt_gsn;
	typedef std::string cmpt_glq;
	typedef std::string cmpt_nn;
	typedef std::string cmpt_res;
	typedef std::string cmpt_spq;

public:
	netspace_url();
	netspace_url(std::string url);
	netspace_url(const netspace_url& orig);
	virtual ~netspace_url();
	
	cmpt_gsn static_route();
	cmpt_glq geosub_query();
	cmpt_nn national_network();
	cmpt_res resource();
	std::string query();

	std::string url();
private:
	std::string m_url;
	cmpt_gsn m_gsn;
	cmpt_glq m_glq;
	cmpt_res m_res;
	cmpt_spq m_spq;

	void decode();
};

#endif /* NETSPACE_URL_HPP */

