#include <errno.h>
#include "sendetherip.h"

void get_if_mac(char* src_mac, char* if_name) {
	// code copied from the internet 
	struct ifreq ifr;
    struct ifconf ifc;
    char buf[1024];
    int success = 0;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock == -1) { /* handle error*/ };

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { /* handle error */ }

    struct ifreq* it = ifc.ifc_req;
    const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

    for (; it != end; ++it) {
		if (strcmp(it->ifr_name, if_name)) {
			continue;
		}
        strcpy(ifr.ifr_name, it->ifr_name);
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
            if (! (ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
                if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
                    success = 1;
                    break;
                }
            }
        }
        else { /* handle error */ }
    }

    if (success) memcpy(src_mac, ifr.ifr_hwaddr.sa_data, 6);
}

struct ip *fill_ip_packet(struct ip *ip_packet,unsigned short checksum)
{
	return ip_packet;
}
 
/* ip transmit */
void ip_transmit(struct ip *ip_packet,unsigned short checksum,char *name, unsigned char *nextmac,char *bufdata,int datalength)
{
	char skbuf[1500];
	char mac_addr[6];
	get_if_mac(mac_addr, name);

	struct ether_header* eth_header = skbuf;
	memcpy(eth_header->ether_dhost, nextmac, 6);
	memcpy(eth_header->ether_shost, mac_addr, 6);
	eth_header->ether_type = htons(ETHERTYPE_IP);
	memcpy(skbuf + ETHER_HEADER_LEN, ip_packet, IP_HEADER_LEN);
	memcpy(skbuf + ETHER_HEADER_LEN + IP_HEADER_LEN, ip_packet, datalength);

	int sendfd;
	if((sendfd=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_IP)))==-1)	
	{
		printf("sendfd() error\n");
		return -1;
	}	
	struct sockaddr_ll sadr;
	sadr.sll_ifindex = if_nametoindex(name);
	sadr.sll_halen = ETH_ALEN;
	memcpy(sadr.sll_addr, nextmac, ETH_ALEN);
	unsigned long retv = sendto(sendfd, skbuf, sizeof(skbuf), 0,
			&sadr, sizeof(struct sockaddr_ll));
	if (!~retv) {
		printf("%d\n", errno);
		switch (errno) {
			case EWOULDBLOCK: puts("EWOULDBLOCK"); break;
			case EBADF: puts("EBADF"); break;
			case ECONNRESET: puts("ECONNRESET"); break;
			case EDESTADDRREQ: puts("EDESTADDRREQ"); break;
			case EFAULT: puts("EFAULT"); break;
			case EINTR: puts("EINTR"); break;
			case EINVAL: puts("EINVAL"); break;
			case EISCONN: puts("EISCONN"); break;
			case EMSGSIZE: puts("EMSGSIZE"); break;
			case ENOBUFS: puts("ENOBUFS"); break;
			case ENOMEM: puts("ENOMEM"); break;
			case ENOTCONN: puts("ENOTCONN"); break;
			case ENOTSOCK: puts("ENOTSOCK"); break;
			case EOPNOTSUPP: puts("EOPNOTSUPP"); break;
			case EPIPE: puts("EPIPE"); break;
		}
	}
}
