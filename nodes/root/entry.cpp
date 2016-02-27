#include <iostream>
#include <chrono>
#include "resolution_service.hpp"
using namespace std::chrono;
int main() {
	std::cout << "Root Node" << std::endl;
	try {
		netspace_ios ios;
		resolution_service rn(ios);
		ios.run();
	} catch(std::exception& e) {
		std::cout << "E: " << e.what() << std::endl;
	}
	return 0;
}