/*
 * fatFSConfig.h
 *
 * Created: 2016-03-05 10:34:22 AM
 *  Author: take-iwiw
 */ 


#ifndef FATFSCONFIG_H_
#define FATFSCONFIG_H_

/* communication settings */
//#define FATFS_MCC_USE_GPIO
#define FATFS_MCC_USE_SIO


/* io settings */
#define FATFS_MMC_CS_PORT PORTB
#define FATFS_MMC_CS_DDR DDRB
#define FATFS_MMC_CS_BIT 0

#ifdef FATFS_MCC_USE_GPIO
#define FATFS_MMC_CK_PORT PORTC
#define FATFS_MMC_MOSI_PORT PORTC
#define FATFS_MMC_MISO_PORT PORTC
#define FATFS_MMC_CK_DDR DDRC
#define FATFS_MMC_MOSI_DDR DDRC
#define FATFS_MMC_MISO_DDR DDRC
#define FATFS_MMC_CK_BIT 2
#define FATFS_MMC_MOSI_BIT 1
#define FATFS_MMC_MISO_BIT 0
#define FATFS_MCC_GPIO_CK_WAIT 2
#endif


#endif /* FATFSCONFIG_H_ */