#include <reg51.h>  // Standard 8051 register definitions

void delay_ms(unsigned int ms);

void main(void) {
    unsigned char pattern;

    while (1) {
        pattern = 0x01; // Start with LED 1 ON (Binary: 0000 0001 -> P1.0)

        for (unsigned char i = 0; i < 4; i++) {
            P1 = pattern;       // Send pattern to Port 1 (P1.0 to P1.3)
            delay_ms(300);      // Wait 300 ms
            pattern = pattern << 1; // Shift bit left to light the next LED
        }
    }
}

// Delay function calibrated for an 11.0592 MHz crystal oscillator
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 112; j++);
    }
}

