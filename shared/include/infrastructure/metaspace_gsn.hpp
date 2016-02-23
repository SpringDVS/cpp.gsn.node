#ifndef METASPACE_GSN_HPP
#define METASPACE_GSN_HPP

#include <sqlite3.h>
#include "common.hpp"

class metaspace_gsn {
public:
	metaspace_gsn();
	metaspace_gsn(const metaspace_gsn& orig);
	virtual ~metaspace_gsn();
	
	std::string resolve_gsn(const std::string& query);
	bool has_outcode(const std::string& query, std::string& outcode);

private:
	std::string from_outcode(const std::string& outcode);
	std::string from_county(const std::string& county);
	sqlite3 *m_db;
};

#endif /* METASPACE_GSN_HPP */

