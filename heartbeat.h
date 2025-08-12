/* 
 * File:   heartbeat.h
 * Author: obadw
 *
 * Created on May 14, 2024, 11:05 PM
 */

// heartbeat.h

#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <xc.h>
#include <stdint.h>

// Define pin connections for the heartbeat sensor
#define HEARTBEAT_SENSOR_PIN RC0

// Define calibration parameters
#define ADC_MIN_VALUE   0    // Minimum ADC value observed during calibration
#define ADC_MAX_VALUE   1023 // Maximum ADC value observed during calibration
#define HR_MIN          60   // Minimum heart rate corresponding to ADC_MIN_VALUE
#define HR_MAX          220  // Maximum heart rate corresponding to ADC_MAX_VALUE


int heartrate(int);
#endif /* HEARTBEAT_H */