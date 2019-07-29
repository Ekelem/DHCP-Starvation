#ifndef _HEADER_ETHERNET_FRAME_
#define _HEADER_ETHERNET_FRAME_

/*
 Computer Communications and Networks 2017/2018
 Project n. 2, variant 2 (DHCP Starvation attack)

 Autor: Erik Kelemen
*/

#include <cstdint>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <net/if.h>

#include "IPv4.h"
#include "udp.h"
#include "dhcp.h"

//BIG_ENDIAN structure
#pragma pack(push, 1)	//no padding
struct ethernet_frame_data
{ 

	uint32_t dest_mac0;
	uint16_t dest_mac1;

	uint32_t source_mac0;
	uint16_t source_mac1;

	uint8_t ethernet_type0;
	uint8_t ethernet_type1;

	char data[1500];
};
#pragma pack(pop)

class ethernet_frame
{
public:
	/**
	 * @brief create ethernet frame
	 * @details allocate and fill memory with default values
	 */
	ethernet_frame();

	/**
	 * @brief implicit destructor
	 * @details implicit destructor of ethernet_frame class
	 */
	~ethernet_frame();

	/**
	 * @brief set destination MAC address
	 * @details set destination MAC address in ethernet frame data structure
	 * 
	 * @param mac0 first octet of MAC address
	 * @param mac1 second octet of MAC address
	 * @param mac2 third octet of MAC address
	 * @param mac3 fourth octet of MAC address
	 * @param mac4 fifth octet of MAC address
	 * @param mac5 sixth octet of MAC address
	 */
	void set_dest_mac(uint8_t mac0, uint8_t mac1, uint8_t mac2, uint8_t mac3, uint8_t mac4, uint8_t mac5);

	/**
	 * @brief set source MAC address
	 * @details set source MAC address in ethernet frame data structure
	 * 
	 * @param mac0 first octet of MAC address
	 * @param mac1 second octet of MAC address
	 * @param mac2 third octet of MAC address
	 * @param mac3 fourth octet of MAC address
	 * @param mac4 fifth octet of MAC address
	 * @param mac5 sixth octet of MAC address
	 */
	void set_source_mac(uint8_t mac0, uint8_t mac1, uint8_t mac2, uint8_t mac3, uint8_t mac4, uint8_t mac5);

	/**
	 * @brief set ethernet type
	 * @details set ethernet type(IEEE 802.3, etc.)
	 * 
	 * @param type [description]
	 */
	void set_ethernet_type(uint16_t type);

	/**
	 * @brief load IPv4 data
	 * @details compute position for layer 3 and fill pointed memory with data from instance
	 * 
	 * @param instance IPv4 data
	 */
	void load_IPv4(IPv4 & instance);

	/**
	 * @brief load udp data
	 * @details compute position for layer 4 and fill pointed memory with data from instance
	 * 
	 * @param instance udp data
	 */
	void load_udp(udp & instance);

	/**
	 * @brief load dhcp data
	 * @details compute position for layer 4 data and fill pointed memory with data from instance
	 * 
	 * @param instance dhcp data
	 */
	void load_dhcp(dhcp & instance);

	/**
	 * @brief get pointer on ethernet frame
	 * @details get pointer on beggining of ethernet frame
	 * @return beggining of ethernet frame
	 */
	void* get_ptr();

	/**
	 * @brief return ethernet frame size
	 * @details sum up sizes of all layers presented in ethernet frame and return total size
	 * @return ethernet frame total size
	 */
	unsigned int get_frame_size();


private:
	/**
	 * @brief compose default ethernet frame
	 * @details fill data of ethernet frame with default values
	 */
	void create_default();
	struct ethernet_frame_data m_data;

	unsigned int m_eth_h_size;	//layer 2 size
	unsigned int m_ip_h_size;	//layer 3 size
	unsigned int m_app_h_size;	//layer 4 size
	unsigned int m_data_size;	//layer 4 data size
	
};

#endif //_HEADER_ETHERNET_FRAME_
