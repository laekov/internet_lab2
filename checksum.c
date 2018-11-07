#include "checksum.h"

int check_sum(unsigned short *iphd,int len,unsigned short checksum)
{
	unsigned int sum = 0;
	for (int i = 0; i < len; ++ i) {
		sum += *(iphd ++);
	}
	while (sum > 0xffffu) {
		sum = (sum >> 16) + (sum & 0xffffu);
	}
	return (~sum & 0xffffu) == 0u;
}
unsigned short count_check_sum(unsigned short *iphd)
{
	return *(iphd + 5) + 1;
}
