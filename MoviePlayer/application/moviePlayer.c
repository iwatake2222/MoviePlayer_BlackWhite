/*
 * moviePlayer.c
 *
 * Created: 2016-03-05 10:22:32 AM
 *  Author: take-iwiw
 */ 
#include <avr/io.h>
#include "../myCommon.h"
#include <util/delay.h>
#include "../myLibraries/myRetarget.h"
#include "../myLibraries/myStdio.h"
#include "../myLibraries/myTimer.h"
#include "../myLibraries/myVideo.h"
#include "../color/colorRGB.h"
#include "../fatFS/ff.h"

/*** Internal Const Values ***/
#define WIDTH  128
#define HEIGHT 64
#define TARGET_FPS 30
#define TARGET_FPS_TIME_MS (1000/TARGET_FPS)
#define SIZE_VIDEO_BUFFER 128
/*** Internal Static Variables ***/
static FATFS FatFs;
static FIL Fil;
static uint8_t s_videoBuffer[SIZE_VIDEO_BUFFER];

/*** Internal Function Declarations ***/
static void drawFile(char* filename);
static void drawFileLineBuff(char* filename);
static void drawFileLineBuff_BlackWhite(char* filename);
static uint8_t getNextFile(DIR *dir, char *filename);
static uint8_t getNextDir(DIR *dir, char *dirname);

/*** External Function Defines ***/
void moviePlayerInit()
{
	uint8_t ret = f_mount(0, "", 0);
	if(ret != 0) printDec(ret);
	ret = f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */		
	if(ret != 0) printDec(ret);
}

/* one file(picture) every one loop */
/* e.g. /dirA/fileX, fileY,  /dirB/fileZ */
void moviePlayerLoop()
{
	/* fps control */
	static int8_t accumulatedFrameDelay;
	uint8_t isSkip = 0;
	int8_t currentDelay = TARGET_FPS_TIME_MS - getIntervalTime() - accumulatedFrameDelay;
	if(currentDelay > 0) {
		// need to wait
		while(currentDelay--) _delay_ms(1);
	} else if(currentDelay < 0) {
		// need to skip
		accumulatedFrameDelay = -currentDelay;
		isSkip = 1;;
	}
	

		
	static DIR dirRoot;
	static char dirname[14];
	static DIR dir;	
	
	uint8_t ret;
	if(dirRoot.fs == 0) {
		// 1, move to /
		ret = f_opendir(&dirRoot, "/");
		if(ret != 0) printDec(ret);
	}
	if(dirname[0] == 0) {
		// 2. move to dirA,  6 move to dirB
		if (getNextDir(&dirRoot, dirname) > 0) {
			//printPos("          ", 0, 0);
			//printPos(dirname, 0, 0);
			f_chdir(dirname);				
			f_opendir(&dir, "");
		} else {
			f_closedir(&dirRoot);
			dirRoot.fs = 0;
		}
	}		
	char filename[14];
	// 3. process fileX,  4. process fileY,  7. process fileZ
	if(getNextFile(&dir, filename) > 0) {
		if(isSkip)return;
		printPos("          ", 0, 0);
		printPos(filename, 0, 0);
#ifdef USE_OLED_SSD1306_VIDEO
		drawFileLineBuff_BlackWhite(filename);
#endif
#ifdef USE_LCD_ST7735R_VIDEO
		drawFileLineBuff(filename);
		//drawFile(filename);
#endif
		//getchar();
	} else {
		// 5. close dirA and move to /,  8. close dirB and move to /
		f_closedir(&dir);
		f_chdir("/");
		dirname[0] = 0;
	}
}

static void drawFile(char* filename)
{
	uint8_t ret = f_open(&Fil, filename, FA_READ ) ;
	if(ret != FR_OK) {
		printDec(ret);
		return;
	}
	setVideoArea(0, 0, WIDTH, HEIGHT);
	startFrame();
	suspendDraw();
	for(uint16_t x = 0; x < WIDTH*HEIGHT; x++){
		uint8_t buffer[2];
		UINT bw;
		f_read(&Fil, buffer, 2, &bw);
		if(bw != 2)break;
		resumeDraw();
		drawPixel(buffer[0], buffer[1]);
		suspendDraw();
	}
	f_close(&Fil);
	uint8_t time = stopFrame();
	printDecPos(1000/time, 0, 1);
}

static void drawFileLineBuff(char* filename)
{
	uint8_t ret = f_open(&Fil, filename, FA_READ ) ;
	if(ret != FR_OK) {
		printDec(ret);
		return;
	}
	setVideoArea(0, 0, WIDTH, HEIGHT);
	startFrame();
	suspendDraw();
	const uint16_t bulkSize = SIZE_VIDEO_BUFFER;		// <= VIDEO_BUFFER_SIZE
	for(uint16_t x = 0; x < WIDTH*HEIGHT*2/bulkSize; x++){
		UINT bw;
		f_read(&Fil, s_videoBuffer, bulkSize, &bw);
		if(bw != bulkSize)break;
		resumeDraw();
		drawBuffer(s_videoBuffer, bulkSize);
		suspendDraw();
	}
	f_close(&Fil);
	uint8_t time = stopFrame();
	printDecPos(1000/time, 0, 1);
	//getchar();
}

static void drawFileLineBuff_BlackWhite(char* filename)
{
	uint8_t ret = f_open(&Fil, filename, FA_READ ) ;
	if(ret != FR_OK) {
		printDec(ret);
		return;
	}	
	setVideoArea(0, 0, WIDTH, HEIGHT);
	startFrame();
	suspendDraw();

	const uint16_t bulkSize = SIZE_VIDEO_BUFFER/2;		// <= VIDEO_BUFFER_SIZE
	for(uint16_t x = 0; x < WIDTH*HEIGHT/8/bulkSize; x++){
		UINT bw;
		f_read(&Fil, s_videoBuffer, bulkSize, &bw);
		//printHex(buffer[0]);
		if(bw != bulkSize)break;
		resumeDraw();
		drawBuffer(s_videoBuffer, bulkSize);
		suspendDraw();
		//getchar();		
	}
	f_close(&Fil);
	uint8_t time = stopFrame();
	//printDecPos(1000/time, 0, 1);
	//printHex(1000/time);
	//getchar();
}



static uint8_t getNextFile(DIR *dir, char *filename)
{
	while(1){
		FILINFO fno;
		uint8_t res = f_readdir(dir, &fno);
		if (res != FR_OK || fno.fname[0] == 0) return 0;
		if (fno.fname[0] == '.') continue;

		if (fno.fattrib & AM_DIR) {
			continue;
		} else {
			memcpy(filename, fno.fname, 14);
			return 1;
		}
	}
}

static uint8_t getNextDir(DIR *dir, char *dirname)
{
	while(1){
		FILINFO fno;
		uint8_t res = f_readdir(dir, &fno);
		if (res != FR_OK || fno.fname[0] == 0) return 0;
		if (fno.fname[0] == '.') continue;

		if ( (fno.fattrib & AM_DIR) && ( (fno.fattrib & AM_SYS) != AM_SYS) ) {
			memcpy(dirname, fno.fname, 14);
			return 1;
		} else {
			continue;
		}
	}
}
