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
// https://www.beej.us/guide/bgnet/html/

#pragma once

#include "endpoint.h"
#include "tcp_socket.h"
#include "message.h"
#include "safe_circular_queue.h"
#include "socket_defines.h"
#include <iostream>
#include <utility>
#include <unordered_map>
#include <linux/wireless.h>
#include <cstring>
#include <errno.h>
#include <atomic>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;

/*!
 TCP Access Point Socket
*/
class TCPAPSocket : public TCPSocket {
public:
	/*!
	* @brief Instantiate an TCP Socket
	*/
	TCPAPSocket(int portId);

	~TCPAPSocket();

	/**
	* @brief Function used to listen to incoming calls to the access point
	*/
	void checkListeningPort();

	/*! Send a packet to a remote endpoint
	 *  @param remote The remote endpoint
	 *  @param packet The packet to be sent
	 *  @param length The length of the packet to be sent
	 *  @return the number of written bytes on success (>=0) or -1 on failure
	 */
	int typeSendTo(Endpoint &remote, const char *packet, int length) override;

	/*!
	* @brief Get the sockfd object
	*
	* @return int
	*/
	int getSockfd() const;

private:
};
