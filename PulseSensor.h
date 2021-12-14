/*
   PulseSensor measurement manager.
   See https://www.pulsesensor.com to get started.

   Copyright World Famous Electronics LLC - see LICENSE
   Contributors:
     Joel Murphy, https://pulsesensor.com
     Yury Gitman, https://pulsesensor.com
     Bradford Needham, @bneedhamia, https://bluepapertech.com

   Licensed under the MIT License, a copy of which
   should have been included with this software.

   This software is not intended for medical use.
*/

// ADAPTED FROM: https://github.com/WorldFamousElectronics/PulseSensorPlayground/blob/master/src/utility/PulseSensor.h

#ifndef PULSE_SENSOR_H
#define PULSE_SENSOR_H

class PulseSensor {
  public:
    // Constructs a PulseSensor manager using a default configuration.
    PulseSensor();

	// sets variables to default start values
	void resetVariables();
	
	// sets the time between readings of the pulse sensor (used for calculations only, does not affect the actual sampling interval)
	void setSampleIntervalMs(long interval);

    // Returns the latest beats-per-minute measurement on this PulseSensor.
    int getBeatsPerMinute();

    // Reads and clears the 'saw start of beat' flag, "QS".
    bool sawStartOfBeat();

    // Process the latest sample and return the current BPM.
    int processLatestSample(float sample);


  private:
    // Pulse detection output variables.
    // Volatile because our pulse detection code could be called from an Interrupt
    volatile int BPM;                // int that holds raw Analog in 0. updated every call to readSensor()
    volatile float Signal;             // holds the latest incoming raw data (from 0 to 1)
    volatile int IBI;                // int that holds the time interval (ms) between beats! Must be seeded!
    volatile bool Pulse;          // "True" when User's live heartbeat is detected. "False" when not a "live beat".
    volatile bool QS;             // The start of beat has been detected and not read by the Sketch.
    volatile float threshSetting;      // used to seed and reset the thresh variable
    volatile float amp;                         // used to hold amplitude of pulse waveform, seeded (sample value)
    volatile unsigned long lastBeatTime;      // used to find IBI. Time (sampleCounter) of the previous detected beat start.

    // Variables internal to the pulse detection algorithm.
    // Not volatile because we use them only internally to the pulse detection.
    unsigned long sampleIntervalMs;  // expected time between calls to readSensor(), in milliseconds.
    int rate[10];                    // array to hold last ten IBI values (ms)
    unsigned long sampleCounter;     // used to determine pulse timing. Milliseconds since we started.
    float P;                           // used to find peak in pulse wave, seeded (sample value)
    float T;                           // used to find trough in pulse wave, seeded (sample value)
    float thresh;                      // used to find instant moment of heart beat, seeded (sample value)
    bool firstBeat;               // used to seed rate array so we startup with reasonable BPM
    bool secondBeat;              // used to seed rate array so we startup with reasonable BPM
};
#endif // PULSE_SENSOR_H