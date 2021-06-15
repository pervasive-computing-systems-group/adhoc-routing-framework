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

#include "udp_socket.h"
using std::memset;

UDPSocket::UDPSocket() : Socket(0, UDP_QUEUE_SIZE) {

}

UDPSocket::~UDPSocket(){ 
  if(UDP_DEBUG){
    printf("[UDP SOCKET]:[DEBUG]: deconstructing udp socket\n");
  }
  close(sockfd); 
}

bool UDPSocket::init(void) { 
  if(UDP_DEBUG){
    printf("[UDP SOCKET]:[DEBUG]: Initializing udp socket\n");
  }
  bool result = initSocket(SOCK_DGRAM);
  return result;
}

// Server initialization
bool UDPSocket::bindToPort(int port) {
  if(UDP_DEBUG){
    printf("[UDP SOCKET]:[DEBUG]: Binding udp socket to port %d\n", port);
  }
  if(sockfd < 0){
    if (!initSocket(SOCK_DGRAM))
      return false;
  }

  struct sockaddr_in localHost;
  std::memset(&localHost, 0, sizeof(localHost));

  localHost.sin_family = AF_INET;
  localHost.sin_port = htons(port);
  localHost.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (const struct sockaddr *)&localHost, sizeof(localHost)) < 0) {
    close(sockfd);
    if(UDP_DEBUG){
      printf("[UDP SOCKET]:[DEBUG]: Unsuccessful binding of udp socket to port %d\n", port);
    }
    return false;
  }
  if(UDP_DEBUG){
    printf("[UDP SOCKET]:[DEBUG]: Successful binding of udp socket to port %d\n", port);
  }

  return true;
}

bool UDPSocket::joinMulticastGroup(const char *address) {
  struct ip_mreq mreq;

  // Set up group address
  mreq.imr_multiaddr.s_addr = inet_addr(address);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);

  return setOption(IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
}

bool UDPSocket::setBroadcasting(bool broadcast) {
  int option = (broadcast) ? (1) : (0);
  if(UDP_DEBUG){
    printf("[UDP SOCKET]:[DEBUG]: Setting udp socket to broadcast mode\n");
  }
  return setOption(SOL_SOCKET, SO_BROADCAST, &option, sizeof(option));
}

// -1 if unsuccessful, else number of bytes written
int UDPSocket::typeSendTo(Endpoint &remote, const char *packet, int length) {
  if(UDP_DEBUG){
    printf("[UDP SOCKET]:[DEBUG]: Sending %s to %s via UDP\n", packet, remote.getAddressC());
  }
  if (sockfd < 0) {
    fprintf(stderr, "[UDP SOCKET]:[ERROR]: sockfd is in error state\n");
    if(UDP_DEBUG){
      fprintf(stderr, "[UDP SOCKET]:[ERROR]: %s\n", strerror(errno));
    }
    return -1;
  }

  int returnVal = sendto(sockfd, packet, length, MSG_CONFIRM,
                (const struct sockaddr *)&remote.remoteHost,
                sizeof(remote.remoteHost));
  if(returnVal < 0){
    fprintf(stderr, "[UDP SOCKET]:[ERROR] Could not send packet %s to %s\n", packet, remote.getAddressC());
    fprintf(stderr, "[UDP SOCKET]:[ERROR]: %s\n", strerror(errno));
  }
  return returnVal;
}

int UDPSocket::receiveFrom(Endpoint &sender, char *buffer, int length) {
  // -1 if unsuccessful, else number of bytes received
  if (sockfd < 0){
    fprintf(stderr, "[UDP SOCKET]:[ERROR]: UDP socketfd is in error state while trying to receive packets\n");
    if(UDP_DEBUG){
      fprintf(stderr, "[UDP SOCKET]:[ERROR]: %s\n", strerror(errno));
    }
    return -1;
  }
  
  sender.resetAddress();
  socklen_t remoteHostLen = sizeof(sender.remoteHost);
  int n = recvfrom(sockfd, buffer, length, 0,
                  (struct sockaddr *)&sender.remoteHost, &remoteHostLen);

  return n;
}


void UDPSocket::receiveFromPort(){
  char *buffer = (char *)malloc(MAXLINE * sizeof(char));
  Endpoint sender;
  int n = receiveFrom(sender, buffer, MAXLINE);
  if (n < 0) {
    fprintf(stderr, "[UDP SOCKET]:[ERROR]: Receiving data on port failed\n");
    if(UDP_DEBUG){
      fprintf(stderr, "[UDP SOCKET]:[ERROR]: %s\n", strerror(errno));
    }
    free(buffer);
    exit(-1);
  } else if (n > 0){
    // Check that the app is connected and continue
    printf(sender.getAddressC());
    if(this->isACHConnected(sender.getAddressI())){
      buffer[n] = '\0';
      messages.push(Message(sender, buffer, n));
    }
  }

  free(buffer);
}

void UDPSocket::receiveFromPortThread() {
  // Continually calls receiveFrom placing the returned messages on the message
  // queue
  while (true) {
    this->receiveFromPort();
  }
}

void UDPSocket::receiveFromPortThreadStoppable(std::atomic<bool>& run) {
  // Continually calls receiveFrom placing the returned messages on the message
  // queue
  while (run) {
    this->receiveFromPort();
  }
}
