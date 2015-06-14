#include "spi.h"
#include "gpio.h"
#include "ft_gpu.h"
#include "ft800.h"
																								// FT800 pins
unsigned int ft800irqPin;																		// Interrupt from FT800 to Arduino - not used here
unsigned int ft800pwrPin;																		// PD_N from Arduino to FT800 - effectively FT800 reset
unsigned int ft800csPin;																		// SPI chip select - defined separately since it's manipulated with GPIO calls
																								// LCD display parameters
unsigned int lcdWidth;																			// Active width of LCD display
unsigned int lcdHeight;																			// Active height of LCD display
unsigned int lcdHcycle;																			// Total number of clocks per line
unsigned int lcdHoffset;																		// Start of active line
unsigned int lcdHsync0;																			// Start of horizontal sync pulse
unsigned int lcdHsync1;																			// End of horizontal sync pulse
unsigned int lcdVcycle;																			// Total number of lines per screen
unsigned int lcdVoffset;																		// Start of active screen
unsigned int lcdVsync0;																			// Start of vertical sync pulse
unsigned int lcdVsync1;																			// End of vertical sync pulse
unsigned char lcdPclk;																			// Pixel Clock
unsigned char lcdSwizzle;																		// Define RGB output pins
unsigned char lcdPclkpol;																		// Define active edge of PCLK

unsigned long ramDisplayList = RAM_DL;															// Set beginning of display list memory
unsigned long ramCommandBuffer = RAM_CMD;														// Set beginning of graphics command memory

unsigned int point_size = 0x0100;																// Define a default dot size
unsigned long point_x = (96 * 16);																// Define a default point x-location (1/16 anti-aliased)
unsigned long point_y = (136 * 16);																// Define a default point y-location (1/16 anti-aliased)
unsigned long color;																			// Variable for chanign colors
unsigned char ft800Gpio;																		// Used for FT800 GPIO register

void ft800cmdWrite(unsigned char ftCommand)
{
	unsigned char cZero = 0x00;																	// Filler value for command
	HAL_GPIO_WritePin(GPIOB, FT800_CS_N, GPIO_PIN_RESET);										// Set chip select low
	HAL_SPI_Transmit(&hspi1, &ftCommand, 1, 0);													// Send command
	HAL_SPI_Transmit(&hspi1, &cZero, 1, 0);														// Send first filler byte
	HAL_SPI_Transmit(&hspi1, &cZero, 1, 0);														// Send second filler byte
	HAL_GPIO_WritePin(GPIOB, FT800_CS_N, GPIO_PIN_SET);											// Set chip select high
}

void ft800memWrite8(unsigned long ftAddress, unsigned char ftData8)
{
	unsigned char cTempAddr[3];																	// FT800 Memory Address

	cTempAddr[2] = (char) (ftAddress >> 16) | MEM_WRITE;										// Compose the command and address to send
	cTempAddr[1] = (char) (ftAddress >> 8);														// middle byte
	cTempAddr[0] = (char) (ftAddress);															// low byte

	HAL_GPIO_WritePin(GPIOB, FT800_CS_N, GPIO_PIN_RESET);										// Set chip select low

  for (int i = 2; i >= 0; i--)
	{
		HAL_SPI_Transmit(&hspi1, &cTempAddr[i], 1, 0); 											// Send Memory Write plus high address byte
	}

	for (int j = 0; j < sizeof(ftData8); j++)
	{
		HAL_SPI_Transmit(&hspi1, &ftData8, 1, 0);												// Send data byte
	}

	HAL_GPIO_WritePin(GPIOB, FT800_CS_N, GPIO_PIN_SET);											// Set chip select high
}

void ft800memWrite16(unsigned long ftAddress, unsigned int ftData16)
{
	unsigned char cTempAddr[3];																	// FT800 Memory Address
	unsigned char cTempData[2];																	// 16-bit data to write

	cTempAddr[2] = (char) (ftAddress >> 16) | MEM_WRITE;										// Compose the command and address to send
	cTempAddr[1] = (char) (ftAddress >> 8);														// middle byte
	cTempAddr[0] = (char) (ftAddress);															// low byte

	cTempData[1] = (char) (ftData16 >> 8);														// Compose data to be sent - high byte
	cTempData[0] = (char) (ftData16);															// low byte

	HAL_GPIO_WritePin(GPIOB, FT800_CS_N, GPIO_PIN_RESET);										// Set chip select low

  for (int i = 2; i >= 0; i--)
	{
		HAL_SPI_Transmit(&hspi1, &cTempAddr[i], 1, 0); 											// Send Memory Write plus high address byte
	}

	for (int j = 0; j < sizeof(cTempData); j++)													// Start with least significant byte
	{
		HAL_SPI_Transmit(&hspi1, &cTempData[j], 1, 0);											// Send data byte
	}

	HAL_GPIO_WritePin(GPIOB, FT800_CS_N, GPIO_PIN_SET);											// Set chip select high
}

