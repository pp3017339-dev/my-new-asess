#include <avr/io.h>
#include <util/delay.h>

// LCD Control Pins on Port B (Arduino Uno Pins 8 & 9)
#define RS (1 << PORTB0)  // Digital Pin 8
#define EN (1 << PORTB1)  // Digital Pin 9

// Push Button Pin on Port D (Arduino Uno Pin 2)
#define BTN_PIN (1 << PIND2)

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
    lcd_send_nibble(cmd);       // Upper nibble
    lcd_send_nibble(cmd << 4);  // Lower nibble
}

void lcd_data(uint8_t data) {
    PORTB |= RS;                // RS = 1 for Data
    lcd_send_nibble(data);       // Upper nibble
    lcd_send_nibble(data << 4); // Lower nibble
}

void lcd_print(const char *str) {
    while (*str) {              // Loop through string using a pointer
        lcd_data(*str++);
    }
}

void lcd_init(void) {
    DDRB |= (RS | EN);          // Set RS & EN (Pins 8, 9) as OUTPUT
    DDRD |= 0xF0;               // Set D4–D7 (Pins 4–7) as OUTPUT
    _delay_ms(20);

    lcd_command(0x02);          // 4-bit initialization
    lcd_command(0x28);          // 2 lines, 5x7 font
    lcd_command(0x0C);          // Display ON, Cursor OFF
    lcd_command(0x06);          // Auto-increment cursor
    lcd_command(0x01);          // Clear display
    _delay_ms(2);
}

int main(void) {
    lcd_init();

    // Configure Digital Pin 2 (PD2) as INPUT with internal pull-up resistor
    DDRD &= ~(1 << DDD2);
    PORTD |= (1 << PORTD2);

    while (1) {
        lcd_command(0x80); // Move cursor to Row 1, Column 1

        if (!(PIND & BTN_PIN)) {
            lcd_print("Switch Pressed  "); // Trailing spaces overwrite old text
        } else {
            lcd_print("Switch Released ");
        }

        _delay_ms(100); // Small delay for stability and simple debouncing
    }

    return 0;
}

