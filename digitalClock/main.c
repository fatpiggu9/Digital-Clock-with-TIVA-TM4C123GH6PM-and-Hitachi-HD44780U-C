#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "systick.h"

volatile int blinkCounter = 0;  // Counter for blinking
volatile int minutes = 0;       // Variable to store minutes
volatile int seconds = 0;       // Variable to store seconds
volatile int hours = 0;         // Variable to store hours

// Writing command to the LCD
void lcd_write_cmd(uint8_t cmd) {
    // Set Register Select (RS) to 0 for cmd transfer
    GPIO_PORTD_DATA_R &= ~(0x04);
    _lcd_write(&GPIO_PORTC_DATA_R, cmd);
}

// Initiating the LCD
void LCD_init(void) {
    _quick_delay(20);  // Wait for power-up

    lcd_write_cmd(0x02); // 4-bit mode, 1-line, 5x8 font
    lcd_write_cmd(0x06); // Increment cursor position after write
    lcd_write_cmd(0x0C); // Display on, cursor off
    lcd_write_cmd(0x01); // Clear display

    _quick_delay(20);
}

// Add delay
void _quick_delay(int num) {
    volatile int i; // Forces the compiler to implement the loop.
    for(i=0;i<num;i++);
}

// Enable data transfer
void _lcd_pulse_enable_flag(int count) {
    GPIO_PORTD_DATA_R |= 0x08;  // Generate a High-to-low pulse on EN pin
    // TODO: delay milliseconds here
    _quick_delay(count);
    GPIO_PORTD_DATA_R &= ~(0x08);
}

// Send a byte of data into the LCD in two 4-bit nibbles, high and low
void _lcd_write(volatile uint32_t *address, uint8_t data){
    // Send the higher nibble
    *address = (data & 0xF0);
    // Falling edge triggers the LCD controller, to read the nibble.
    _lcd_pulse_enable_flag(1000);
    _quick_delay(5000);

    // Send the lower nibble
    *address = ((data << 4) & 0xF0);
    // Falling edge triggers the LCD controller, to read the nibble.
    _lcd_pulse_enable_flag(1000);
    _quick_delay(5000);
}

// Writing data to the LCD
void lcd_write_data(uint8_t data) {
    // Set Register Select (RS) to 1 for data transfer
    GPIO_PORTD_DATA_R |= 0x04;
    _lcd_write(&GPIO_PORTC_DATA_R, data);
}

int main(void) {
    int dummy;

    // Enable the GPIO ports used for the LCD (Ports C and D)
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOC | SYSCTL_RCGC2_GPIOD;

    // Initiate a dummy read to insert a few cycles after enabling the peripheral
    dummy = SYSCTL_RCGC2_R;

    // Set the direction of the GPIO pins for the LCD data lines (PC4-PC7, PD0, PD1)
    GPIO_PORTC_DIR_R |= 0xF0; // Set PC4-PC7 as outputs
    GPIO_PORTD_DIR_R |= 0x0F; // Set PD0 and PD3 as outputs

    // Enable the GPIO pins for digital function (PC4-PC7, PD0-PD3)
    GPIO_PORTC_DEN_R |= 0xF0; // Enable digital functionality for PC4-PC7
    GPIO_PORTD_DEN_R |= 0x0F; // Enable digital functionality for PD0-PD3

    LCD_init(); // Initialize LCD

    // Loop forever.
    while (1) {

        // Display corresponding hours
        lcd_write_data('0' + (hours / 10));
        lcd_write_data('0' + (hours % 10));

        // Display separator between hours and minutes
        lcd_write_data(':');

        // Display corresponding minutes
        lcd_write_data('0' + (minutes / 10));
        lcd_write_data('0' + (minutes % 10));

        // Display blinking separator between minutes and seconds
        lcd_write_data((blinkCounter % 2 == 0) ? ' ' : ':');

        // Display corresponding seconds
        lcd_write_data('0' + (seconds / 10));
        lcd_write_data('0' + (seconds % 10));

        // Clear any remaining bits in the line
        lcd_write_data(' ');
        lcd_write_data(' ');
        lcd_write_data(' ');
        lcd_write_data(' ');
        lcd_write_data(' ');
        lcd_write_data(' ');
        lcd_write_data(' ');
        lcd_write_data(' ');

        // Increment second every 10 blinks
        if (blinkCounter % 10 == 0){
            seconds++;

            // Reset second's index and increment minute's index
            if(seconds == 60){
                seconds = 0;
                minutes++;

                // Reset minute's index and increment hour's index
                if(minutes == 60){
                    minutes = 0;
                    hours++;

                    // Reset hour's index
                    if(hours == 24){
                        hours = 0;
                    }
                }
            }
        }

        blinkCounter++;
    }

    return 0;
}