void ft800memWrite32(unsigned long ftAddress, unsigned long ftData32)
{
	unsigned char cTempAddr[3];																	// FT800 Memory Address
	unsigned char cTempData[4];																	// 32-bit data to write

	cTempAddr[2] = (char) (ftAddress >> 16) | MEM_WRITE;										// Compose the command and address to send
	cTempAddr[1] = (char) (ftAddress >> 8);														// middle byte
	cTempAddr[0] = (char) (ftAddress);															// low byte

	cTempData[3] = (char) (ftData32 >> 24);														// Compose data to be sent - high byte
	cTempData[2] = (char) (ftData32 >> 16);
	cTempData[1] = (char) (ftData32 >> 8);
	cTempData[0] = (char) (ftData32);															// low byte

	HAL_GPIO_WritePin(GPIOB, FT800_CS_N, GPIO_PIN_RESET);										// Set chip select low

  for (int i = 2; i >= 0; i--)
	{
		HAL_SPI_Transmit(&hspi1, &cTempAddr[i], 1, 0); 											// Send Memory Write plus high address byte
	}

	for (int j = 0; j < sizeof(cTempData); j++)													// Start with least significant byte
	{
		HAL_SPI_Transmit(&hspi1, &cTempData[j], 1, 0);											// Send SPI byte
	}

	HAL_GPIO_WritePin(GPIOB, FT800_CS_N, GPIO_PIN_SET);											// Set chip select high
}

unsigned char ft800memRead8(unsigned long ftAddress)
{
  unsigned char ftData8 = ZERO;																	// Place-holder for 8-bits being read
	unsigned char cTempAddr[3];																	// FT800 Memory Address
	unsigned char cZeroFill = ZERO;																// Dummy byte

	cTempAddr[2] = (char) (ftAddress >> 16) | MEM_READ;											// Compose the command and address to send
	cTempAddr[1] = (char) (ftAddress >> 8);														// middle byte
	cTempAddr[0] = (char) (ftAddress);															// low byte

	HAL_GPIO_WritePin(GPIOB, FT800_CS_N, GPIO_PIN_RESET);										// Set chip select low

  for (int i = 2; i >= 0; i--)
	{
		HAL_SPI_Transmit(&hspi1, &cTempAddr[i], 1, 0); 											// Send Memory Write plus high address byte
	}

  HAL_SPI_Transmit(&hspi1, &cZeroFill, 1, 0);													// Send dummy byte

	for (int j = 0; j < sizeof(ftData8); j++)													// Start with least significant byte
	{
		HAL_SPI_Receive(&hspi1, &ftData8, 1, 0);												// Receive data byte
	}

	HAL_GPIO_WritePin(GPIOB, FT800_CS_N, GPIO_PIN_SET);											// Set chip select high

  return ftData8;																				// Return 8-bits
}

unsigned int ft800memRead16(unsigned long ftAddress)
{
  unsigned int ftData16;																		// 16-bits to return
	unsigned char cTempAddr[3];																	// FT800 Memory Address
	unsigned char cTempData[2];																	// Place-holder for 16-bits being read
	unsigned char cZeroFill;

	cTempAddr[2] = (char) (ftAddress >> 16) | MEM_READ;											// Compose the command and address to send
	cTempAddr[1] = (char) (ftAddress >> 8);														// middle byte
	cTempAddr[0] = (char) (ftAddress);															// low byte

	HAL_GPIO_WritePin(GPIOB, FT800_CS_N, GPIO_PIN_RESET);										// Set chip select low

  for (int i = 2; i >= 0; i--)
	{
		HAL_SPI_Transmit(&hspi1, &cTempAddr[i], 1, 0); 											// Send Memory Write plus high address byte
	}

  HAL_SPI_Transmit(&hspi1, &cZeroFill, 1, 0);													// Send dummy byte

	for (int j = 0; j < sizeof(cTempData); j++)													// Start with least significant byte
	{
		HAL_SPI_Receive(&hspi1, &cTempData[j], 1, 0);											// Receive data byte
	}

	HAL_GPIO_WritePin(GPIOB, FT800_CS_N, GPIO_PIN_SET);											// Set chip select high

	ftData16 = (cTempData[1]<< 8) | 															// Compose value to return - high byte
					   (cTempData[0]); 															// low byte

  return ftData16;																				// Return 16-bits
}

