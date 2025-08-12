/*
 * File:   heartbeat.c
 * Author: obadw
 *
 * Created on May 14, 2024, 11:07 PM
 */


#include <xc.h>
#include "heartbeat.h"
#include "I2C_LCD.h"

#define _XTAL_FREQ 20000000

// Function to calculate heart rate from ADC reading
int heartrate(int adcReading) {
    return adcReading * (HR_MAX - HR_MIN) / (ADC_MAX_VALUE - ADC_MIN_VALUE) + HR_MIN;
}