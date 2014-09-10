/*
* ATMega2560.h
*
* Created: 16.08.2014 16:27:42
* Author: Mikhail Doshevsky <m.doshevsky@gmail.com>
*/

#ifndef ATMEGA2560_H_
#define ATMEGA2560_H_

void InitializeSerialPort(void);
inline byte RecieveSerialByte(void);

#define SERIAL_INTERRUPT USART0_RX_vect

// GPIO A0-A7
#define DATA_DDR DDRF
#define DATA_PORT PORTF

// GPIO 2
#define E1_DDR DDRE
#define E1_PORT PORTE
#define E1_PIN PINE4

// GPIO 3
#define E2_DDR DDRE
#define E2_PORT PORTE
#define E2_PIN PINE5

// GPIO 5
#define RESET_DDR DDRE
#define RESET_PORT PORTE
#define RESET_PIN PINE3

// GPIO 6
#define RW_DDR DDRH
#define RW_PORT PORTH
#define RW_PIN PINH3

// GPIO 8
#define A0_DDR DDRH
#define A0_PORT PORTH
#define A0_PIN PINH5

// GPIO 9
#define E_DDR DDRH
#define E_PORT PORTH
#define E_PIN PINH6

void InitializeSerialPort(void)
{
    UBRR0H = (byte)(BAUD_RATE >> 8);
    UBRR0L = (byte)BAUD_RATE;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
}

inline byte RecieveSerialByte(void)
{
    while (!(UCSR0A & (1 << RXC0)))
    {
    }

    return UDR0;
}

#endif /* ATMEGA2560_H_ */
