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

#include "tcp_socket.h"
using std::memset;


TCPSocket::TCPSocket() : Socket(), messages(TCP_QUEUE_SIZE) {
}

TCPSocket::~TCPSocket(){
	if(TCP_DEBUG){
		printf("[TCP SOCKET]:[DEBUG]: deconstructing udp socket\n");
	}
	close(sockfd);
}

bool TCPSocket::init(void) {
	if(TCP_DEBUG) {
		printf("[TCP SOCKET]:[DEBUG]: Initializing udp socket\n");
	}

	return initSocket(SOCK_STREAM);
}

// Server initialization
bool TCPSocket::bindToPort(int port) {
	if(TCP_DEBUG){
		printf("[TCP SOCKET]:[DEBUG]: Binding udp socket to port %d\n", port);
	}

	if(sockfd < 0) {
		if (!initSocket(SOCK_STREAM))
			return false;
	}

	struct sockaddr_in localHost;
	std::memset(&localHost, 0, sizeof(localHost));

	localHost.sin_family = AF_INET;
	localHost.sin_port = htons(port);
	localHost.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockfd, (const struct sockaddr*) &localHost, sizeof(localHost)) < 0) {
		close(sockfd);

		if(TCP_DEBUG){
			printf("[TCP SOCKET]:[DEBUG]: Unsuccessful binding of tcp socket to port %d\n", port);
		}
		return false;
	}

	if(TCP_DEBUG){
		printf("[TCP SOCKET]:[DEBUG]: Successful binding of tcp socket to port %d\n", port);
	}

	return true;
}

bool TCPSocket::joinMulticastGroup(const char *address) {
  struct ip_mreq mreq;

  // Set up group address
  mreq.imr_multiaddr.s_addr = inet_addr(address);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);

  return setOption(IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
}

bool TCPSocket::setBroadcasting(bool broadcast) {
	int option = (broadcast) ? (1) : (0);
	if(TCP_DEBUG){
		printf("[TCP SOCKET]:[DEBUG]: Setting tcp socket to broadcast mode\n");
	}

	return setOption(SOL_SOCKET, SO_BROADCAST, &option, sizeof(option));
}

// -1 if unsuccessful, else number of bytes written
int TCPSocket::sendTo(Endpoint &remote, const char *packet, int length) {
  if(TCP_DEBUG){
    printf("[TCP SOCKET]:[DEBUG]: Sending %s to %s via TCP\n", packet, remote.getAddressC());
  }
  if (sockfd < 0) {
    fprintf(stderr, "[TCP SOCKET]:[ERROR]: sockfd is in error state\n");
    if(TCP_DEBUG){
      fprintf(stderr, "[TCP SOCKET]:[ERROR]: %s\n", strerror(errno));
    }
    return -1;
  }

  int returnVal = sendto(sockfd, packet, length, MSG_CONFIRM,
                (const struct sockaddr *)&remote.remoteHost,
                sizeof(remote.remoteHost));
  if(returnVal < 0){
    fprintf(stderr, "[TCP SOCKET]:[ERROR] Could not send packet %s to %s\n", packet, remote.getAddressC());
    fprintf(stderr, "[TCP SOCKET]:[ERROR]: %s\n", strerror(errno));
  }
  return returnVal;
}

int TCPSocket::sendTo(char *buffer, int length, uint32_t dest, int port) {
  Endpoint remote;
  remote.setAddress(dest, port);
  return sendTo(remote, buffer, length);
}

int TCPSocket::receiveFrom(Endpoint &sender, char *buffer, int length) {
	int ret_val = -1;
	// -1 if unsuccessful, else number of bytes received
	if (sockfd < 0) {
		fprintf(stderr, "[TCP SOCKET]:[ERROR]: TCP socketfd is in error state while trying to receive packets\n");
		if(TCP_DEBUG) {
			fprintf(stderr, "[TCP SOCKET]:[ERROR]: %s\n", strerror(errno));
		}

		ret_val = -1;
	}
	else {
		sender.resetAddress();
		socklen_t remoteHostLen = sizeof(sender.remoteHost);
		ret_val = recvfrom(sockfd, buffer, length, 0, (struct sockaddr*) &sender.remoteHost, &remoteHostLen);

		if(TCP_DEBUG) {
			printf("[TCP SOCKET]:[DEBUG]: recvfrom() returned %d\n", ret_val);
		}
	}

	return ret_val;
}


void TCPSocket::receiveFromPort(){
	char buffer[MAXLINE];
	Endpoint sender;
	int n = receiveFrom(sender, buffer, MAXLINE);

	if (n > 0) {
		buffer[n] = '\0';

		// Add message to ring-buffer
		messages.push(Message(sender, buffer, n));
	}
	else if (n < 0) {
		if(TCP_DEBUG){
			fprintf(stderr, "[TCP SOCKET]:[ERROR]: Receiving data on port failed\n");
		}
	}
}

void TCPSocket::receiveFromPortThread() {
	// Continually calls receiveFrom placing the returned messages on the message
	// queue
	while (true) {
		this->receiveFromPort();
	}
}

void TCPSocket::receiveFromPortThreadStoppable(std::atomic<bool>& run) {
	// Continually calls receiveFrom placing the returned messages on the message
	// queue
	while (run) {
		this->receiveFromPort();
	}
}


bool TCPSocket::getMessage(Message &message) { return messages.pop(message); }

bool TCPSocket::areThereMessages(){
  Message temp;
  return messages.peek(temp);
} 

int TCPSocket::getSockfd() const{
  return sockfd;
}
