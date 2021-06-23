#ifndef _LINE_OF_SIGHT_H
#define _LINE_OF_SIGHT_H

#include "los_defines.h"
#include <string>
#include <map>
#include <vector>
#include <stdio.h>
#include <cmath>

using namespace std;

struct Pos3D
{
	double xpos;
	double ypos;
	double zpos;
};

class LineOfSight
{
public:
	// Declare variables
	map<int, vector<Pos3D>> orbitData;			// store orbit xyz positions for each s/c
	int homeID;									// this is the ID of the s/c that the program is running on
	string fileName;							// should just be "sc_orbits.txt", but this is here for flexibility
	int numSC;									// total # of s/c (not including carrier). This data comes from orbit info file
	int timestepSize;							// size of timestep. e.g. deltaT = 1 sec. This data comes from orbit info file
	int timestepNum;							// number of data points that each represent a point in time. This data comes from orbit info file.
	double asteroidRadius = 16.84;				// asteroid radius [km] (https://ssd.jpl.nasa.gov/sbdb.cgi?sstr=2000433)

	// Declare Constructor
	LineOfSight(string orbitFileName);
	LineOfSight(int ID, string orbitFileName);			// declare constructor. define it in defineLOS.cpp

	// Declare functions
	void setSCID(int id);
	void pullOrbitData();									// open the file with orbit info
	/*
	 * Determines if this S/C has Line-Of-Sight with the given S/C, otherScID, at lapsed
	 *  time, timeLapsed, from start of orbits (in seconds).
	 *
	 * Returns true if there is LOS, false otherwise.
	 */
	bool isThereLOS(int otherScID, double timeLapsedS);
	// scales all of the positions by scaleFactor input argument. Use for test bed adaptation
	void scalePositions(float scaleFactor);
	// returns the distance [km] between s/c 1 (homeID) and s/c 2 (otherScID) at a particular time. Note this is not a position vector
	void distanceBetweenSC(int otherScID, double currentTime, double relativePositionInfo[]);
};

#endif
