#ifndef _HEADER_UDP_
#define _HEADER_UDP_

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>


#pragma pack(push, 1)
struct udp_data
{
	uint16_t source_port;
	uint16_t dest_port;
	uint16_t length;
	uint16_t checksum;
};
#pragma pack(pop)

class udp
{
public:
	udp();
	~udp();

	void set_source_port(uint16_t port);
	void set_dest_port(uint16_t port);
	void set_length(uint16_t length);
	void set_checksum(uint16_t checksum);

	void commit(void * destination);

private:
	struct udp_data m_data;
	
};

#endif //_HEADER_UDP_
