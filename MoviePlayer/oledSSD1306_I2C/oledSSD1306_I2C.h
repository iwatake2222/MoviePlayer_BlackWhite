/*
 * oledSSD1306_I2C.h
 *
 * Created: 2016-03-19 7:29:18 PM
 *  Author: take-iwiw
 */ 


#ifndef OLEDSSD1306_I2C_H_
#define OLEDSSD1306_I2C_H_

void oledSSD1306_init();
void oledSSD1306_setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void oledSSD1306_setAddrWindowFull();
void oledSSD1306_fillRect(uint16_t color, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void oledSSD1306_start();
void oledSSD1306_stop();
void oledSSD1306_sendDataByte(uint8_t data);
void oledSSD1306_sendDataBurst(uint8_t *data, uint16_t num);

// for text output
void oledSSD1306_putchar(char c);
void oledSSD1306_setCurPos(uint8_t textX, uint8_t textY);
void oledSSD1306_saveCurPos();
void oledSSD1306_loadCurPos();

#endif /* OLEDSSD1306_I2C_H_ */