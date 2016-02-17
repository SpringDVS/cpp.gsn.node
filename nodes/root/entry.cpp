#include <iostream>
#include <chrono>

#include "activity/activity.hpp"

using namespace std::chrono;
int main() {
	std::cout << "Root Node" << std::endl;
	std::cout << "Size: " << sizeof(activity_header::hash) << std::endl;
	
	return 0;
}
