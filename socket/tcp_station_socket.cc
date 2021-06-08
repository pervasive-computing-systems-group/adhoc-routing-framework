/* Modified version of the following code, which was liscenced under MIT
 * source: https://github.com/ARMmbed/mbed-os
 */
/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "tcp_station_socket.h"
using std::memset;

TCPStationSocket::TCPStationSocket(int portId)
		: TCPSocket(portId)
{ }

TCPStationSocket::~TCPStationSocket(){
	if(TCP_DEBUG){
		printf("[TCP SOCKET]:[DEBUG]: deconstructing TCP socket\n");
	}
}

// -1 if unsuccessful, else number of bytes written
int TCPStationSocket::typeSendTo(Endpoint &remote, const char *packet, int length) {
	if(TCP_DEBUG){
		printf("[TCP SOCKET]:[DEBUG]: Sending %s to %s via TCP connection\n", packet, remote.getAddressC());
	}

	int returnVal = -1;
//	long arg;
	struct addrinfo tConfigAddr, *tAddrSet, *tAddrInfo;

	// Configure the socket type that we want
	memset(&tConfigAddr, 0, sizeof tConfigAddr);
	tConfigAddr.ai_family = AF_INET;		// Use IPv4
	tConfigAddr.ai_socktype = SOCK_STREAM;	// TCP
	tConfigAddr.ai_flags = AI_PASSIVE;		// Use local machine IP

	// Convert port number to c-string
    char str[5];
    sprintf(str, "%d", remote.getPort());

	// Get a set of socket addresses
	int nRVal = getaddrinfo(remote.getAddressC(), str, &tConfigAddr, &tAddrSet);
	if(nRVal != 0) {
		printf("ERROR: getaddrinfo() failed: %s\n", gai_strerror(nRVal));
		returnVal = -1;
	}
	else {
		tAddrInfo = tAddrSet;

		// Loop through addresses and try to connect
		while(tAddrInfo != NULL) {
			// Create socket
			sockfd = socket(tAddrInfo->ai_family, tAddrInfo->ai_socktype, tAddrInfo->ai_protocol);
			if(sockfd == -1) {
				if(TCP_DEBUG){
					printf("[TCP SOCKET]:[DEBUG]: Trying to connect to socket\n");
				}
				tAddrInfo = tAddrInfo->ai_next;

				continue;
			}

			int synRetries = 2; // Send a total of 3 SYN packets => Timeout ~7s
			setsockopt(sockfd, IPPROTO_TCP, TCP_SYNCNT, &synRetries, sizeof(synRetries));

			/*
			// Set non-blocking
			if( (arg = fcntl(sockfd, F_GETFL, NULL)) < 0) {
				printf("[TCP SOCKET]:[ERROR]: fcntl(..., F_GETFL) (%s)\n", strerror(errno));
				exit(0);
			}
			arg |= O_NONBLOCK;
			if( fcntl(sockfd, F_SETFL, arg) < 0) {
				printf("[TCP SOCKET]:[ERROR]: fcntl(..., F_SETFL) (%s)\n", strerror(errno));
				exit(0);
			}
			*/

			// Attempt to connect to server socket
			if(connect(sockfd, tAddrInfo->ai_addr, tAddrInfo->ai_addrlen) == -1) {
				// Failed to connect
				close(sockfd);
				if(TCP_DEBUG){
					printf("[TCP SOCKET]:[DEBUG]: Failed to connect to socket\n");
				}
				tAddrInfo = tAddrInfo->ai_next;

				continue;
			}
			else {
				break;
			}
		}

		// Free list
		freeaddrinfo(tAddrSet);

		if(tAddrInfo == NULL) {
			printf("ERROR: failed to connect\n");
			returnVal = -1;
		}
		else {
			returnVal = send(sockfd, packet, length, 0);

			if(returnVal < 0) {
				printf("[TCP SOCKET]:[ERROR] Could not send packet %s to %s\n", packet, remote.getAddressC());
				printf("[TCP SOCKET]:[ERROR]: %s\n", strerror(errno));
			}

			// Close the socket
			close(sockfd);
		}
	}

	return returnVal;
}

int TCPStationSocket::getSockfd() const{
  return sockfd;
}
