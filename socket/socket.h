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

#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/wireless.h>
#include <cstring>
#include <errno.h>
#include <utility>

#include "endpoint.h"
#include "socket_defines.h"
#include "AppPacketHandler.h"
#include "message.h"
#include "../data_structures/safe_circular_queue.h"

using namespace std;
// TimeInterval class prototype
class TimeInterval;

class Socket {
public:
  // Constructor is deprecated!! At a fundamental level, a socket has an IP address and a port number,
  // we should be adding the port number is a pre-req to instantiating a socket.
  Socket();
  Socket(uint32_t nPortNum, uint32_t nMsgBffrSize);

  virtual ~Socket();

  /*! Set socket options
   *  @param level stack level
   *  @param optname option ID
   *  @param optval option value
   *  @param socklen_t length of the option value
   *  @return true on success, false on failure
   */
  bool setOption(int level, int optname, const void *optval, socklen_t optlen);

  /*! Get socket options
   *  @param level stack level
   *  @param optname option ID
   *  @param optval buffer pointer where to write the option value
   *  @param socklen_t length of the option value
   *  @return true on success, false on failure
   */
  bool getOption(int level, int optname, void *optval, socklen_t *optlen);

  /*! Close the socket
   */
  void sclose();

  /*!
   * @brief Get the Transmission Power of the interface
   * 
   * @return transmission power in dBm or -1 on error
   */
  int getTransmissionPower();

  /*!
   * @brief Set the Transmission Power of the interface
   * 
   * @param txPwr
   * @return true if set correctly
   * @return false if unable to set or set correctly
   */
  bool setTransmissionPower(int txPwr);

  /*! Send a packet to a remote endpoint
   *  @param remote The remote endpoint
   *  @param packet The packet to be sent
   *  @param length The length of the packet to be sent
   *  @return the number of written bytes on success (>=0) or -1 on failure
   */
  int sendTo(Endpoint &remote, const char *packet, int length);

  /*!
   * @brief Send a packet to an IP address and port
   *
   * @param buffer
   * @param length
   * @param dest
   * @return int
   */
  int sendTo(char* buffer, int length, uint32_t dest, int port);

  /*!
   * @brief Run the application packet received handler (if one was given to this socket)
   */
  void runAPHReceive(Message* pMsg);

  /*!
   * @brief Run the application packet send handler (if one was given to this socket)
   */
  void runAPHSend(int nBytesSent, char* pMsg);

  /*!
   * @brief Set an application specific packet handler for this socket. DO
   * NOT pass in dynamically allocated memory! There is nothing that frees
   * old AppPacketHanlders.
   */
  void setAppPacketHandler(AppPacketHandler *pAppPacketHandler);

  /*!
   * @brief Get one message from the socket
   *
   * @param message will get set to the first message on the queue
   * @return true a message was received
   * @return false no messages
   */
  bool getMessage(Message &message);

	/*!
	* @brief Check if there are any messages without attempting to get the packet
	*
	*/
	bool areThereMessages();

	  /*!
	   * @brief Get the sockfd object
	   *
	   * @return int
	   */
	  int getSockfd() const;

protected:
  bool initSocket(int type);
  /*! Send a packet to a remote endpoint for a specific socket type
   *  @param remote The remote endpoint
   *  @param packet The packet to be sent
   *  @param length The length of the packet to be sent
   *  @return the number of written bytes on success (>=0) or -1 on failure
   */
  virtual int typeSendTo(Endpoint &remote, const char *packet, int length) = 0;

  // To hold threaded messages
  SafeCircularQueue<Message> messages;
  int sockfd;
  // Port number for this socket
  uint32_t m_nPortNum;

private:
  AppPacketHandler *m_pAppPacketHandler;
};
