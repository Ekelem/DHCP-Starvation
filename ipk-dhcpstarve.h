#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include <cstdint>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <net/if.h>

#include <linux/if_packet.h>


#include "ethernet_frame.h"
#include "IPv4.h"
#include "udp.h"
#include "dhcp.h"


class dhcp_request_generator
{
public:
	dhcp_request_generator(int argc, char * const argv[], const char *optstring);
	~dhcp_request_generator();

	/**
	 * @brief ask DHCP server for IP address
	 * @details ask DHCP server for IP address mark down address to taken_address vector
	 * @return true if server responded, false if not.
	 */

	void parse_arguments(int argc, char * const argv[], const char *optstring);
	bool beg_for_IP();
	void spam_DHCP_DISCOVERY();

private:

	/**
	 * @brief setup dhcp sockets
	 * @details setup broadcast udp sockets on port 67 and 68
	 */

	void create_dhcp_sockets();
	void create_send_socket();
	void create_listen_socket();


	char * interface;
	int dhcp_socket_send;	//socket descriptor
	int dhcp_socket_listen;	//socket descriptor
	std::vector<uint32_t> taken_address;

	fd_set rfds;
    struct timeval time_out;
};
