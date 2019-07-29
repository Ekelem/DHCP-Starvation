CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: ipk-dhcpstarve Makefile

ipk-dhcpstarve: ipk-dhcpstarve.cpp IPv4.cpp ethernet_frame.cpp udp.cpp dhcp.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

