#include <avr/io.h>
#include <util/delay.h>

// Control Pins on Port B (Arduino Uno Pins 8 & 9)
#define RS (1 << PORTB0)  // Digital Pin 8
#define EN (1 << PORTB1)  // Digital Pin 9

// Data Pins D4–D7 on Port D (Arduino Uno Pins 4, 5, 6, 7) -> Mask 0xF0

void lcd_pulse_enable(void) {
    PORTB |= EN;
    _delay_us(1);
    PORTB &= ~EN;
    _delay_ms(2);
}

void lcd_send_nibble(uint8_t nibble) {
    PORTD = (PORTD & 0x0F) | (nibble & 0xF0);
    lcd_pulse_enable();
}

void lcd_command(uint8_t cmd) {
    PORTB &= ~RS;               // RS = 0 for Command
    lcd_send_nibble(cmd);       // Send upper nibble
    lcd_send_nibble(cmd << 4);  // Send lower nibble
}

void lcd_data(uint8_t data) {
    PORTB |= RS;                // RS = 1 for Data
    lcd_send_nibble(data);      // Send upper nibble
    lcd_send_nibble(data << 4); // Send lower nibble
}

void lcd_init(void) {
    DDRB |= (RS | EN);  // Set RS and EN as outputs
    DDRD |= 0xF0;       // Set PD4–PD7 as outputs
    _delay_ms(20);      // LCD power-on delay

    lcd_command(0x02);  // Initialize 4-bit mode
    lcd_command(0x28);  // 4-bit, 2 lines, 5x7 matrix
    lcd_command(0x0C);  // Display ON, Cursor OFF
    lcd_command(0x06);  // Increment cursor
    lcd_command(0x01);  // Clear display
    _delay_ms(2);
}

int main(void) {
    lcd_init();

    while (1) {
        // Countdown from 9 down to 0 using a signed integer (int8_t)
        for (int8_t num = 9; num >= 0; num--) {
            lcd_command(0x80);        // Set cursor to Row 1, Column 1
            lcd_data(num + '0');      // Convert digit to ASCII character
            _delay_ms(1000);          // 1-second delay per count
        }
    }

    return 0;
}
