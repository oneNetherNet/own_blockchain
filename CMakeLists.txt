cmake_minimum_required(VERSION 3.10)
project(own_blockchain)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART ON)

find_package(OpenSSL REQUIRED)
find_package(nlohmann_json 3.0 REQUIRED)

add_executable(nether 
	src/main.cpp
)

target_link_libraries(nether
	PRIVATE OpenSSL::SSL OpenSSL::Crypto nlohmann_json::nlohmann_json
)