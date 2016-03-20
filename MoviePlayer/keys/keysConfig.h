/*
 * keysConfig.h
 *
 * Created: 2016-03-09 10:36:36 PM
 *  Author: take-iwiw
 */ 


#ifndef KEYSCONFIG_H_
#define KEYSCONFIG_H_

/* io config */
#define KEYS_MAX 2
#define KEYS_0_DDR DDRC
#define KEYS_1_DDR DDRC
//#define KEYS_2_DDR 
//#define KEYS_3_DDR 
//#define KEYS_4_DDR 
//#define KEYS_5_DDR 
//#define KEYS_6_DDR 
//#define KEYS_7_DDR 
#define KEYS_0_PORT PORTC
#define KEYS_1_PORT PORTC
//#define KEYS_2_PORT 
//#define KEYS_3_PORT 
//#define KEYS_4_PORT 
//#define KEYS_5_PORT 
//#define KEYS_6_PORT 
//#define KEYS_7_PORT 
#define KEYS_0_PIN PINC
#define KEYS_1_PIN PINC
//#define KEYS_2_PIN
//#define KEYS_3_PIN
//#define KEYS_4_PIN
//#define KEYS_5_PIN
//#define KEYS_6_PIN
//#define KEYS_7_PIN
#define KEYS_0_BIT 2
#define KEYS_1_BIT 3
//#define KEYS_2_BIT
//#define KEYS_3_BIT
//#define KEYS_4_BIT
//#define KEYS_5_BIT
//#define KEYS_6_BIT
//#define KEYS_7_BIT



/* operation config */
#define KEYS_USE_PULLUP
#define KEYS_CHECK_INTERVAL_MS 20


#endif /* KEYSCONFIG_H_ */