unsigned long ft800memRead32(unsigned long ftAddress)
{
  unsigned long ftData32;																		// 32-bits to return
	unsigned char cTempAddr[3];																	// FT800 Memory Address
	unsigned char cTempData[4];																	// Place holder for 32-bits being read
	unsigned char cZeroFill;																	// Dummy byte

	cTempAddr[2] = (char) (ftAddress >> 16) | MEM_READ;											// Compose the command and address to send
	cTempAddr[1] = (char) (ftAddress >> 8);														// middle byte
	cTempAddr[0] = (char) (ftAddress);															// low byte

	HAL_GPIO_WritePin(GPIOB, FT800_CS_N, GPIO_PIN_RESET);										// Set chip select low

  for (int i = 2; i >= 0; i--)
	{
		HAL_SPI_Transmit(&hspi1, &cTempAddr[i], 1, 0); 											// Send Memory Write plus high address byte
	}

  HAL_SPI_Transmit(&hspi1, &cZeroFill, 1, 0);													// Send dummy byte

	for (int j = 0; j < sizeof(cTempData); j++)													// Start with least significatn byte
	{
		HAL_SPI_Receive(&hspi1, &cTempData[j], 1, 0);											// Receive data byte
	}

	HAL_GPIO_WritePin(GPIOB, FT800_CS_N, GPIO_PIN_SET);											// Set chip select high

	ftData32 = (cTempData[3]<< 24) | 															// Compose value to return - high byte
						 (cTempData[2]<< 16) |
						 (cTempData[1]<< 8) |
						 (cTempData[0]); 														// Low byte

  return ftData32;																				// Return 32-bits
}

unsigned int incCMDOffset(unsigned int currentOffset, unsigned char commandSize)
{
    unsigned int newOffset;																		// Used to hold new offset
    newOffset = currentOffset + commandSize;													// Calculate new offset
    if(newOffset > 4095)																		// If new offset past boundary...
    {
        newOffset = (newOffset - 4096);															// ... roll over pointer
    }
    return newOffset;																			// Return new offset
}

