#include <iostream>
#include <chrono>
#include "resolution_service.hpp"
//#include "infrastructure/netspace_table.hpp"
//#include "infrastructure/metaspace_gsn.hpp"
using namespace std::chrono;
int main() {
	std::cout << "Root Node" << std::endl;
	netspace_table nstable;
	metaspace_gsn msgsn;
	try {
		netspace_ios ios;
		resolution_service rn(ios, nstable, msgsn);
		ios.run();
	} catch(std::exception& e) {
		std::cout << "E: " << e.what() << std::endl;
	}
	return 0;
}