#include <avr/io.h>

int main(void) {
    // 1. Set Pin 13 (PB5) as OUTPUT for the LED
    DDRB |= (1 << DDB5);

    // 2. Set Pin 2 (PD2) as INPUT for the Push Button
    DDRD &= ~(1 << DDD2);

    // 3. Enable internal pull-up resistor on Pin 2 (PD2)
    // Pin reads HIGH (1) when unpressed, and LOW (0) when pressed
    PORTD |= (1 << PORTD2);

    while (1) {
        // Read Pin 2 state from the PIND input register
        if (!(PIND & (1 << PIND2))) {
            PORTB |= (1 << PORTB5);   // Button pressed (LOW) -> Turn LED ON
        } else {
            PORTB &= ~(1 << PORTB5);  // Button released (HIGH) -> Turn LED OFF
        }
    }

    return 0;
}
