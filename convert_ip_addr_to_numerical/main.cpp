#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cstdint>
//
#include <assert.h>


#define IP_ADDR_STRING_PROTO	"xxx.yyy.zzz.nnn"
#define IP_ADDR_STRING_SIZE		sizeof(IP_ADDR_STRING_PROTO)

// Disable safety-check:
//#define _CRT_SECURE_NO_WARNINGS	1


/**
@brief Tests IP-to-uint32 and uint32-to-IP conversions.
*/

uint32_t ipAddrToUlong(char *ipAddrStr)
{
	int numChars = 0;
	uint32_t num = 0;
	uint32_t val;
	char *tok;
	char *ptr;
	char strBuf[IP_ADDR_STRING_SIZE];

	strncpy(strBuf, ipAddrStr, IP_ADDR_STRING_SIZE);

	tok = strtok(strBuf, ".");
	while (tok != NULL)
	{
		val = strtoull(tok, &ptr, 0);
		num = (num << 8) + val;
		tok = strtok(NULL, ".");
		++numChars;
		if (numChars > IP_ADDR_STRING_SIZE)
		{
			printf("Malformed IP-address string!\r\n");
			num = 0;
			break;
		}
	}

	return(num);
}

uint32_t ipAddrToUlongSimple(char *ipAddrStr)
{
	int i = 0;
	int numChars = 0;
	uint32_t num = 0;
	uint32_t digits;
	char octet[4] = { '\0', '\0', '\0', '\0' };
	char strBuf[IP_ADDR_STRING_SIZE];
	char *c = ipAddrStr;

	while (numChars <= IP_ADDR_STRING_SIZE)
	{
		char ch = *c;
		if (ch == '.' || ch == '\0')
		{
			digits = atoi(octet) & 0x000000FF;
			num = (num & 0xFFFFFF00) | digits;
			num = (ch == '\0') ? num : (num << 8);
			memset(octet, 0, sizeof(octet));
			i = 0;
		}
		else
		{
			octet[i++] = ch;
			if (i > 3)
			{
				printf("Too many digits (>3) in octet - bailing out!\r\n");
				return 0;
			}
		}
		++numChars;
		++c;
	}

	return(num);
}


/**
@brief Convert 32-bit unsigned int to IP-address string.
@note Assumes Little-Endianness!
*/
void ipAddrNumericalToString(uint32_t ip, char *buf)
{
	uint8_t bytes[4];

	bytes[0] = ip & 0xFF;
	bytes[1] = (ip >> 8) & 0xFF;
	bytes[2] = (ip >> 16) & 0xFF;
	bytes[3] = (ip >> 24) & 0xFF;

	snprintf(buf, IP_ADDR_STRING_SIZE, 
		"%u.%u.%u.%u", 
		(const char *)bytes[3], 
		(const char *)bytes[2], 
		(const char *)bytes[1], 
		(const char *)bytes[0]);
}



int main(int argc, char *argv[])
{
	char chk[IP_ADDR_STRING_SIZE];

	char testIN1[] = "192.168.0.110";

	/********************* 1st TEST ************************/
	printf("testing using 'ipAddrToUlong()'\r\n", chk);

	uint32_t testOUT1 = ipAddrToUlong(testIN1);
	printf("%s numerical value: %u (0x%08X)\r\n", testIN1, testOUT1, testOUT1);
	//
	ipAddrNumericalToString(testOUT1, chk);
	printf("Readback: %s\r\n", chk);

	printf("testing using 'ipAddrToUlongSimple()'\r\n", chk);

	uint32_t testOUT2 = ipAddrToUlongSimple(testIN1);
	printf("%s numerical value: %u (0x%08X)\r\n", testIN1, testOUT2, testOUT2);
	//
	ipAddrNumericalToString(testOUT2, chk);
	printf("Readback: %s\r\n", chk);

	assert(testOUT1 == testOUT2);

	/********************* 2nd TEST ************************/
	char testIN2[] = "109.247.232.180";

	printf("testing using 'ipAddrToUlong()'\r\n", chk);

	testOUT1 = ipAddrToUlong(testIN2);
	printf("%s numerical value: %u (0x%08X)\r\n", testIN2, testOUT1, testOUT1);
	//
	ipAddrNumericalToString(testOUT1, chk);
	printf("Readback: %s\r\n", chk);

	printf("testing using 'ipAddrToUlongSimple()'\r\n", chk);

	testOUT2 = ipAddrToUlongSimple(testIN2);
	printf("%s numerical value: %u (0x%08X)\r\n", testIN2, testOUT2, testOUT2);
	//
	ipAddrNumericalToString(testOUT2, chk);
	printf("Readback: %s\r\n", chk);
	
	assert(testOUT1 == testOUT2);

	return 0;
}


