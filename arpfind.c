#include "arpfind.h"

int arpGet(struct arpmac *srcmac,char *ifname, char *ipStr)  
{  
	srcmac->index = if_nametoindex(ifname);
	FILE* fin = fopen("/proc/net/arp", "r");
	char* buf = malloc(31);
	while (!feof(fin)) {
		fscanf(fin, "%s", buf);
		if (feof(fin)) {
			fclose(fin);
			free(buf);
			return -1;
		}
		if (!strcmp(buf, ipStr)) {
			fscanf(fin, "%*s%*s%s", buf);
			sscanf(buf, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", srcmac->mac, srcmac->mac + 1, srcmac->mac + 2, srcmac->mac + 3, srcmac->mac + 4, srcmac->mac + 5); 
			fclose(fin);
			return 0;
		}
		fscanf(fin, "%*[^\n]");
	}
}  
