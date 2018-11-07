#include "arpfind.h"

int arpGet(struct arpmac *srcmac,char *ifname, char *ipStr)  
{  

    struct ifreq ifr;
    struct ifconf ifc;
    char buf[1024];
    int success = 0;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock == -1) { return -1; }

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { return -1; }

    struct ifreq* it = ifc.ifc_req;
    const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

    for (; it != end; ++it) {
        strcpy(ifr.ifr_name, it->ifr_name);
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
            if (! (ifr.ifr_flags & IFF_LOOPBACK)) { 
                if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
					if (!strcmp(ifr.ifr_name, ifname)) {
						srcmac->mac = ifr.ifr_hwaddr.sa_data;
						srcmac->index = if_nametoindex(ifname);
						return 0;
					}
                }
            }
        }
        else { return -1; }
    }
	return -1;
}  
