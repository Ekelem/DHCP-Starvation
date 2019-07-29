#ifndef _HEADER_IPV4_
#define _HEADER_IPV4_

/*
 Computer Communications and Networks 2017/2018
 Project n. 2, variant 2 (DHCP Starvation attack)

 Autor: Erik Kelemen
*/

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <netinet/in.h>

#include "common.h"
#include "udp.h"

//BIG_ENDIAN STRUCT
#pragma pack(push, 1)	//no padding
struct ipv4_data
{ 

	uint16_t version_IHL_DSCP_ECN;
	uint16_t total_length;

	uint16_t identification;
	uint16_t flags_fragment_offset;

	uint16_t time_to_live_protocol;
	uint16_t header_checksum;

	uint32_t source_IP_address;
	uint32_t dest_IP_address;
};
#pragma pack(pop)


class IPv4
{
public:
	/**
	 * @brief create IPv4 data
	 * @details allocate and fill IPv4 structure with default data
	 */
	IPv4();

	/**
	 * @brief implicit destructor
	 * @details implicit destructor of IPv4 data
	 */
	~IPv4();

	/**
	 * @brief set IHL
	 * @details set Internet Header Length
	 * 
	 * @param IHL Internet Header Length
	 */
	void set_IHL(uint8_t IHL);

	/**
	 * @brief set DSCP
	 * @details set Differentiated Services Code Point
	 * 
	 * @param DSCP Differentiated Services Code Point
	 */
	void set_DSCP(uint8_t DSCP);

	/**
	 * @brief set ECN
	 * @details set Explicit Congestion Notification
	 * 
	 * @param ECN Explicit Congestion Notification
	 */
	void set_ECN(uint8_t ECN);

	/**
	 * @brief get Version
	 * @details get Version
	 * @return Version
	 */
	uint8_t get_version();

	/**
	 * @brief get IHL
	 * @details get Internet Header Length
	 * @return Internet Header Length
	 */
	uint8_t get_IHL();

	/**
	 * @brief get DSCP
	 * @details get Differentiated Services Code Point
	 * @return Differentiated Services Code Point
	 */
	uint8_t get_DSCP();

	/**
	 * @brief get ECN
	 * @details get Explicit Congestion Notification
	 * @return Explicit Congestion Notification
	 */
	uint8_t get_ECN();

	/**
	 * @brief get IPV4 header length
	 * @details get IPV4 header length(depends on options field size)
	 * @return IPV4 header length
	 */
	uint16_t get_header_length();

	/**
	 * @brief get IPV4 total length
	 * @details get IPV4 total length, which is sum of data and header lengths
	 * @return IPV4 total length
	 */
	uint16_t get_total_length();

	/**
	 * @brief get IPV4 data length
	 * @details get IPv4 data length
	 * @return IPv4 data length
	 */
	uint16_t get_data_length();

	/**
	 * @brief set IPv4 total length according to data length
	 * @details compute total length as data length plus header length
	 * 
	 * @param data_length IPv4 total length
	 */
	void set_total_length(uint16_t data_length);

	/**
	 * @brief set random identifier
	 * @details set random identifier(16 bits)
	 * @return random identifier
	 */
	uint16_t set_random_ID();

	/**
	 * @brief set identifier
	 * @details set identifier
	 * 
	 * @param ID identifier
	 */
	void set_ID(uint16_t ID);

	/**
	 * @brief set flags
	 * @details set flags(broadcast, etc.)
	 * 
	 * @param flags flag value
	 */
	void set_flags(uint8_t flags);

	/**
	 * @brief set fragment offset
	 * @details set fragment offset(0 if first)
	 * 
	 * @param fragment_offset fragment offset
	 */
	void set_fragment_offset(uint16_t fragment_offset);

	/**
	 * @brief set TTL
	 * @details set Time To Live
	 * 
	 * @param TTL Time To Live
	 */
	void set_TTL(uint8_t TTL);

	/**
	 * @brief set protocol
	 * @details set protocol of application layer(UDP, ICMP, etc.)
	 * 
	 * @param protocol application layer protocol
	 */
	void set_protocol(uint8_t protocol);

	/**
	 * @brief set source IPv4 address
	 * @details set source IPv4 address
	 * 
	 * @param address source IPv4 address
	 */
	void set_source_address_raw(uint32_t address);

	/**
	 * @brief set destination IPv4 address
	 * @details set destination IPv4 address
	 * 
	 * @param address destination IPv4 address
	 */
	void set_dest_address_raw(uint32_t address);

	/**
	 * @brief set destination IPv4 address
	 * @details set destination IPv4 address(each octet separately)
	 * 
	 * @param address0 first octet of IPv4 source address
	 * @param address1 second octet of IPv4 source address
	 * @param address2 third octet of IPv4 source address
	 * @param address3 fourth octet of IPv4 source address
	 */
	void set_source_address(uint8_t address0, uint8_t address1, uint8_t address2, uint8_t address3);

	/**
	 * @brief set destination IPv4 address
	 * @details set destination IPv4 address
	 * 
	 * @param address0 first octet of IPv4 destination address
	 * @param address1 second octet of IPv4 destination address
	 * @param address2 third octet of IPv4 destination address
	 * @param address3 fourth octet of IPv4 destination address
	 */
	void set_dest_address(uint8_t address0, uint8_t address1, uint8_t address2, uint8_t address3);

	/**
	 * @brief calculate checksum
	 * @details evaluate checksum from data structure
	 */
	void calculate_checksum();

	/**
	 * @brief commit data into memory
	 * @details copy data into pointed memory
	 * 
	 * @param destination pointed memory
	 */
	void commit(void * destination);

private:
	/**
	 * @brief set version
	 * @details set version(IPv4 always 4)
	 * 
	 * @param version Version
	 */
	void set_version(uint8_t version);

	struct ipv4_data m_data;

	
};

#endif //_HEADER_IPV4_
