/*
 * ATMega32a.h
 *
 * Created: 10.09.2014 23:56:22
 * Author: Mikhail Doshevsky <m.doshevsky@gmail.com>
 */ 


#ifndef ATMEGA32A_H_
#define ATMEGA32A_H_

void InitializeSerialPort(void);
inline byte RecieveSerialByte(void);

#define SERIAL_INTERRUPT USART_RX_vect

// GPIO 40-33
#define DATA_DDR DDRA
#define DATA_PORT PORTA

// GPIO 29
#define E1_DDR DDRC
#define E1_PORT PORTC
#define E1_PIN PINC7

// GPIO 28
#define E2_DDR DDRC
#define E2_PORT PORTC
#define E2_PIN PINC6

// GPIO 27
#define RESET_DDR DDRC
#define RESET_PORT PORTC
#define RESET_PIN PINC5

// GPIO 26
#define RW_DDR DDRC
#define RW_PORT PORTC
#define RW_PIN PINC4

// GPIO 25
#define A0_DDR DDRC
#define A0_PORT PORTC
#define A0_PIN PINC3

// GPIO 24
#define E_DDR DDRC
#define E_PORT PORTC
#define E_PIN PINC2

void InitializeSerialPort(void)
{
    UBRRH = (byte)(BAUD_RATE >> 8);
    UBRRL = (byte)BAUD_RATE;
    UCSRB = (1 << RXEN) | (1 << TXEN);
}

inline byte RecieveSerialByte(void)
{
    while (!(UCSRA & (1 << RXC)))
    {
    }

    return UDR;
}

#endif /* ATMEGA32A_H_ */