/*
 * oledSSD1306_I2CConfig.h
 *
 * Created: 2016-03-19 7:29:28 PM
 *  Author: take-iwiw
 */ 


#ifndef OLEDSSD1306_I2CCONFIG_H_
#define OLEDSSD1306_I2CCONFIG_H_

#define SLAVE_ADDR 0x3C
//#define SLAVE_ADDR 0x3D

#define I2C_SPEED_KHZ 400

#define OLED_SSD1306_WIDTH  128
#define OLED_SSD1306_HEIGHT  64
#define OLED_SSD1306_PAGE_SIZE 8
#define OLED_SSD1306_PAGE    OLED_SSD1306_HEIGHT/OLED_SSD1306_PAGE_SIZE


/* text settings */
#define TEXT_AREA_ORIGIN_X 0
//#define TEXT_AREA_ORIGIN_Y 128
#define TEXT_AREA_ORIGIN_Y 0
// actual end position is the below number -1
#define TEXT_AREA_END_X 128
#define TEXT_AREA_END_Y 64
#define FONT_WIDTH 5
#define FONT_HEIGHT 8
#define FONT_DISPLAY_SIZE 2	// must be 2^x


#endif /* OLEDSSD1306_I2CCONFIG_H_ */