/*
* Commands.h
*
* Created: 09.09.2014 23:42:14
* Author: Mikhail Doshevsky <m.doshevsky@gmail.com>
*/

#ifndef COMMANDS_H_
#define COMMANDS_H_

#define DRAW_COMMAND 0x1
// no arguments

#define CLEAR_COMMAND 0x2
// no arguments

#define PUT_PIXEL_COMMAND 0x3
// arguments:
// byte 1 - X [0:127]
// byte 2 - Y [0:63]

#define ADD_LINE_COMMAND 0x4
// arguments:
// byte 1 - X1 [0:127]
// byte 2 - Y1 [0:63]
// byte 3 - X2 [0:127]
// byte 4 - Y2 [0:63]

#endif /* COMMANDS_H_ */