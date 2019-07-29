#include "IPv4.h"

IPv4::IPv4()
{
	m_data.version_IHL_DSCP_ECN = 0;
	set_version(4);
	set_IHL(5);	//without options
	set_DSCP(12);
	set_ECN(0);	//no-ECT
	set_total_length(0);	//no data yet
	set_random_ID();
	set_flags(0);
	set_fragment_offset(0);	//first
	set_TTL(255);
	set_protocol(17);	//udp
	set_source_address_raw(0);
	set_dest_address_raw(0xffffffff);	//broadcast

	calculate_checksum();
}

IPv4::~IPv4() {}; //implicit destructor


void IPv4::set_version(uint8_t version)
{
	m_data.version_IHL_DSCP_ECN = htons((ntohs(m_data.version_IHL_DSCP_ECN) & bit_mask(0, 12)) | ((version << 12) & ~bit_mask(0, 12)));
}

void IPv4::set_IHL(uint8_t IHL)
{
	m_data.version_IHL_DSCP_ECN = htons((ntohs(m_data.version_IHL_DSCP_ECN) & ~bit_mask(8, 12)) | ((IHL << 8) & bit_mask(8, 12)));
}

void IPv4::set_DSCP(uint8_t DSCP)
{
	m_data.version_IHL_DSCP_ECN = htons((ntohs(m_data.version_IHL_DSCP_ECN) & ~bit_mask(2, 8)) | ((DSCP << 2) & bit_mask(2, 8)));
}

void IPv4::set_ECN(uint8_t ECN)
{
	m_data.version_IHL_DSCP_ECN = htons((ntohs(m_data.version_IHL_DSCP_ECN) & ~bit_mask(0, 2)) | (ECN & bit_mask(0, 2)));
}

void IPv4::set_total_length(uint16_t data_length)
{
	m_data.total_length = htons(get_header_length() + data_length);
}

uint16_t IPv4::set_random_ID()
{
	std::srand(time(NULL));	//random seed
	return (m_data.identification = std::rand());
}

void IPv4::set_ID(uint16_t ID)
{
	m_data.identification = ID;
}

void IPv4::set_flags(uint8_t flags)
{
	m_data.flags_fragment_offset = htons((ntohs(m_data.flags_fragment_offset) & bit_mask(0, 13)) | ((flags << 13) & ~bit_mask(0, 13)));
}

void IPv4::set_fragment_offset(uint16_t fragment_offset)
{
	m_data.flags_fragment_offset = htons((ntohs(m_data.flags_fragment_offset) & ~bit_mask(0, 13)) | (fragment_offset & bit_mask(0, 13)));
}

void IPv4::set_TTL(uint8_t TTL)
{
	m_data.time_to_live_protocol = htons((ntohs(m_data.time_to_live_protocol) & bit_mask(0, 8)) | ((TTL << 8) & ~bit_mask(0, 8)));
}

void IPv4::set_protocol(uint8_t protocol)
{
	m_data.time_to_live_protocol = htons((ntohs(m_data.time_to_live_protocol) & ~bit_mask(0, 8)) | (protocol & bit_mask(0, 8)));
}

void IPv4::set_source_address_raw(uint32_t address)
{
	m_data.source_IP_address = htonl(address);
}

void IPv4::set_dest_address_raw(uint32_t address)
{
	m_data.dest_IP_address = htonl(address);
}

void IPv4::set_source_address(uint8_t address0, uint8_t address1, uint8_t address2, uint8_t address3)
{
	uint8_t complete_addr[4] = {address0, address1, address2, address3};
	uint32_t * together = reinterpret_cast<uint32_t*>(complete_addr);
	m_data.source_IP_address = htons(*together);
}

//source: https://stackoverflow.com/questions/26774761/ip-and-tcp-header-checksum-calculate-in-c
void IPv4::calculate_checksum()
{
  m_data.header_checksum = 0;
  uint16_t *addr = reinterpret_cast<uint16_t*>(&m_data);
  int count = get_header_length();
  register uint32_t sum = 0;

  // Sum up 2-byte values until none or only one byte left.
  while (count > 1) {
    sum += *(addr++);
    count -= 2;
  }

  // Add left-over byte, if any.
  if (count > 0) {
    sum += *(uint8_t *) addr;
  }

  // Fold 32-bit sum into 16 bits; we lose information by doing this,
  // increasing the chances of a collision.
  // sum = (lower 16 bits) + (upper 16 bits shifted right 16 bits)
  while (sum >> 16) {
    sum = (sum & 0xffff) + (sum >> 16);
  }

  m_data.header_checksum = ~sum;
}

void IPv4::commit(void * destination)
{
	std::memcpy(destination, &m_data, get_header_length());
}

uint8_t IPv4::get_version()
{
	return (ntohs(m_data.version_IHL_DSCP_ECN) & bit_mask(12, 16)) >> 12;
}

uint8_t IPv4::get_IHL()
{
	return (ntohs(m_data.version_IHL_DSCP_ECN) & bit_mask(8, 12)) >> 8;
}


uint16_t IPv4::get_header_length()
{
	return get_IHL()*4;
}
