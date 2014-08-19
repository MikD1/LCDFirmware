/*
* ATMega2560_addresess.h
*
* Created: 16.08.2014 16:27:42
* Author: Mikhail Doshevsky <m.doshevsky@gmail.com>
*/

#ifndef ATMEGA2560_ADDRESESS_H_
#define ATMEGA2560_ADDRESESS_H_

#define DATA_DDR DDRF
#define DATA_PORT PORTF

#define E1_DDR DDRE
#define E1_PORT PORTE
#define E1_PIN PINE4

#define E2_DDR DDRE
#define E2_PORT PORTE
#define E2_PIN PINE5

#define RESET_DDR DDRE
#define RESET_PORT PORTE
#define RESET_PIN PINE5

#define RW_DDR DDRH
#define RW_PORT PORTH
#define RW_PIN PINH3

#define A0_DDR DDRH
#define A0_PORT PORTH
#define A0_PIN PINH5

#define E_DDR DDRH
#define E_PORT PORTH
#define E_PIN PINH6

#endif /* ATMEGA2560_ADDRESESS_H_ */
