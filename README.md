# VRPPD Route Library

A C++20 library for managing routes and node dependencies, designed for vehicle routing problems with pickup and delivery (VRPPD).  
This library provides classes to represent nodes, requests, and routes, supporting dependency management and request validation.

## Features

- **Node and Route Modeling:**  
  Represent nodes and routes with predecessor/successor relationships.

- **Request Management:**  
  Add, validate, and remove pickup/delivery requests with dependency checks.

- **Operator Overloads:**  
  Stream output for easy debugging and logging.

## Usage
#include "Route.h"
int main() { routingRoute route; routingRequest request1{ 1, 0, 1 }; route.addRequest(request1); stdcout << "Route: " << route << stdendl; // ... more operations }
See [`main.cpp`](src/main.cpp) for a complete example.

## Requirements

- C++20 compatible compiler

## File Overview

- `src/Route.h` – Main library header
- `src/Route.cpp` – Implementation
- `src/main.cpp` – Example usage

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.