void ft800_Init(void) {

#ifdef LCD_QVGA																					// QVGA display parameters
    lcdWidth   = 320;																			// Active width of LCD display
    lcdHeight  = 240;																			// Active height of LCD display
    lcdHcycle  = 408;																			// Total number of clocks per line
    lcdHoffset = 70;																			// Start of active line
    lcdHsync0  = 0;																				// Start of horizontal sync pulse
    lcdHsync1  = 10;																			// End of horizontal sync pulse
    lcdVcycle  = 263;																			// Total number of lines per screen
    lcdVoffset = 13;																			// Start of active screen
    lcdVsync0  = 0;																				// Start of vertical sync pulse
    lcdVsync1  = 2;																				// End of vertical sync pulse
    lcdPclk    = 8;																				// Pixel Clock
    lcdSwizzle = 2;																				// Define RGB output pins
    lcdPclkpol = 0;																				// Define active edge of PCLK
#endif

#ifdef LCD_WQVGA																				// WQVGA display parameters
	lcdWidth   = 480;																			// Active width of LCD display
	lcdHeight  = 272;																			// Active height of LCD display
	lcdHcycle  = 548;																			// Total number of clocks per line
	lcdHoffset = 43;																			// Start of active line
	lcdHsync0  = 0;																				// Start of horizontal sync pulse
	lcdHsync1  = 41;																			// End of horizontal sync pulse
	lcdVcycle  = 292;																			// Total number of lines per screen
	lcdVoffset = 12;																			// Start of active screen
	lcdVsync0  = 0;																				// Start of vertical sync pulse
	lcdVsync1  = 10;																			// End of vertical sync pulse
	lcdPclk    = 5;																				// Pixel Clock
	lcdSwizzle = 0;																				// Define RGB output pins
	lcdPclkpol = 1;																				// Define active edge of PCLK
#endif

	HAL_GPIO_WritePin(GPIOB, FT800_PD_N, GPIO_PIN_SET); 										// Initial state of PD_N - high
	HAL_GPIO_WritePin(GPIOB, FT800_CS_N, GPIO_PIN_SET);											// Initial state of SPI CS - high
	HAL_Delay(20);																				// Wait 20ms
	HAL_GPIO_WritePin(GPIOB, FT800_PD_N, GPIO_PIN_RESET); 										// Reset FT800
	HAL_Delay(20);																				// Wait 20ms
	HAL_GPIO_WritePin(GPIOB, FT800_PD_N, GPIO_PIN_SET); 										// FT800 is awake
	HAL_Delay(20);																				// Wait 20ms - required

	ft800cmdWrite(FT800_ACTIVE);																// Start FT800
	HAL_Delay(5);																				// Give some time to process
	ft800cmdWrite(FT800_CLKEXT);																// Set FT800 for external clock
	HAL_Delay(5);																				// Give some time to process
	ft800cmdWrite(FT800_CLK48M);																// Set FT800 for 48MHz PLL
	HAL_Delay(5);																				// Give some time to process
																								// Now FT800 can accept commands at up to 30MHz clock on SPI bus
																								// This application leaves the SPI bus at 10.5MHz
 																								// Getting stuck in the while loop here
	if (ft800memRead8(REG_ID) != 0x7C)															// Read ID register - is it 0x7C?
	{
	  while(1);																					// If we don't get 0x7C, the ineface isn't working - halt with infinite loop
	}
	ft800memWrite8(REG_PCLK, ZERO);																// Set PCLK to zero - don't clock the LCD until later
	ft800memWrite8(REG_PWM_DUTY, ZERO);															// Turn off backlight
																								// End of Wake-up FT800
																								//***************************************
																								// Initialize Display
	ft800memWrite16(REG_HSIZE,   lcdWidth);														// active display width
	ft800memWrite16(REG_HCYCLE,  lcdHcycle);													// total number of clocks per line, incl front/back porch
	ft800memWrite16(REG_HOFFSET, lcdHoffset);													// start of active line
	ft800memWrite16(REG_HSYNC0,  lcdHsync0);													// start of horizontal sync pulse
	ft800memWrite16(REG_HSYNC1,  lcdHsync1);													// end of horizontal sync pulse
	ft800memWrite16(REG_VSIZE,   lcdHeight);													// active display height
	ft800memWrite16(REG_VCYCLE,  lcdVcycle);													// total number of lines per screen, incl pre/post
	ft800memWrite16(REG_VOFFSET, lcdVoffset);													// start of active screen
	ft800memWrite16(REG_VSYNC0,  lcdVsync0);													// start of vertical sync pulse
	ft800memWrite16(REG_VSYNC1,  lcdVsync1);													// end of vertical sync pulse
	ft800memWrite8(REG_SWIZZLE,  lcdSwizzle);													// FT800 output to LCD - pin order
	ft800memWrite8(REG_PCLK_POL, lcdPclkpol);													// LCD data is clocked in on this PCLK edge
																								// Don't set PCLK yet - wait for just after the first display list
																								// End of Initialize Display
																								//***************************************
																								// Configure Touch and Audio - not used in this example, so disable both
	ft800memWrite8(REG_TOUCH_MODE, 3);															// Enable touch
	ft800memWrite8(REG_TOUCH_ADC_MODE, 1);
	ft800memWrite16(REG_TOUCH_CHARGE, 5000);
	ft800memWrite8(REG_TOUCH_SETTLE, 3);
	ft800memWrite8(REG_TOUCH_OVERSAMPLE, 7);
	ft800memWrite16(REG_TOUCH_RZTHRESH, 1500);													// Eliminate any false touches
	ft800memWrite8(REG_VOL_PB, ZERO);															// turn recorded audio volume down
	ft800memWrite8(REG_VOL_SOUND, ZERO);														// turn synthesizer volume down
	ft800memWrite16(REG_SOUND, 0x6000);															// set synthesizer to mute
																								// End of Configure Touch and Audio
																								//***************************************
																								// Write Initial Display List & Enable Display
	ramDisplayList = RAM_DL;																	// start of Display List
	ft800memWrite32(ramDisplayList, DL_CLEAR_RGB); 												// Clear Color RGB   00000010 RRRRRRRR GGGGGGGG BBBBBBBB  (R/G/B = Colour values) default zero / black
	ramDisplayList += 4;																		// point to next location
	ft800memWrite32(ramDisplayList, (DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));					// Clear 00100110 -------- -------- -----CST  (C/S/T define which parameters to clear)
	ramDisplayList += 4;																		// point to next location
	ft800memWrite32(ramDisplayList, DL_DISPLAY);												// DISPLAY command 00000000 00000000 00000000 00000000 (end of display list)

	ft800memWrite32(REG_DLSWAP, DLSWAP_FRAME);													// 00000000 00000000 00000000 000000SS  (SS bits define when render occurs)
																								// Nothing is being displayed yet... the pixel clock is still 0x00
	ramDisplayList = RAM_DL;																	// Reset Display List pointer for next list

	ft800Gpio = ft800memRead8(REG_GPIO);														// Read the FT800 GPIO register for a read/modify/write operation
	ft800Gpio = ft800Gpio | 0x80;																// set bit 7 of FT800 GPIO register (DISP) - others are inputs
	ft800memWrite8(REG_GPIO, ft800Gpio);														// Enable the DISP signal to the LCD panel
	ft800memWrite8(REG_PCLK, lcdPclk);															// Now start clocking data to the LCD panel
	for(int duty = 0; duty <= 128; duty++)
	{
	  ft800memWrite8(REG_PWM_DUTY, duty);														// Turn on backlight - ramp up slowly to full brighness
	  HAL_Delay(10);
	}
																								// End of Write Initial Display List & Enable Display
}
