#include <avr/io.h>
#include <util/delay.h>

// Define pin bitmasks on Port D (Arduino Uno Digital Pins 2, 3, 4)
#define RED_LED     (1 << PORTD2)  // Digital Pin 2
#define YELLOW_LED  (1 << PORTD3)  // Digital Pin 3
#define GREEN_LED   (1 << PORTD4)  // Digital Pin 4

int main(void) {
    // Set Pins 2, 3, and 4 as OUTPUTS
    DDRD |= (RED_LED | YELLOW_LED | GREEN_LED);

    while (1) {
        // 1. RED Light ON (Stop) - 5 seconds
        PORTD = RED_LED;
        _delay_ms(5000);

        // 2. GREEN Light ON (Go) - 5 seconds
        PORTD = GREEN_LED;
        _delay_ms(5000);

        // 3. YELLOW Light ON (Caution) - 2 seconds
        PORTD = YELLOW_LED;
        _delay_ms(2000);
    }

    return 0;
}
