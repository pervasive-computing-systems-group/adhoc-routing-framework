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
	double asteroidRadius = 1;					// asteroid radius [km] using spherical approximation (mean radius obtained from wikipedia)

	// Declare Constructor
	LineOfSight(int ID, string orbitFileName);			// declare constructor. define it in defineLOS.cpp

	// Declare functions
	void pullOrbitData();									// open the file with orbit info
	bool isThereLOS(int other_sc_ID, double current_time);	// returns true if there is no Line of Sight (LOS) and false if not
	void scalePositions(float scaleFactor);					// scales all of the positions by scaleFactor input argument. Use for testbed adaptation
	void distanceBetweenSC(int other_sc_ID, double current_time, double relativePositionInfo[]);  // returns the distance [km] between s/c 1 (homeID) and s/c 2 (other_sc_ID) at a particular time. Note this is not a position vector
};

#endif