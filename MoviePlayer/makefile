TARGET = hello.out
MCU = atmega328p
F_CPU = 16000000UL
DIR_BUILD = build
MAKE = make
CC = avr-gcc
CXX = avr-g++
AR = avr-ar
ASM = avr-as
OBJCOPY = avr-objcopy
CFLAGS = -g -O2 -mmcu=$(MCU) -DF_CPU=$(F_CPU)
LDFLAGS =  -g -O2 -Wl,-Map,hello.map -mmcu=$(MCU) 
INCLUDES = 
LIBS = 
#SRCS = main.c sub.c
#OBJS = $(SRCS:.c=.o)
OBJS = main.o\
	accelADXL345/accelADXL345.o\
	adc/adc.o\
	application/deviceTrial.o application/fpsCheck.o application/moviePlayer.o application/tinyDisplay.o\
	charLcd/charLcd.o\
	fatFS/ff.o fatFS/sdmmc_spi.o\
	font/font.o\
	i2c/i2c.o\
	keys/keys.o\
	lcdST7735R_SPI/lcdST7735R_SPI.o lcdST7735R_SPI/lcdST7735R_SPIText.o\
	myLibraries/myInput.o myLibraries/myStdio.o myLibraries/myTimer.o myLibraries/myVideo.o\
	oledSSD1306_I2C/oledSSD1306_I2C.o oledSSD1306_I2C/oledSSD1306_I2CText.o\
	rotaryEncoder/rotaryEncoder.o\
	spi/spi.o\
	system/taskManager.o\
	timer0System/timer0System.o\
	uart0/uart0.o

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -std=c99 -c $< -o $(DIR_BUILD)/$(notdir $(basename $<)).o
.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $(DIR_BUILD)/$(notdir $(basename $<)).o

.PHONY: all
all: $(DIR_BUILD) $(TARGET)

$(DIR_BUILD):
	mkdir -p $(DIR_BUILD)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(addprefix $(DIR_BUILD)/, $(notdir $(OBJS))) $(LIBS)

.PHONY: clean
clean:
	rm -f *.o *.a *.exe *.out
	@for subdir in $(SUBDIRS) ; do \
		(cd $$subdir && $(MAKE) clean) ;\
	done
	rm -rf $(DIR_BUILD)

