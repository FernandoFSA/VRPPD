#include <iostream>
#include "Route.h"

int main()
{
	// Example usage of the Route class
	routing::Route route;
	routing::Request request1{ 1, 0, 1 };
	routing::Request request2{ 2, 1, 2 };
	routing::Request request3{ 3, 2, 3 };
	routing::Request request4{ 4, 2, 1 };

	route.addRequest(request1);
	std::cout << "Route: " << route << std::endl;
	route.addRequest(request2);
	std::cout << "Route: " << route << std::endl;
	route.addRequest(request3);
	std::cout << "Route: " << route << std::endl;
	route.addRequest(request4);
	std::cout << "Route: " << route << std::endl;
	route.removeRequest(2);
	std::cout << "Route after removing request 2: " << route << std::endl;
	route.removeRequest(1);
	std::cout << "Route after removing request 1: " << route << std::endl;
	route.removeRequest(3);
	std::cout << "Route after removing request 3: " << route << std::endl;

	return 0;
}