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
		printf("[TCP SOCKET]:[DEBUG]: Sending %s to %s via UDP\n", packet, remote.getAddressC());
	}
	if (sockfd < 0) {
		fprintf(stderr, "[TCP SOCKET]:[ERROR]: sockfd is in error state\n");
		return -1;
	}

	// TODO: Open socket
	// TODO: Send packet

	int returnVal = 0; //sendto(sockfd, packet, length, MSG_CONFIRM, (const struct sockaddr *)&remote.remoteHost, sizeof(remote.remoteHost));

	if(returnVal < 0){
		fprintf(stderr, "[TCP SOCKET]:[ERROR] Could not send packet %s to %s\n", packet, remote.getAddressC());
		fprintf(stderr, "[TCP SOCKET]:[ERROR]: %s\n", strerror(errno));
	}
	return returnVal;
}

int TCPStationSocket::getSockfd() const{
  return sockfd;
}
