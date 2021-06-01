/* Modified version of the following code, which was liscenced under MIT
 * source: https://github.com/ARMmbed/mbed-os
 */

/* Copyright (C) 2012 mbed.org, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "socket.h"
#include "socket_defines.h"
#include <iostream>
#include <utility>
#include <unordered_map>
#include <linux/wireless.h>
#include <cstring>
#include <errno.h>
#include <atomic>

using namespace std;

/*!
UDP Socket
*/
class UDPSocket : public Socket {

public:
  /*! 
   * @brief Instantiate an UDP Socket
   */
  UDPSocket();

  ~UDPSocket();

  /*! 
   *  @brief Init the UDP Client Socket without binding it to any specific por
   * 
   *  @return true on success, false on failure
   */
  bool init(void);

  /*! 
   * @brief Bind a UDP Server Socket to a specific port
   *  
   * @param port The port to listen for incoming connections on
   * @return true on success, false on failure
   */
  bool bindToPort(int port);

  /*! 
   * @brief Join the multicast group at the given address
   *  
   * @param address The address of the multicast group
   * @return true on success, false on failure
   */
  bool joinMulticastGroup(const char *address);

  /*! 
   * @brief Set the socket in broadcasting mode
   *  
   * @return true on success, false on failure
   */
  bool setBroadcasting(bool broadcast = true);

  /*! Send a packet to a remote endpoint
   *  @param remote The remote endpoint
   *  @param packet The packet to be sent
   *  @param length The length of the packet to be sent
   *  @return the number of written bytes on success (>=0) or -1 on failure
   */
  int typeSendTo(Endpoint &remote, const char *packet, int length) override;

  /**
   *  @brief Receive a packet from a remote endpoint
   *  @param remote The remote endpoint
   *  @param buffer The buffer for storing the incoming packet data. If a packet
   *                is too long to fit in the supplied buffer, excess bytes are discarded
   *  @param length The length of the buffer
   *  @return the number of received bytes on success (>=0) or -1 on failure
   */
  int receiveFrom(Endpoint &remote, char *buffer, int length);

  /**
   * @brief Receives a single message from the port placing messages onto the socket's 
   * message queue
   * 
   */
  void receiveFromPort();

  /**
   * @brief Continuously reads data from the port, placing messages onto the
   *  socket's message queue.
   */
  void receiveFromPortThread();

  /**
   * @brief Continuously reads data from the port, placing messages onto the
   *  socket's message queue.
   * @param run an atomic boolean to stop the thread loop
   */
  void receiveFromPortThreadStoppable(std::atomic<bool>& run);

private:
};

#endif
