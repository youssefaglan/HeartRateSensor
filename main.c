/*
 * File:   main.c
 * Author: obadw
 *
 * Created on May 15, 2024, 12:29 PM
 */

// CONFIG
#pragma config FOSC = XT
#pragma config WDTE = ON        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "I2C_LCD.h"
#include "heartbeat.h"

#define _XTAL_FREQ     4000000

#define VCFG0 ADCON1bits.VCFG0
#define VCFG1 ADCON1bits.VCFG1

void ADC_Init() {
    // Configure ADC control registers
    // VDD and VSS reference voltage configure other analog channels as digital I/O
    ADCON1 = 0x80; 

    // Configure ADC conversion clock source
    // Fosc/32
    ADCON0bits.ADCS = 0b10;

    // Configure ADC result format
    // Left justified
    ADCON1bits.ADFM = 0;

    // Turn on ADC module
    ADCON0bits.ADON = 1;
}


int ADC_Read(unsigned char channel) {
    // Ensure channel selection is within valid range
    if (channel > 7) {
        return 0;
    }

    // Configure ADC channel selection
    ADCON0 = (channel << 2) | 0x01; // Select channel and turn ADC module on

    // Wait for acquisition time
    __delay_us(20); // Assuming acquisition time is 20 microseconds for a 4MHz clock

    // Start conversion
    GO_DONE = 1;
    
    // Wait for conversion to complete
    while (GO_DONE);

    // Return ADC result
    return ((ADRESH << 8) + ADRESL); // Combine high and low ADC result registers
}

void main() 
{
    char heart1[15]; // Increased buffer size for the formatted string
    int heart;
    TRISB =0x00;
    TRISA =0xff;
    
    I2C_Master_Init();
    LCD_Init(0x4E);
    ADC_Init();
    
    while(1) 
    {
        heart = ADC_Read(0);
        
        //conversion ADC value to heart rate in BPM
        int bpm = heartrate(heart);

        // display
        
        if (bpm >= 55 && bpm <= 70)
        {
            __delay_ms(500);
            sprintf(heart1, "HR: %d BPM", bpm);
            LCD_Set_Cursor(1, 1);
            LCD_Write_String(heart1);
            LCD_Set_Cursor(2, 1);
            LCD_Write_String("Average HR");
        }
        else if ((bpm > 70 && bpm <= 90) || (bpm < 55 && bpm >= 50) )
        {
            
            __delay_ms(500);
            sprintf(heart1, "HR: %d BPM", bpm);
            LCD_Set_Cursor(1, 1);
            LCD_Write_String(heart1);
            LCD_Set_Cursor(2, 1);
            LCD_Write_String("Abnormal HR");
        }
        else if (bpm >= 90)
        {
            __delay_ms(500);
            LCD_Set_Cursor(2, 1);
            LCD_Write_String("Warning! High HR");
        }
        else if (bpm >= 0 && bpm < 50)
        {
            __delay_ms(500);
            LCD_Set_Cursor(1, 1);
            LCD_Write_String("HR Not Detected!");
            
            LCD_Set_Cursor(2, 1);
            LCD_Write_String("Inconclusive HR");
        }
        
        __delay_ms(2000);
    }
}