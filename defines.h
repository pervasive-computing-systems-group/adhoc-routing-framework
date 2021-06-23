#ifndef DEFINES_H
#define DEFINES_H

#define DEBUG 0
#define RPI 1

#include <stdint.h>

#define IP_ADDR			    uint32_t

#if RPI
#define BROADCAST_STR       "192.168.1.255"
#else
#define BROADCAST_STR       "255.255.255.255"
#endif

#define MY_IP_ADDR          "192.168.1.1"
#define WIRELESS_INTERFACE_NAME        "wlan0"
#define MONITORING_INTERFACE_NAME      "wlan1"

#define ROUTING_PORT        5969
#define DATA_PORT           5432
#define MONITOR_PORT        5123
#define LOCAL_DATA_UPDATE_RATE_MS 500
#define HELLO_PORT          5124

// Implemented routing protocols
#define USE_SINGLE_HOP		0
#define USE_AODV			1
// Single-hop node type
#define AP_NODE				0
#define STATION_NODE		1

/*
 * The following macros define the behavior of the routing protocol
 */
// The routing protocol to use in field test (select from above)
#define RT_PROTOCOL			USE_AODV
// This node's single-hop type
#define SH_NODE_TYPE		STATION_NODE
// Internal experiment settings
#define PACKET_BUFFER_CAPACITY	1048576 // 1 GiB
#define DATA_RATE				1000	// The data capture rate in ms
#define BUFF_CHECK_RATE			2000	// Buffer attempt-to-empty rate in ms
#define LOGGING_RATE			2000	// The data logging rate in ms
// List of devices to send data to
#define SENDTO_LIST				{ "192.168.1.1" }
// Logging locations
#define DATA_CAPRUTE_LOG		"test_logs/test1/aodv_1_data_captured.txt"

#endif
