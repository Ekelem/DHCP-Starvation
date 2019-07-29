#ifndef _HEADER_DHCP_
#define _HEADER_DHCP_

#include <cstdint>
#include <cstring>
#include <arpa/inet.h>

/*
 Computer Communications and Networks 2017/2018
 Project n. 2, variant 2 (DHCP Starvation attack)

 Autor: Erik Kelemen
*/

enum dhcp_op
{
	REQUEST = 1,
	REPLY
};

//   Operation Code (op)
//
//            1   REQUEST
//            2   REPLY

enum dhcp_htype
{
	Ethernet_10mb = 1,
	Experimental_Ethernet_3mb,
	Amateur_Radio_AX_25,
	Proteon_ProNET_Token_Ring,
	Chaos,
	IEEE_802_Networks,
	ARCNET,
	Hyperchannel,
	Lanstar,
	Autonet_Short_Address,
	LocalTalk,
	LocalNet,
	Ultra_link,
	SMDS,
	Frame_Relay,
	Asynchronous_Transmission_Mode
};

//      Type   Description                                   References
//      ----   -----------                                   ----------
//        1    Ethernet (10Mb)                                    [JBP]
//        2    Experimental Ethernet (3Mb)                        [JBP]
//        3    Amateur Radio AX.25                                [PXK]
//        4    Proteon ProNET Token Ring                          [JBP]
//        5    Chaos                                              [GXP]
//        6    IEEE 802 Networks                                  [JBP]
//        7    ARCNET                                             [JBP]
//        8    Hyperchannel                                       [JBP]
//        9    Lanstar                                             [TU]
//       10    Autonet Short Address                             [MXB1]
//       11    LocalTalk                                         [JKR1]
//       12    LocalNet (IBM PCNet or SYTEK LocalNET)             [JXM]
//       13    Ultra link                                        [RXD2]
//       14    SMDS                                              [GXC1]
//       15    Frame Relay                                        [AGM]
//       16    Asynchronous Transmission Mode (ATM)              [JXB2]


#pragma pack(push, 1)	//no padding
struct dhcp_data
{ 
	uint8_t op;
	uint8_t htype;
	uint8_t hlen;
	uint8_t hops;	//8

	uint32_t xid;	//12

	uint16_t scs;
	uint16_t flags;	//16

	uint32_t ciaddr;
	uint32_t yiaddr;
	uint32_t siaddr;
	uint32_t qiaddr;	//32

	uint8_t chaddr[16];	//48

	char sname[64];
	char file[128];	//240

	char options[312];	//552
};
#pragma pack(pop)


class dhcp
{
public:

	/**
	 * @brief create default dhcp data
	 * @details constructor, fill dhcp data with default values
	 */

	dhcp();

	/**
	 * @brief implicit destructor
	 * @details implicit destructor of dhcp class
	 */
	~dhcp();

	/**
	 * @brief create dhcp discover message
	 * @details rewrite dhcp data with values representing DHCP DISCOVER message with provided identifier.
	 * 
	 * @param id provided identifier
	 */

	void create_discover_msg(uint32_t id);
	void create_request_msg(uint32_t id, uint32_t addr);

	/**
	 * @brief commit data into memory
	 * @details copy data into pointed memory
	 * 
	 * @param destination pointed memory
	 */

	void set_xid(uint32_t xid);
	void set_chaddr_MAC(uint8_t MAC0, uint8_t MAC1, uint8_t MAC2, uint8_t MAC3, uint8_t MAC4, uint8_t MAC5);
	
	void commit(void * destination);

private:
	struct dhcp_data m_data;
	
};

#endif //_HEADER_DHCP_
