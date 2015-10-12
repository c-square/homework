#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <linux/udp.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <string>

#define _UDP_PROTOCOL 1
#define _ICMP_PROTOCOL 2

#define _DEFAULT_DESTINATION "8.8.8.8" //Google Server
#define _DEFAULT_PROTOCOL _ICMP_PROTOCOL
#define _DEFAULT_ATTEMPTS 3
#define _DEFAULT_TTL 255
#define _DEFAULT_TIMEOUT 10 //seconds
#define _DEFAULT_SEQUENCE_START 1

#define _TRACEROUTE_NOTHING_HAPPENED -2
#define _TRACEROUTE_ALL_OK -3
#define _TRACEROUTE_PACKET_DIED -4
#define _TRACEROUTE_SEND_ERROR -5
#define _TRACEROUTE_SOCKET_ERROR -6
#define _TRACEROUTE_PACKET_LOST -7
#define _TRACEROUTE_PACKET_READ_ERROR -8
#define _TRACEROUTE_RECV_OWN_PACKET -9


typedef int protocolType;
using namespace std;

//Structs
typedef struct ICMP_PACKET
{
	struct iphdr iph;
	struct icmphdr icmph;
	char fill[56];
	
} ICMP_PACKET;

typedef struct UDP_PACKET
{
	struct iphdr iph;
	struct udphdr udph;
	char fill[56];
	
} UDP_PACKET;


typedef struct PACKET
{
    int index;
    int isIcmp;
    int latency;
    union
    {
        ICMP_PACKET icmpPacket;
        UDP_PACKET udpPacket;
    };
} PACKET;


class Traceroute
{
private:
	string destinationAddr;
	double MsLatency;
	int MicroLatency;
	int sequence;

	protocolType protocol;
	int TTL;
	int attempts;
	int timeout;

	int bytesSent;
	int bytesRecv;
	
	ICMP_PACKET recvPacket;
	ICMP_PACKET sentICMPPacket;
	UDP_PACKET sentUDPPacket;
	
	//private functions
	void setDefaultValues();
public:
	Traceroute();
	Traceroute(protocolType prot);
	Traceroute(string destination, protocolType prot);
	Traceroute(string destination);

	//getters
	string getDestinationAddr();
	protocolType getCurrentProtocol();
	void getRecvICMPPacket (ICMP_PACKET *packet);
	void getSentICMPPacket (ICMP_PACKET *packet);
	void getSentUDPPacket (UDP_PACKET *packet);
	int getRecvBytes();
	int getSentBytes();
	double getLatencyMs();
	double getLatencyMicro();

	//setters
	void setDestinationAddr(std::string);
	void useUDP();
	void useICMP();
	void setAttemptNr(int nrAttempts);
	void setTTL (int TTLval);
	void setTimeout (int timeoutVal);

	//functional
	int trace();
	int traceWithUDP();
	int traceWithICMP();
};


////////////// HELP FUNCTIONS ////////////////
unsigned short Checksum(unsigned short*, int);
void getCurrentIP(char *buffer, int buflen);

