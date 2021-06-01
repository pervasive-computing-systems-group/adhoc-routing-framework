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

	int returnVal = 0;
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
		fprintf(stderr,"ERROR: getaddrinfo() failed: %s\n", gai_strerror(nRVal));
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
			fprintf(stderr,"ERROR: failed to connect\n");
			returnVal = -1;
		}
		else {
//			sendto(sockfd, packet, length, MSG_CONFIRM, (const struct sockaddr *)&remote.remoteHost, sizeof(remote.remoteHost));
			returnVal = send(sockfd, packet, length, 0);

			if(returnVal < 0){
				fprintf(stderr, "[TCP SOCKET]:[ERROR] Could not send packet %s to %s\n", packet, remote.getAddressC());
				fprintf(stderr, "[TCP SOCKET]:[ERROR]: %s\n", strerror(errno));
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
