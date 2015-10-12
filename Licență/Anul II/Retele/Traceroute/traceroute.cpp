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
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <string>

#include "traceroute.h"

using namespace std;




////////////// HELP VARS /////////////////////
int IP_HEADER_SIZE = sizeof(struct iphdr);
int UDP_HEADER_SIZE = sizeof(struct udphdr);
int ICMP_HEADER_SIZE = sizeof(struct icmphdr);
int ICMP_PACKET_SIZE = sizeof(ICMP_PACKET);
int UDP_PACKET_SIZE = sizeof(UDP_PACKET);

	
Traceroute::Traceroute() 
{
	setDefaultValues();
}
Traceroute::Traceroute(protocolType prot) 
{
	setDefaultValues();
	this->protocol = prot;
}
Traceroute::Traceroute(string destination) 
{
	setDefaultValues();
	this->destinationAddr = destination;
}
Traceroute::Traceroute(string destination, protocolType prot)
{
	setDefaultValues();
	this->destinationAddr = destination;
	this->protocol = prot;
}


void Traceroute::setDefaultValues()
{
	this->destinationAddr = _DEFAULT_DESTINATION;
	this->protocol = _DEFAULT_PROTOCOL;
	this->attempts = _DEFAULT_ATTEMPTS;
	this->TTL = _DEFAULT_TTL;
	this->timeout = _DEFAULT_TIMEOUT; //seconds
	this->sequence = _DEFAULT_SEQUENCE_START;

	this->bytesSent = 0;
	this->bytesRecv = 0;
}

//getters
string Traceroute::getDestinationAddr() { return this->destinationAddr;}
protocolType Traceroute::getCurrentProtocol() { return this->protocol; }
void Traceroute::getRecvICMPPacket (ICMP_PACKET *packet) { *packet = this->recvPacket; }
void Traceroute::getSentICMPPacket (ICMP_PACKET *packet) { *packet = this->sentICMPPacket; }
void Traceroute::getSentUDPPacket (UDP_PACKET *packet)	{ *packet = this->sentUDPPacket; }
int Traceroute::getRecvBytes() { return this->bytesRecv; }
int Traceroute::getSentBytes() { return this->bytesSent; }
double Traceroute::getLatencyMs() { return this->MsLatency; }
double Traceroute::getLatencyMicro() { return (double)this->MicroLatency; }

//setters
void Traceroute::setDestinationAddr(std::string destination) { this->destinationAddr = destination; }
void Traceroute::useUDP() { this->protocol = _UDP_PROTOCOL; }
void Traceroute::useICMP() { this->protocol = _ICMP_PROTOCOL; }
void Traceroute::setAttemptNr(int nrAttempts) { this->attempts = nrAttempts; }
void Traceroute::setTTL (int TTLval) { this->TTL = TTLval; }
void Traceroute::setTimeout (int timeoutVal) { this->timeout = timeoutVal;}

