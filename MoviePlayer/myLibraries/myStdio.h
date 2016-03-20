/*
 * myStdio.h
 *
 * Created: 2016-03-02 9:47:59 PM
 *  Author: take-iwiw
 */ 


#ifndef MYSTDIO_H_
#define MYSTDIO_H_

/***
 * Const values
 ***/

/***
 * Enum definitions
 ***/

/***
 * Struct definitions
 ***/

/***
 * External APIs
 ***/
void stdioInit();
char putchar(char c);
char getchar();
char getcharTry(char *c);
void print(const char *str);
void printDec(const uint8_t dec);	// heavy function. only for debug
void printHex(const uint8_t hex);
void printDecPos(const uint8_t dec, const uint8_t x, const uint8_t y);
void printPos(const char *str, const uint8_t x, const uint8_t y);

#endif /* MYSTDIO_H_ */