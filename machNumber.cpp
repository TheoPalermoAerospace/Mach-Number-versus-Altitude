#include<iostream>
#include<cmath>
#include <string>
#include<vector>
#include<fstream> // For file operations
#include<cstdlib> // For system() to call GNUPLOT

using namespace std;

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

	double gammaAir = 1.4;
	double R = 287.085;
	int velocity = 315;
	int h = 0;

	vector<double> temperatureVectors;
	double temperatureTroposphere;
	double temperatureStratosphereFirstLayer;
	double temperatureStratosphereSecondLayer;
	double tropopauseTemperature = 216.65;

	// Base on Atmosphere Temperature Distribuction

	double m_troposphere = (-1)/6.5; // -11/(71.5)
	double m_stratosphere_1 = 1; // (32-20)/(228.65-216.65) = 1
	double m_stratosphere_2 = 1/2.8; // (47-32)/(270.65-228.65) = 15/42

	double n_troposphere = 44.3;
	double n_stratosphere_1 = -196.65;
	double n_stratosphere_2 = -49.66;

	do {
		if (h >= 0 && h <= 11) {
			temperatureTroposphere = (h-n_troposphere)/m_troposphere;
			temperatureVectors.push_back(temperatureTroposphere);
			++h;
		}

		else if (h >= 12 && h <= 20) {
			temperatureVectors.push_back(tropopauseTemperature);
			++h;
		}

		else if (h >= 21 && h <= 32) {
			temperatureStratosphereFirstLayer = (h-n_stratosphere_1)/m_stratosphere_1;
			temperatureVectors.push_back(temperatureStratosphereFirstLayer);
			++h;
		}

		else {
			temperatureStratosphereSecondLayer = (h-n_stratosphere_2)/m_stratosphere_2;
			temperatureVectors.push_back(temperatureStratosphereSecondLayer);
			++h;
		}

	} while (h <= 41);

	vector<double> altitudeVector = linspace(0.0, 41.0, 42);  // Corrected the number of steps
	vector<double> soundVector = calculateSoundSpeed(gammaAir, R, temperatureVectors);
	vector<double> machNumberVector = calculateMachNumber(velocity, temperatureVectors, soundVector);

	// Save data to a file (data.txt)
    ofstream dataFile("data.txt");
    for (int i = 0; i < machNumberVector.size(); ++i) {
        dataFile << altitudeVector[i] << " " << machNumberVector[i] << endl;
    }
    dataFile.close();

    // Plotting using Gnuplot
	system("gnuplot -e \"set terminal png; set output 'plot.png'; set grid; plot 'data.txt' using 2:1 with lines lc rgb 'black' lw 2 title 'Mach Number vs Altitude'\"");

	/*for (int i = 0; i < machNumberVector.size(); ++i) {
		cout << "Mach Number: " << machNumberVector[i] << endl;
	}*/

	return 0;
}