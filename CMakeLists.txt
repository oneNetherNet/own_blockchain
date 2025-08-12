cmake_minimum_required(VERSION 3.10)
project(own_blockchain)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART ON)

find_package(OpenSSL REQUIRED)
find_package(nlohmann_json 3.0 REQUIRED)

add_library(core_lib STATIC 
	src/core/block.cpp
	src/core/blockchain.cpp
)

target_include_directories(core_lib PUBLIC 
	${CMAKE_SOURCE_DIR}/include
)

add_executable(nether 
	src/main.cpp
)

target_link_libraries(nether PRIVATE
	core_lib
	OpenSSL::SSL 
	OpenSSL::Crypto 
	nlohmann_json::nlohmann_json
)