#include <iostream>
#include "line_of_sight.h"
#include <iomanip>
#include <map>
#include <vector>
//#pragma warning(disable : 4996)

using namespace std;

const string RED = "\033[1;31m";
const string GREEN = "\033[1;32m";
const string END = "\033[0m\n";
const double TOLERANCE = 0.0000001;

bool doubleEq(double a, double b){
	return abs(a - b) < TOLERANCE;
}

void test(bool condition, string desc){
	if (condition)
		cout << GREEN << "[PASS]: " << desc << END;
	else 
		cout << RED << "[FAIL]: " << desc << END;
}

void test(double desired, double actual, string desc){
	test(doubleEq(desired, actual), desc);
}



int main() {
	int id = 0;
	string file = "test_files/sc_orbits.txt";
	LineOfSight los(id, file);
	cout << los.fileName << endl;

	bool losYN = los.isThereLOS(1, 3);
	test(losYN, "Line of Sight is true when ships can see eachother");

	test(1.0, los.asteroidRadius, "Asteroid radius sets correctly");

	los.scalePositions(2);
	test(2, los.asteroidRadius, "Scaling positions up scales asteroid correctly");
	bool carrierPosScaledUpCorrectly = true;
	carrierPosScaledUpCorrectly &= doubleEq(los.orbitData[0][0].xpos, 200);
	carrierPosScaledUpCorrectly &= doubleEq(los.orbitData[0][0].ypos, 0);
	carrierPosScaledUpCorrectly &= doubleEq(los.orbitData[0][0].zpos, 0);
	test(carrierPosScaledUpCorrectly, "Scaling positions up scales ship locations correctly");

	los.scalePositions(0.5);
	test(1, los.asteroidRadius, "Scaling positions down scales asteroid correctly");
	bool carrierPosScaledDownCorrectly = true;
	carrierPosScaledDownCorrectly &= doubleEq(los.orbitData[0][0].xpos, 100);
	carrierPosScaledDownCorrectly &= doubleEq(los.orbitData[0][0].ypos, 0);
	carrierPosScaledDownCorrectly &= doubleEq(los.orbitData[0][0].zpos, 0);
	test(carrierPosScaledDownCorrectly, "Scaling positions down scales ship locations correctly");

	double relativePositionInfo[4];
	los.distanceBetweenSC(1, 3, relativePositionInfo);
	double distance = relativePositionInfo[3];
	test(75.28661080585771970, distance, "distance is correctly calculated between two drone");
	test(-74.71733893500000079, relativePositionInfo[0], "relative x position is correctly calculated between two drones");
	test(32.59819314500000331,  relativePositionInfo[1], "relative y position is correctly calculated between two drones");
	test(-29.97862988500000014,  relativePositionInfo[2], "relative z position is correctly calculated between two drones");
	
	return 0;
}