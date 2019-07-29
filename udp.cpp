#include "udp.h"

udp::udp()
{
	set_source_port(0);
	set_dest_port(0);
	set_length(8);
	set_checksum(0);
}

udp::~udp() {};	//implicit destructor


void udp::set_source_port(uint16_t port)
{
	m_data.source_port = htons(port);
}

void udp::set_dest_port(uint16_t port)
{
	m_data.dest_port = htons(port);
}

void udp::set_length(uint16_t length)
{
	m_data.length = htons(length);
}

void udp::set_checksum(uint16_t checksum)
{
	m_data.checksum= htons(checksum);
}

void udp::commit(void * destination)
{
	std::memcpy(destination, &m_data, 8);
}