//functional
int Traceroute::traceWithUDP()
{
	printf("Size of UPD packet:%d\n", UDP_PACKET_SIZE);

	struct sockaddr_in connection_out, connection_in;
	struct hostent *hname;
	struct in_addr* in;
	struct protoent *protoUDP=NULL;
	struct protoent *protoICMP=NULL;

	struct timeval waitTime;
	
	int myPort = 16449;
	int outerPort = 16963;
	int socketUDP;
	int socketICMP;
	fd_set read_flags,write_flags; // the flag sets for select
	
	int optionVal = 1; //to set the kernel NO_IPHDR Flag

	char sourceAddress[40];

	int i;
	int sel;                 
	int currentReq = 0;
	int addrLen;
	int waitingForRead, ok;

	int errorCode = _TRACEROUTE_NOTHING_HAPPENED;

	protoUDP = getprotobyname("UDP");
	protoICMP = getprotobyname("ICMP");
	
	bzero(sourceAddress, 40);
	bzero(&connection_out, sizeof(connection_out));
	bzero(&connection_in, sizeof(connection_in));
	bzero(&(this->sentUDPPacket), sizeof(this->sentUDPPacket));
	bzero(&(this->recvPacket), sizeof(this->recvPacket));

	//get addreses - my ip and destination server ip
	getCurrentIP(sourceAddress, 40);
	printf("My ip: %s\n", sourceAddress);
	hname = gethostbyname(this->destinationAddr.data());

	//initialize ipHeader
	this->sentUDPPacket.iph.version = 4;  				//versiunea de header
	this->sentUDPPacket.iph.ihl = 5;     				//numarul de 32bit WORDs pe care il contine header-ul (min 5)
	this->sentUDPPacket.iph.tos = 0;					//Type Of Service - MultiUse - noNeed
	this->sentUDPPacket.iph.tot_len = htons(UDP_PACKET_SIZE);	//Total Packet Size
	this->sentUDPPacket.iph.id = htons(0);              		//ID-ul specific trimitatorului 
	this->sentUDPPacket.iph.frag_off = 0;				//Fragmentation Offset - off
	this->sentUDPPacket.iph.ttl = this->TTL;				//TimeToLive
	//sendPacket.iph.protocol = protoUDP->p_proto;		//Protocolul = ICMP
	this->sentUDPPacket.iph.protocol = 17;				//Protocolul = ICMP
	this->sentUDPPacket.iph.saddr = inet_addr(sourceAddress);	//Adresa sursa
	this->sentUDPPacket.iph.daddr = *(long*)hname->h_addr;		//Adresa destinatie
	this->sentUDPPacket.iph.check = Checksum((unsigned short*)(&(this->sentUDPPacket.iph)), IP_HEADER_SIZE);//IP Header CheckSum (ONLY IPHEADER CHECKSUM!!)
	
	this->sentUDPPacket.udph.source = htons(myPort);
	this->sentUDPPacket.udph.dest = htons(outerPort);
	this->sentUDPPacket.udph.len = htons(UDP_PACKET_SIZE - IP_HEADER_SIZE);			
	this->sentUDPPacket.udph.check = 0;	
	
	if ((socketICMP = socket(PF_INET, SOCK_RAW, protoICMP->p_proto)) == -1) //open socket
	{
		errorCode = _TRACEROUTE_SOCKET_ERROR;  		//socket initialization failed
	}

	if ((socketUDP = socket(PF_INET, SOCK_RAW, IPPROTO_UDP)) == -1)
	{
		errorCode = _TRACEROUTE_SOCKET_ERROR;	
	}

	fcntl(socketUDP, F_SETFL, O_NONBLOCK);  					//setam socket-ul non-blocant
	fcntl(socketICMP, F_SETFL, O_NONBLOCK);
	setsockopt(socketUDP, IPPROTO_IP, IP_HDRINCL, &optionVal, sizeof(int)); // ii spunem Kernel-ului ca adaugam noi header-ul IP
	setsockopt(socketICMP, IPPROTO_IP, IP_HDRINCL, &optionVal, sizeof(int));

	connection_out.sin_family = AF_INET; 			//initializam setarile conexiunii
	connection_out.sin_addr.s_addr = *(long*)hname->h_addr;
	

	while (currentReq < this->attempts && errorCode == _TRACEROUTE_NOTHING_HAPPENED)    	//Cat timp n-am primit un pachet raspuns
	{	
		// set wait time
		waitTime.tv_sec = this->timeout;
		waitTime.tv_usec = 0;

		FD_ZERO(&read_flags);
		FD_ZERO(&write_flags);
		FD_SET(socketUDP, &write_flags);

		sel = select(socketUDP+1, &read_flags, &write_flags, (fd_set*)0, &waitTime);  //asteptam pana cand putem scrie pe socket
		if(sel < 0)
		{
			continue;  //socket not ready to write
		}	

		if (FD_ISSET(socketUDP, &write_flags))    //if we can write
		{
			FD_CLR(socketUDP, &write_flags); 	//clear the flag
			this->bytesSent = sendto(socketUDP, (char*)(&(this->sentUDPPacket)), UDP_PACKET_SIZE, 0, (struct sockaddr*)&connection_out, sizeof(struct sockaddr_in));
			if (this->bytesSent <= 0)
			{
				close(socketUDP);
				errorCode = _TRACEROUTE_SEND_ERROR;
			}
		}
		else
			continue;


		if (errorCode != _TRACEROUTE_NOTHING_HAPPENED) break;

		// RECEIVING TIME
		waitTime.tv_sec = this->timeout; //set wait time
		waitTime.tv_usec = 0;

		FD_ZERO(&read_flags);
		FD_ZERO(&write_flags);
		FD_SET(socketICMP, &read_flags);

		sel = select(socketICMP+1, &read_flags, &write_flags, (fd_set*)0, &waitTime);
		if(sel < 0)
		{
			currentReq++; 			//timed out response.
		}

		this->MicroLatency = (this->timeout - (int)waitTime.tv_sec - 1)*1000000 + 1000000-(int)waitTime.tv_usec;   //build time
		this->MsLatency = this->MicroLatency/1000;

		if (FD_ISSET(socketICMP, &read_flags))  //if we can read
		{
			FD_CLR(socketICMP, &read_flags); // clearflag

			addrLen = sizeof(connection_in);
			if (( this->bytesRecv = recvfrom(socketICMP, (char*)(&(this->recvPacket)), ICMP_PACKET_SIZE, 0, (struct sockaddr *)&connection_in, (unsigned int *)&addrLen)) == -1)
			{
				errorCode = _TRACEROUTE_PACKET_READ_ERROR;
			}
			else
			{
				if (this->recvPacket.iph.saddr != sentUDPPacket.iph.saddr && this->recvPacket.iph.daddr != sentUDPPacket.iph.daddr)
				{
					if (this->recvPacket.icmph.code == 3 && this->recvPacket.icmph.type == 3)
						errorCode = _TRACEROUTE_ALL_OK;
					else
					if (this->recvPacket.icmph.code == 0 && this->recvPacket.icmph.type == 11)
						errorCode = _TRACEROUTE_PACKET_DIED;		
				}
				else
				{
					errorCode = _TRACEROUTE_RECV_OWN_PACKET;
				}
			}
		}
		else
		{
			currentReq++; // request timeout
		}
	}
	close(socketICMP);
	close(socketUDP);

	if (currentReq >= this->attempts)
		return _TRACEROUTE_PACKET_LOST;
	return errorCode;

}

