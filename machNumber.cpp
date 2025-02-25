#include<iostream>
#include<cmath>
#include <string>
#include<vector>
#include<fstream> // For file operations
#include<cstdlib> // For system() to call GNUPLOT

using namespace std;

// Based on Atmosphere Temperature Distribuction

struct AtmosphereParams {
    double mTroposphere = -1 / 6.5;
    double mStratosphere1 = 1;
    double mStratosphere2 = 1 / 2.8;
    double nTroposphere = 44.3;
    double nStratosphere1 = -196.65;
    double nStratosphere2 = -49.66;
    double tropopauseTemperature = 216.65;
};

vector<double> linspace(double start, double end, int num) {
	
	vector<double> linearSpace; // Face coordinates vector
	
	double step = (end - start)/(num - 1);

	for (int i = 0; i < num; ++i) {
		linearSpace.push_back(start + i*step);
	}

	return linearSpace;
}

vector<double> calculateSoundSpeed(double gammaAir, double R, const vector<double>& temperatureVectors) {
	
	vector<double> soundSpeedVector;
	double soundSpeed;

	for (int i = 0; i < temperatureVectors.size(); ++i) {
		soundSpeed = sqrt(gammaAir*R*temperatureVectors[i]);
		soundSpeedVector.push_back(soundSpeed);
	}

	return soundSpeedVector;
}

vector<double> calculateMachNumber(int velocity, const vector<double>& temperatureVectors, const vector<double>& soundSpeedVector) {
	
	double machNumber;
	vector<double> machVector;

	for (int i = 0; i < temperatureVectors.size(); ++i) {
		machNumber = velocity/soundSpeedVector[i];
		machVector.push_back(machNumber);
	}

	return machVector;
}

int main() {

	const double gammaAir = 1.4;
	const double R = 287.085;
	int velocity = 315;
	int h = 0;

	AtmosphereParams params;

	vector<double> temperatureVectors;
	double temperatureTroposphere;
	double temperatureStratosphereFirstLayer;
	double temperatureStratosphereSecondLayer;
	double tropopauseTemperature = 216.65;

	do {
		if (h >= 0 && h <= 11) {
			temperatureTroposphere = (h-params.nTroposphere)/params.mTroposphere;
			temperatureVectors.push_back(temperatureTroposphere);
			++h;
		}

		else if (h >= 12 && h <= 20) {
			temperatureVectors.push_back(tropopauseTemperature);
			++h;
		}

		else if (h >= 21 && h <= 32) {
			temperatureStratosphereFirstLayer = (h-params.nStratosphere1)/params.mStratosphere1;
			temperatureVectors.push_back(temperatureStratosphereFirstLayer);
			++h;
		}

		else {
			temperatureStratosphereSecondLayer = (h-params.nStratosphere2)/params.mStratosphere2;
			temperatureVectors.push_back(temperatureStratosphereSecondLayer);
			++h;
		}

	} while (h <= 41);

	vector<double> altitudeVector = linspace(0.0, 41.0, 42);  // Corrected the number of steps
	vector<double> soundVector = calculateSoundSpeed(gammaAir, R, temperatureVectors);
	vector<double> machNumberVector = calculateMachNumber(velocity, temperatureVectors, soundVector);

	// Save data to a file (data.txt)
    ofstream dataFile("data.txt");
	
	if (!dataFile) {
	    cerr << "Error opening file for writing..." << endl;
	    return -1;
	}

    for (int i = 0; i < machNumberVector.size(); ++i) {
        dataFile << altitudeVector[i] << " " << machNumberVector[i] << endl;
    }
    dataFile.close();

    // Plotting using Gnuplot
	system("gnuplot -e \"set terminal png; set output 'plot.png'; set grid; plot 'data.txt' using 2:1 with lines lc rgb 'black' lw 2 title 'Mach Number vs Altitude'\"");

	return 0;
}
