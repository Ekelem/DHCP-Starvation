#include "ethernet_frame.h"

ethernet_frame::ethernet_frame()
{
	m_eth_h_size=14;
	m_ip_h_size=0;

	set_dest_mac(0,1,2,3,4,5);
	set_source_mac(2,3,2,3,1,2);
	set_ethernet_type(ETH_P_IP);//(0x0800);	//IPv4 datagram
}

ethernet_frame::~ethernet_frame() {};	//implicit destructor

void ethernet_frame::set_dest_mac(uint8_t mac0, uint8_t mac1, uint8_t mac2, uint8_t mac3, uint8_t mac4, uint8_t mac5)
{
	uint8_t part0[4] = {mac0, mac1, mac2, mac3};
	uint32_t * together0 = reinterpret_cast<uint32_t*>(&part0);
	m_data.dest_mac0 = *together0;//htonl(*together0);

	uint8_t part1[2] = {mac4, mac5};
	uint16_t * together1 = reinterpret_cast<uint16_t*>(&part1);
	m_data.dest_mac1 = *together1;//htons(*together1);
}

void ethernet_frame::set_source_mac(uint8_t mac0, uint8_t mac1, uint8_t mac2, uint8_t mac3, uint8_t mac4, uint8_t mac5)
{
	uint8_t part0[4] = {mac0, mac1, mac2, mac3};
	uint32_t * together0 = reinterpret_cast<uint32_t*>(&part0);
	m_data.source_mac0 = *together0;

	uint8_t part1[2] = {mac4, mac5};
	uint16_t * together1 = reinterpret_cast<uint16_t*>(&part1);
	m_data.source_mac1 = *together1;
}

void ethernet_frame::set_ethernet_type(uint16_t type)
{
	m_data.ethernet_type0 = type / 256;
	m_data.ethernet_type1 = type % 256;
}

/*void ethernet_frame::load_IPv4(struct ip instance)
{
	m_ip_h_size=instance.ip_hl * 4;
	memcpy(m_data.data, &instance, sizeof(struct ip));
}*/

void ethernet_frame::load_IPv4(IPv4 & instance)
{
	m_ip_h_size=instance.get_header_length();
	instance.commit(m_data.data);
}

void ethernet_frame::load_udp(udp & instance)
{
	m_app_h_size=8;
	instance.commit(&(m_data.data[m_ip_h_size]));
}

void ethernet_frame::load_dhcp(dhcp & instance)
{
	instance.commit(&m_data.data[m_ip_h_size+m_app_h_size]);
	m_data_size = 242;
}

void* ethernet_frame::get_ptr()
{
	return &m_data;
}

unsigned int ethernet_frame::get_frame_size()
{
	return m_eth_h_size + m_ip_h_size;
}