int Traceroute::traceWithICMP()
{
	struct sockaddr_in connection_out, connection_in;
	struct hostent *hname;
	struct in_addr* in;
	struct protoent *proto=NULL;

	struct timeval waitTime;
	
	int socketFD;
	fd_set read_flags,write_flags; // the flag sets for select
	
	int optionVal = 1; //to set the kernel NO_IPHDR Flag

	char sourceAddress[40];

	int i;
	int sel;                 
	int currentReq = 0;
	int addrLen;
	int waitingForRead, ok;

	int errorCode = _TRACEROUTE_NOTHING_HAPPENED;

	
	proto = getprotobyname("ICMP");
	

	bzero(sourceAddress, 40);
	bzero(&connection_out, sizeof(connection_out));
	bzero(&connection_in, sizeof(connection_in));
	bzero(&(this->sentICMPPacket), sizeof(this->sentICMPPacket));
	bzero(&(this->recvPacket), sizeof(this->recvPacket));

	//get addreses - my ip and destination server ip
	getCurrentIP(sourceAddress, 40);
	printf("My ip: %s\n", sourceAddress);
	hname = gethostbyname(this->destinationAddr.data());

	//initialize ipHeader
	this->sentICMPPacket.iph.version = 4;  				//versiunea de header
	this->sentICMPPacket.iph.ihl = 5;     				//numarul de 32bit WORDs pe care il contine header-ul (min 5)
	this->sentICMPPacket.iph.tos = 0;					//Type Of Service - MultiUse - noNeed
	this->sentICMPPacket.iph.tot_len = htons(ICMP_PACKET_SIZE);	//Total Packet Size
	this->sentICMPPacket.iph.id = htons(0);              		//ID-ul specific trimitatorului 
	this->sentICMPPacket.iph.frag_off = 0;				//Fragmentation Offset - off
	this->sentICMPPacket.iph.ttl = this->TTL;				//TimeToLive
	this->sentICMPPacket.iph.protocol = proto->p_proto;		//Protocolul = ICMP
	this->sentICMPPacket.iph.saddr = inet_addr(sourceAddress);	//Adresa sursa
	this->sentICMPPacket.iph.daddr = *(long*)hname->h_addr;		//Adresa destinatie
	this->sentICMPPacket.iph.check = Checksum((unsigned short*)(&(this->sentICMPPacket.iph)), IP_HEADER_SIZE);//IP Header CheckSum (ONLY IPHEADER CHECKSUM!!)
	
	this->sentICMPPacket.icmph.type = ICMP_ECHO;
	this->sentICMPPacket.icmph.code = 0;
	this->sentICMPPacket.icmph.un.echo.id = 3124;			//Should use a better one
	this->sentICMPPacket.icmph.un.echo.sequence = this->sequence++;	//Numarul pachetului din secventa de trimiteri
	this->sentICMPPacket.icmph.checksum = Checksum((unsigned short *)(&(this->sentICMPPacket.icmph)), ICMP_HEADER_SIZE);
	
	if ((socketFD = socket(PF_INET, SOCK_RAW, proto->p_proto)) == -1) //open socket
	{
		errorCode = _TRACEROUTE_SOCKET_ERROR;  		//socket initialization failed
	}

	fcntl(socketFD, F_SETFL, O_NONBLOCK);  					//setam socket-ul non-blocant
	setsockopt(socketFD, IPPROTO_IP, IP_HDRINCL, &optionVal, sizeof(int)); // ii spunem Kernel-ului ca adaugam noi header-ul IP

	connection_out.sin_family = hname->h_addrtype; 			//initializam setarile conexiunii
	connection_out.sin_addr.s_addr = *(long*)hname->h_addr;
	

	while (currentReq < this->attempts && errorCode == _TRACEROUTE_NOTHING_HAPPENED)    	//Cat timp n-am primit un pachet raspuns
	{	
		// set wait time
		waitTime.tv_sec = this->timeout;
		waitTime.tv_usec = 0;

		FD_ZERO(&read_flags);
		FD_ZERO(&write_flags);
		FD_SET(socketFD, &write_flags);

		sel = select(socketFD+1, &read_flags, &write_flags, (fd_set*)0, &waitTime);  //asteptam pana cand putem scrie pe socket
		if(sel < 0)
		{
			continue;  //socket not ready to write
		}	

		if (FD_ISSET(socketFD, &write_flags))    //if we can write
		{
			FD_CLR(socketFD, &write_flags); 	//clear the flag
			this->bytesSent = sendto(socketFD, (char*)(&(this->sentICMPPacket)), ICMP_PACKET_SIZE, 0, (struct sockaddr*)&connection_out, sizeof(struct sockaddr_in));
			if (this->bytesSent <= 0)
			{
				close(socketFD);
				errorCode = _TRACEROUTE_SEND_ERROR;
			}
		}
		else
			continue;


		if (errorCode != _TRACEROUTE_NOTHING_HAPPENED) break;

		// RECEIVING TIME
		waitTime.tv_sec = this->timeout; //set wait time
		waitTime.tv_usec = 0;

		FD_ZERO(&read_flags);
		FD_ZERO(&write_flags);
		FD_SET(socketFD, &read_flags);

		sel = select(socketFD+1, &read_flags, &write_flags, (fd_set*)0, &waitTime);
		if(sel < 0)
		{
			currentReq++; 			//timed out response.
		}

		this->MicroLatency = (this->timeout - (int)waitTime.tv_sec - 1)*1000000 + 1000000-(int)waitTime.tv_usec;   //build time
		this->MsLatency = this->MicroLatency/1000;

		if (FD_ISSET(socketFD, &read_flags))  //if we can read
		{
			FD_CLR(socketFD, &read_flags); // clearflag

			addrLen = sizeof(connection_in);
			if (( this->bytesRecv = recvfrom(socketFD, (char*)(&(this->recvPacket)), ICMP_PACKET_SIZE, 0, (struct sockaddr *)&connection_in, (unsigned int *)&addrLen)) == -1)
			{
				errorCode = _TRACEROUTE_PACKET_READ_ERROR;
			}
			else
			{
				if (this->recvPacket.iph.saddr != sentICMPPacket.iph.saddr && this->recvPacket.iph.daddr != sentICMPPacket.iph.daddr)
				{
					if (this->recvPacket.icmph.code == 0 && this->recvPacket.icmph.type == 0)
						errorCode = _TRACEROUTE_ALL_OK;
					else
					if (this->recvPacket.icmph.code == 0 && this->recvPacket.icmph.type == 11)
						errorCode = _TRACEROUTE_PACKET_DIED;		
				}
				else
				{
					errorCode = _TRACEROUTE_RECV_OWN_PACKET;
				}
			}
		}
		else
		{
			currentReq++; // request timeout
		}
	}
	close(socketFD);

	if (currentReq >= this->attempts)
		return _TRACEROUTE_PACKET_LOST;
	return errorCode;
}

