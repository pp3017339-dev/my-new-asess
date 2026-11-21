#include <avr/io.h>
#include <util/delay.h>

// LCD Control Pins on Port B (Arduino Uno Pins 8 & 9)
#define RS      (1 << PORTB0)  // Digital Pin 8
#define EN      (1 << PORTB1)  // Digital Pin 9

// LED Pin on Port B (Arduino Uno Pin 13)
#define LED_PIN (1 << PORTB5)  // Digital Pin 13

// Push Button Pin on Port D (Arduino Uno Pin 2)
#define BTN_PIN (1 << PIND2)   // Digital Pin 2

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
    lcd_send_nibble(data);      // Upper nibble
    lcd_send_nibble(data << 4); // Lower nibble
}

void lcd_print(const char *str) {
    while (*str) {
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
    uint8_t led_state = 0;      // 0 = OFF, 1 = ON

    lcd_init();

    // Configure LED Pin 13 (PB5) as OUTPUT and start OFF
    DDRB |= LED_PIN;
    PORTB &= ~LED_PIN;

    // Configure Push Button Pin 2 (PD2) as INPUT with internal pull-up
    DDRD &= ~(1 << DDD2);
    PORTD |= (1 << PORTD2);

    // Display initial status
    lcd_command(0x80);
    lcd_print("System Ready");
    lcd_command(0xC0);
    lcd_print("LED Status: OFF");

    while (1) {
        // Detect button press (LOW due to pull-up)
        if (!(PIND & BTN_PIN)) {
            _delay_ms(30); // Debounce delay

            if (!(PIND & BTN_PIN)) {
                led_state = !led_state; // Toggle state between 0 and 1

                lcd_command(0xC0);      // Move cursor to Row 2, Column 1
                if (led_state) {
                    PORTB |= LED_PIN;             // Turn LED ON
                    lcd_print("LED Status: ON "); // Space clears trailing 'F'
                } else {
                    PORTB &= ~LED_PIN;            // Turn LED OFF
                    lcd_print("LED Status: OFF");
                }

                // Wait for button release so one press = one toggle
                while (!(PIND & BTN_PIN));
                _delay_ms(30);
            }
        }
    }

    return 0;
}
   