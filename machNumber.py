#---------------------------------------------------------------------#
#-------------------- Mach Number versus Altitude --------------------#
#---------------------------------------------------------------------#

# Package imports
import math
import numpy as np
import matplotlib.pyplot as plt
import sympy as sp

# Parameters
v = 315 # aircraft speed in meters per seconds
R = 287.085 # gas constant for air in J/kg K
gamma_air = 1.4

# Troposphere and stratosphere
h = 0 # declaring altitude variable
T = 0 # declaring temperature variable
soundSpeedVector = [] # Sound speed according to the temperature
machVector = [] # Mach number according to the altitude
temperatureVectors = []
altitudeVector = np.arange(0, 42, 1) # from 0 to 41 km of altitude

"""
h = m*T + n
m = Δy/Δx = Δh/ΔT
Therefore, T = (h-n)/m
"""

# The following numbers were obtained by given data
m_troposphere = (-1)/6.5 # -11/(71.5)
m_stratosphere_1 = 1 # (32-20)/(228.65-216.65) = 1
m_stratosphere_2 = 1/2.8 # (47-32)/(270.65-228.65) = 15/42

n_troposphere = 44.3
n_stratosphere_1 = -196.65
n_stratosphere_2 = -49.66

# The altitude goes from 0 to 41 km. Therefore:
while h <= 41:
    if h >= 0 and h <= 11:
        T_troposfera = (h-n_troposphere)/m_troposphere
        T = T_troposfera
        temperatureVectors.append(T)
        h += 1
    elif h >= 12 and h <= 20:
        T = 216.65
        temperatureVectors.append(T)
        h += 1
    elif h >= 21 and h <= 32:
        T_estratosfera_1 = (h-n_stratosphere_1)/m_stratosphere_1
        T = T_estratosfera_1
        temperatureVectors.append(T)
        h += 1
    else:
        T_estratosfera_2 = (h-n_stratosphere_2)/m_stratosphere_2
        T = T_estratosfera_2
        temperatureVectors.append(T)
        h += 1

def calculateSoundSpeed(gamma_air, R, temperatureVectors):
    i = 0 # loop variable
    while i < len(temperatureVectors):
        soundSpeed = math.sqrt(gamma_air*R*temperatureVectors[i])
        soundSpeedVector.append(soundSpeed)
        i += 1
    return soundSpeedVector

def calculateMachNumber(v, soundSpeedVector):
    i = 0 # loop variable
    while i < len(temperatureVectors):
        machNumber = v/soundSpeedVector[i]
        machVector.append(machNumber)
        i += 1
    return machVector

calculateSoundSpeed(gamma_air, R, temperatureVectors)
calculateMachNumber(v, soundSpeedVector)

plt.plot(machVector, altitudeVector)
plt.grid()
plt.xlabel('Mach Number')
plt.ylabel('Altitude (km)')
plt.title('Mach versus Altitude')
plt.yticks(np.arange(0, 45, step = 5))
plt.show()