int Traceroute::trace()
{
	if (this->protocol == _ICMP_PROTOCOL)
		return traceWithICMP();
	else
	if (this->protocol == _UDP_PROTOCOL)
		return traceWithUDP();
}




unsigned short Checksum(unsigned short *addr, int len)
{
	int suma = 0;
	u_short checksum = 0;
	u_short *p = addr;
	int lenSave = len;

	while (lenSave > 1)
	{
		suma += *p++;
		lenSave -= 2;
	}
	
	if (lenSave == 1)
	{
		*(u_char *) (&checksum) = *(u_char *) p;
		suma += checksum;
	}

	suma = (suma >> 16) + (suma & 0xffff); 
	suma += (suma >> 16); 
	checksum = ~suma;
 
	return checksum;
}

void getCurrentIP(char* buffer, int buflen)
{
	struct sockaddr_in name;
	struct sockaddr_in serv;
	char kGoogleDnsIp[] = "8.8.8.8";
    char* p;
	uint16_t kDnsPort = 53;
	int err;

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	assert(sock != -1);
	
	memset(buffer, 0, buflen);
	assert(buflen >= 16);
	memset(&serv, 0, sizeof(serv));

	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
	serv.sin_port = htons(kDnsPort);

	err = connect(sock, (struct sockaddr*) &serv, sizeof(serv));
	assert(err != -1);

	socklen_t namelen = sizeof(name);
	err = getsockname(sock, (struct sockaddr*) &name, &namelen);
	assert(err != -1);

    p = inet_ntop(AF_INET, &name.sin_addr, buffer, buflen);
	assert(p);

	close(sock);
}
