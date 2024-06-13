// matrixkeyboard.h


#ifndef MATRIXKEYBOARD_H_
#define MATRIXKEYBOARD_H_

// Define a 4x4 Matrix Keyboard
#define NUMBER_OF_ELEMENTS 4

extern unsigned short int R_POSITIONS[NUMBER_OF_ELEMENTS];
extern unsigned short int C_POSITIONS[NUMBER_OF_ELEMENTS];
extern char keys[NUMBER_OF_ELEMENTS][NUMBER_OF_ELEMENTS];

char USER_MATRIX_KEYBOARD_Read( void );

#endif
