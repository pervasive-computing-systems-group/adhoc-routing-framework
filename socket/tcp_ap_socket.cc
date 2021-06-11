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

#include "tcp_ap_socket.h"
using std::memset;

TCPAPSocket::TCPAPSocket(int portId)
		: TCPSocket(portId)
{
	bool ret_val = false;
	struct addrinfo tConfigAddr, *tAddrSet, *tAddrInfo;
	const int nYES = 1;


	// Configure the socket type
	memset(&tConfigAddr, 0, sizeof tConfigAddr);
	tConfigAddr.ai_family = AF_INET;		// Use IPv4
	tConfigAddr.ai_socktype = SOCK_STREAM;	// TCP
	tConfigAddr.ai_flags = AI_PASSIVE;		// Use local machine IP

	// Convert port number to c-string
    char str[5];
    sprintf(str, "%d", portId);

	// Get a set of socket addresses
	int nRVal = getaddrinfo(NULL, str, &tConfigAddr, &tAddrSet);
	if(nRVal) {
		fprintf(stderr,"[TCP SOCKET]:[ERROR]: getaddrinfo() failed: %s\n", gai_strerror(nRVal));
		exit(1);
	}

	tAddrInfo = tAddrSet;

	// Loop through addresses and try to connect
	while(tAddrInfo != NULL) {
		// Create listening socket
		sockfd = socket(tAddrInfo->ai_family, tAddrInfo->ai_socktype, tAddrInfo->ai_protocol);
		if(sockfd == -1) {
			if(TCP_DEBUG){
				printf("[TCP SOCKET]:[DEBUG]: Trying to connect to socket\n");
			}
			tAddrInfo = tAddrInfo->ai_next;

			continue;
		}

		// Set socket option to reuse address
		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &nYES, sizeof(int)) == -1) {
			fprintf(stderr,"[TCP SOCKET]:[ERROR]: setsockopt() failed\n");
			exit(1);
		}

		// Attempt to bind address info to socket
		if(bind(sockfd, tAddrInfo->ai_addr, tAddrInfo->ai_addrlen) == -1) {
			close(sockfd);
			if(TCP_DEBUG){
				printf("[TCP SOCKET]:[DEBUG]: Failed to bind socket\n");
			}
			tAddrInfo = tAddrInfo->ai_next;
			continue;
		}
		else {
			break;
		}
	}

	if(tAddrInfo == NULL) {
		fprintf(stderr,"[TCP SOCKET]:[ERROR]: failed to bind socket\n");
		exit(1);
	}
	else {
		ret_val = true;
	}

	// Free list
	freeaddrinfo(tAddrSet);

	// Enable socket to accept incoming connections with client queue
	if(listen(sockfd, CONNECTION_QUEUE_SIZE) == -1) {
		fprintf(stderr,"ERROR: unable to set socket to listen\n");
		exit(1);
	}
}

TCPAPSocket::~TCPAPSocket(){
	if(TCP_DEBUG){
		printf("[TCP SOCKET]:[DEBUG]: deconstructing TCP socket\n");
	}
	close(sockfd);
}

// Function used to listen to incoming calls to the access point
void TCPAPSocket::checkListeningPort() {
	struct sockaddr_storage tClientAddr;
	socklen_t tpClientAddrSize = sizeof(tClientAddr);
	char buffer[MAXLINE];
	Endpoint sender;
	socklen_t remoteHostLen = sizeof(*sender.getRemoteHostAddr());
	int nClientSocketID, nBytesRead;

	// Infinite loop to handle incoming connections
	while(true) {
		// Accept incoming connections
		nClientSocketID = accept(sockfd, (struct sockaddr *)&tClientAddr, &tpClientAddrSize);

		// Verify we connected
		if(nClientSocketID != -1) {
			sender.resetAddress();

			// Read packet from socket
			nBytesRead = recvfrom(nClientSocketID, buffer, MAXLINE, 0, (struct sockaddr*) sender.getRemoteHostAddr(), &remoteHostLen);

			// Verify we received data
			if (nBytesRead > 0) {
				// Check that the application states we are connected
				if(this->isACHConnected(sender.getAddressI())){
					// Add message to ring-buffer
					buffer[nBytesRead] = '\0';
					messages.push(Message(sender, buffer, nBytesRead));
				}
			}
			else if (nBytesRead < 0) {
				// Something went wrong receiving packet...
				if(TCP_DEBUG){
					fprintf(stderr, "[TCP SOCKET]:[ERROR]: Receiving data on port failed\n");
				}
			}

			close(nClientSocketID);
		}
	}
}

int TCPAPSocket::typeSendTo(Endpoint &remote, const char *packet, int length) {
	// The access point does not send packets
	if(TCP_DEBUG){
		fprintf(stderr, "[TCP SOCKET]:[ERROR]: Attempting to send packets from access point!\n");
	}

	return -1;
}

int TCPAPSocket::getSockfd() const{
  return sockfd;
}
