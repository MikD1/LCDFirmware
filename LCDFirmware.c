/*
* LCDFirmware.c
*
* Created: 16.08.2014 16:27:42
* Author: Mikhail Doshevsky <m.doshevsky@gmail.com>
*/

#define F_CPU 12000000
#define bool char
#define byte unsigned char

#include <avr/io.h>
#include <util/delay.h>
#include "ATMega2560_addresess.h"

typedef enum { CHIP_1, CHIP_2 } Chips;
typedef enum { DATA, COMMAND } DataTypes;

void Clear(void);
void PutPixel(byte x, byte y);
void AddLine(byte x1, byte y1, byte x2, byte y2);
void AddCircle(byte x, byte y, byte r);
void Draw(void);

void InitializePins(void);
void InitializeLCD(void);
void SetPage(byte page, Chips chip);
void SetAddress(byte address, Chips chip);
void SendData(byte data, Chips chip, DataTypes dataType);

byte Data[8][128];

int main(void)
{
    InitializePins();
    InitializeLCD();
    
    Clear();
    Draw();
    
    while (1)
    {
    }
}

void Clear(void)
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 128; ++j)
        {
            Data[i][j] = 0;
        }
    }
}

void PutPixel(byte x, byte y)
{
    if (x > 127 || y > 63)
    {
        return;
    }
    
    Data[y / 8][x] |= 1 << (y % 8);
}

void AddLine(byte x1, byte y1, byte x2, byte y2)
{
}

void AddCircle(byte x, byte y, byte r)
{
}

void Draw(void)
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

void SetPage(byte page, Chips chip)
{
    byte data = 0b10111000 | (0b00000111 & page);
    SendData(data, chip, COMMAND);
}

void SetAddress(byte address, Chips chip)
{
    byte data = 0b01000000 | (0b00111111 & address);
    SendData(data, chip, COMMAND);
}

void SendData(byte data, Chips chip, DataTypes dataType)
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