#include "los_connection_handler.h"

LOSConnectionHandler::LOSConnectionHandler(uint32_t ip, string losFilename, string ipMapFilename)
	: m_ip(ip),
	  m_los(losFilename)
{
	_pullIpMap(ipMapFilename);
	m_startTime = chrono::duration_cast< std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

	if(LOS_CONNECTION_DEBUG) {
		cout << "[LOS CONNECTION HANDLER]:[DEBUG]: Read in ip map" << endl;
	}

	int id;
	if(m_mIpToId.count(ip)) {
		id = m_mIpToId[ip];
	}
	else {
		cerr << "[LOS CONNECTION HANDLER]:[ERROR]: Cannot find data for our own ip address " << ip << endl;
		exit(-1);
	}

	m_los.setSCID(id);
	if(LOS_CONNECTION_DEBUG) {
		cout << "[LOS CONNECTION HANDLER]:[DEBUG]: Read in flight path map" << endl;
	}
}

bool LOSConnectionHandler::isConnected(uint32_t ip) {
	// Get current time
	chrono::milliseconds currentTime
		= chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
	double elapsedTime = (currentTime - m_startTime).count() / 1000.0;

	// Get id
	int id;
	if(m_mIpToId.count(ip)) {
		id = m_mIpToId[ip];
	}
	else {
		cerr << "[LOS CONNECTION HANDLER]:[ERROR]: Cannot find data for ip address " << ip << endl;
		exit(-1);
	}

	bool connected = m_los.isThereLOS(id, elapsedTime);

	if(LOS_CONNECTION_DEBUG) {
		cout << "[LOS CONNECTION HANDLER]:[DEBUG]: There is "
				<< (connected ? "a" : "not a") << " connection with " << ip << " at time "
				<< currentTime.count()/1000.0 << endl;
	}

	return connected;
}

void LOSConnectionHandler::_pullIpMap(string ipMapFilename) {
	// open the file
	ifstream file;
	file.open(ipMapFilename);
	if(!file.is_open()) {
		cerr << "[LOS CONNECTION HANDLER]:[ERROR]: Unable to open file '" << ipMapFilename << "'" << endl;
		exit(-1);
	}

	// Get number of ips
	int numIps;
	file >> numIps;

	// read in ip and drone id from file
	for(int i = 0; i < numIps; i++) {
		string ip;
		int id;
		file >> ip >> id;
		IP_ADDR intIp = getIpFromString(ip);

		// Check if ip already in map
		if(m_mIpToId.count(intIp)) {
			// throw error
			cerr << "[LOS CONNECTION HANDLER]:[ERROR]: Invalid format, file contains multiple entries of the same ip address" << endl;
			exit(-1);
		}
		else {
			m_mIpToId[intIp] = id;
		}
	}

	file.close();
}
