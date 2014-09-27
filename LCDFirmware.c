/*
* LCDFirmware.c
*
* Created: 16.08.2014 16:27:42
* Author: Mikhail Doshevsky <m.doshevsky@gmail.com>
*/

#define F_CPU 12000000
#define BAUD_RATE ((unsigned int)6)
#define byte unsigned char

#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "Commands.h"
#include "ATMega2560.h"

typedef enum { CHIP_1, CHIP_2 } Chips;
typedef enum { DATA, COMMAND } DataTypes;

inline void Clear(void);
inline void PutPixel(byte x, byte y);
inline void AddLine(byte x1, byte y1, byte x2, byte y2);
inline void AddCircle(byte x, byte y, byte r);
inline void Draw(void);

void InitializePins(void);
void InitializeLCD(void);
inline void SetPage(byte page, Chips chip);
inline void SetAddress(byte address, Chips chip);
inline void SendData(byte data, Chips chip, DataTypes dataType);

byte Data[8][128];

int main(void)
{
    InitializePins();
    InitializeLCD();
    InitializeSerialPort();
    
    AddLine(0, 0, 127, 63);
    AddLine(127, 0, 0, 63);
    Draw();
    
    sei();
    
    while (1)
    {
    }
}

inline void Clear(void)
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 128; ++j)
        {
            Data[i][j] = 0;
        }
    }
}

inline void PutPixel(byte x, byte y)
{
    if (x > 127 || y > 63)
    {
        return;
    }
    
    Data[y / 8][x] |= 1 << (y % 8);
}

inline void AddLine(byte x1, byte y1, byte x2, byte y2)
{
    byte lengthX = fabs(x1 - x2);
    byte lengthY = fabs(y1 - y2);

    byte length = lengthX > lengthY ? lengthX : lengthY;
    if (length == 0)
    {
        PutPixel(x1, y1);
        return;
    }

    float dX = (float)(x2 - x1) / length;
    float dY = (float)(y2 - y1) / length;

    float x = x1;
    float y = y1;

    ++length;
    while (--length)
    {
        x += dX;
        y += dY;
        PutPixel(round(x), round(y));
    }
}

inline void AddCircle(byte x, byte y, byte r)
{
}

inline void Draw(void)
{
    for (int i = 0; i < 8; ++i)
    {
        SetPage(i, CHIP_1);
        SetAddress(0, CHIP_1);
        for (int j = 0; j < 64; ++j)
        {
            SendData(Data[i][j], CHIP_1, DATA);
        }
    }
    
    for (int i = 0; i < 8; ++i)
    {
        SetPage(i, CHIP_2);
        SetAddress(0, CHIP_2);
        for (int j = 64; j < 128; ++j)
        {
            SendData(Data[i][j], CHIP_2, DATA);
        }
    }
}

void InitializePins(void)
{
    DATA_DDR = 255;
    E1_DDR |= (1 << E1_PIN);
    E2_DDR |= (1 << E2_PIN);
    RESET_DDR |= (1 << RESET_DDR);
    RW_DDR |= (1 << RW_PIN);
    A0_DDR |= (1 << A0_PIN);
    E_DDR |= (1 << E_PIN);
}

void InitializeLCD(void)
{
    RESET_PORT &= ~(1 << RESET_PIN);
    _delay_ms(2);
    RESET_PORT |= (1 << RESET_PIN);
    _delay_ms(2);
    
    SendData(0b00111111, CHIP_1, COMMAND);
    SetPage(0, CHIP_1);
    SetAddress(0, CHIP_1);
    
    SendData(0b00111111, CHIP_2, COMMAND);
    SetPage(0, CHIP_2);
    SetAddress(0, CHIP_2);
}

inline void SetPage(byte page, Chips chip)
{
    byte data = 0b10111000 | (0b00000111 & page);
    SendData(data, chip, COMMAND);
}

inline void SetAddress(byte address, Chips chip)
{
    byte data = 0b01000000 | (0b00111111 & address);
    SendData(data, chip, COMMAND);
}

inline void SendData(byte data, Chips chip, DataTypes dataType)
{
    if (chip == CHIP_1)
    {
        E1_PORT |= (1 << E1_PIN);
        E2_PORT &= ~(1 << E2_PIN);
    }
    else
    {
        E1_PORT &= ~(1 << E1_PIN);
        E2_PORT |= (1 << E2_PIN);
    }
    
    if (dataType == COMMAND)
    {
        A0_PORT &= ~(1 << A0_PIN);
    }
    else
    {
        A0_PORT |= (1 << A0_PIN);
    }
    
    RW_PORT &= ~(1 << RW_PIN);
    
    DATA_PORT = data;
    
    E_PORT &= ~(1 << E_PIN);
    _delay_us(10);
    E_PORT |= (1 << E_PIN);
    _delay_us(10);
    E_PORT &= ~(1 << E_PIN);
}

ISR (SERIAL_INTERRUPT)
{
    byte val = RecieveSerialByte();
    
    switch (val)
    {
        case DRAW_COMMAND:
        {
            Draw();
            break;
        }
        
        case CLEAR_COMMAND:
        {
            Clear();
            break;
        }
        
        case PUT_PIXEL_COMMAND:
        {
            byte x = RecieveSerialByte();
            byte y = RecieveSerialByte();
            PutPixel(x, y);
            break;
        }
        
        case ADD_LINE_COMMAND:
        {
            byte x1 = RecieveSerialByte();
            byte y1 = RecieveSerialByte();
            byte x2 = RecieveSerialByte();
            byte y2 = RecieveSerialByte();
            AddLine(x1, y1, x2, y2);
            break;
        }
    }
}
