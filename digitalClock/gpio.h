#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

// Base addresses for GPIO ports
#define GPIO_PORTA_BASE         0x40004000
#define GPIO_PORTB_BASE         0x40005000
// ... (add more ports as needed)

// Offsets for GPIO registers
#define GPIO_O_DATA             0x000
#define GPIO_O_DIR              0x400
#define GPIO_O_AFSEL            0x420
#define GPIO_O_PUR              0x510
#define GPIO_O_PDR              0x514
#define GPIO_O_DEN              0x51C
#define GPIO_O_LOCK             0x520
#define GPIO_O_CR               0x524
#define GPIO_O_AMSEL            0x528
#define GPIO_O_PCTL             0x52C

// Bit masks for GPIO registers
#define GPIO_PIN_0              0x01
#define GPIO_PIN_1              0x02
#define GPIO_PIN_2              0x04
#define GPIO_PIN_3              0x08
#define GPIO_PIN_4              0x10
#define GPIO_PIN_5              0x20
#define GPIO_PIN_6              0x40
#define GPIO_PIN_7              0x80

// Function prototypes
void GPIOPinTypeGPIOOutput(uint32_t ui32Port, uint8_t ui8Pins);
void GPIOPinWrite(uint32_t ui32Port, uint8_t ui8Pins, uint8_t ui8Val);

// ... (add more functions as needed)

#endif /* GPIO_H_ */
