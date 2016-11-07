#ifndef Lcd_h
#define Lcd_h

//  *****************************************************************************
//          lcd.h
//
//       include file for Epson S1D15G00 LCD Controller
//
//
//    Author:  James P Lynch  August 30, 2007
//  *****************************************************************************

#define DISON     0xAF      // Display on
#define DISOFF    0xAE      // Display off
#define DISNOR    0xA6      // Normal display
#define DISINV    0xA7      // Inverse display
#define COMSCN    0xBB      // Common scan direction
#define DISCTL    0xCA      // Display control
#define SLPIN     0x95      // Sleep in
#define SLPOUT    0x94      // Sleep out
#define PASET     0x75      // Page address set
#define CASET     0x15      // Column address set
#define DATCTL    0xBC      // Data scan direction, etc.
#define RGBSET8   0xCE      // 256-color position set
#define RAMWR     0x5C      // Writing to memory
#define RAMRD     0x5D      // Reading from memory
#define PTLIN     0xA8      // Partial display in
#define PTLOUT    0xA9      // Partial display out
#define RMWIN     0xE0      // Read and modify write
#define RMWOUT    0xEE      // End
#define ASCSET    0xAA      // Area scroll set
#define SCSTART   0xAB      // Scroll start set
#define OSCON     0xD1      // Internal oscillation on
#define OSCOFF    0xD2      // Internal oscillation off
#define PWRCTR    0x20      // Power control
#define VOLCTR    0x81      // Electronic volume control
#define VOLUP     0xD6      // Increment electronic control by 1
#define VOLDOWN   0xD7      // Decrement electronic control by 1
#define TMPGRD    0x82      // Temperature gradient set
#define EPCTIN    0xCD      // Control EEPROM
#define EPCOUT    0xCC      // Cancel EEPROM control
#define EPMWR     0xFC      // Write into EEPROM
#define EPMRD     0xFD      // Read from EEPROM
#define EPSRRD1   0x7C      // Read register 1
#define EPSRRD2   0x7D      // Read register 2
#define NOP       0x25      // NOP instruction

#define BKLGHT_LCD_ON        1
#define BKLGHT_LCD_OFF       2


// backlight control
#define BKLGHT_LCD_ON        1
#define BKLGHT_LCD_OFF       2

// Booleans
#define NOFILL         0
#define FILL          1

// 12-bit color definitions
#define WHITE    0xFFF
#define BLACK    0x000
#define RED    0xF00
#define GREEN    0x0F0
#define BLUE    0x00F
#define CYAN    0x0FF
#define MAGENTA  0xF0F
#define YELLOW   0xFF0
#define BROWN    0xB22
#define ORANGE   0xFA0
#define  PINK   0xF6A

// Font sizes
#define SMALL    0
#define MEDIUM   1
#define  LARGE   2

// hardware definitions
#define SPI_SR_TXEMPTY
#define LCD_RESET_LOW     pPIOA->PIO_CODR   = BIT2
#define LCD_RESET_HIGH    pPIOA->PIO_SODR   = BIT2

// mask definitions
#define   BIT0        0x00000001
#define   BIT1        0x00000002
#define   BIT2        0x00000004
#define   BIT3        0x00000008
#define   BIT4        0x00000010
#define   BIT5        0x00000020
#define   BIT6        0x00000040
#define   BIT7        0x00000080
#define   BIT8        0x00000100
#define   BIT9        0x00000200
#define   BIT10       0x00000400
#define   BIT11       0x00000800
#define   BIT12       0x00001000
#define   BIT13       0x00002000
#define   BIT14       0x00004000
#define   BIT15       0x00008000
#define   BIT16       0x00010000
#define   BIT17       0x00020000
#define   BIT18       0x00040000
#define   BIT19       0x00080000
#define   BIT20       0x00100000
#define   BIT21       0x00200000
#define   BIT22       0x00400000
#define   BIT23       0x00800000
#define   BIT24       0x01000000
#define   BIT25       0x02000000
#define   BIT26       0x04000000
#define   BIT27       0x08000000
#define   BIT28       0x10000000
#define   BIT29       0x20000000
#define   BIT30       0x40000000
#define   BIT31       0x80000000
















//********************************************************************
//
//			PHILLIPS Controller Definitions
//
//********************************************************************
//LCD Commands
//	Apr  4,	2010	<MLS> see definitions below, 

#if 1
	#define	NOPP		0x00	// nop 
	#define	BSTRON		0x03	
	#define SLEEPIN     0x10	
	#define	SLEEPOUT	0x11	
	#define	NORON		0x13	
	#define	INVOFF		0x20	
	#define INVON      	0x21	
	#define	SETCON		0x25	
	#define DISPOFF     0x28	
	#define DISPON      0x29	
	#define CASETP      0x2A	
	#define PASETP      0x2B	
	#define RAMWRP      0x2C	
	#define RGBSET	    0x2D	
	#define	MADCTL		0x36	
	#define	COLMOD		0x3A	
	#define DISCTR      0xB9	
	#define	EC			0xC0	
#endif

//*************************************************************************************
//	LCD Include File for Philips PCF8833 STN RGB- 132x132x3 Driver 
// 
//		Taken from Philips data sheet Feb 14, 2003 
//*************************************************************************************
//*	I changed them to P_ for Philips
//*	many of these commands are not used but I wanted them all listed in case
//*	anyone wants to write more features
//	Philips PCF8833 LCD controller command codes 
/*#define	P_NOP			0x00	// nop 
#define	P_SWRESET		0x01	// software reset 
#define	P_BSTROFF		0x02	// booster voltage OFF 
#define	P_BSTRON		0x03	// booster voltage ON 
#define	P_RDDIDIF		0x04	// read display identification 
#define	P_RDDST			0x09	// read display status 
#define	P_SLEEPIN		0x10	// sleep in 
#define	P_SLEEPOUT		0x11	// sleep out 
#define	P_PTLON			0x12	// partial display mode 
#define	P_NORON			0x13	// display normal mode 
#define	P_INVOFF		0x20	// inversion OFF 
#define	P_INVON			0x21	// inversion ON 
#define	P_DALO			0x22	// all pixel OFF 
#define	P_DAL			0x23	// all pixel ON 
#define	P_SETCON		0x25	// write contrast 
#define	P_DISPOFF		0x28	// display OFF 
#define	P_DISPON		0x29	// display ON 
#define	P_CASET			0x2A	// column address set 
#define	P_PASET			0x2B	// page address set 
#define	P_RAMWR			0x2C	// memory write 
#define	P_RGBSET		0x2D	// colour set 
#define	P_PTLAR			0x30	// partial area 
#define	P_VSCRDEF		0x33	// vertical scrolling definition 
#define	P_TEOFF			0x34	// test mode 
#define	P_TEON			0x35	// test mode 
#define	P_MADCTL		0x36	// memory access control 
#define	P_SEP			0x37	// vertical scrolling start address 
#define	P_IDMOFF		0x38	// idle mode OFF 
#define	P_IDMON			0x39	// idle mode ON 
#define	P_COLMOD		0x3A	// interface pixel format 
#define	P_SETVOP		0xB0	// set Vop
#define	P_BRS			0xB4	// bottom row swap 
#define	P_TRS			0xB6	// top row swap 
#define	P_DISCTR		0xB9	// display control 
#define	P_DOR			0xBA	// data order 
#define	P_TCDFE			0xBD	// enable/disable DF temperature compensation 
#define	P_TCVOPE		0xBF	// enable/disable Vop temp comp 
#define	P_EC			0xC0	// internal or external oscillator 
#define	P_SETMUL		0xC2	// set multiplication factor 
#define	P_TCVOPAB		0xC3	// set TCVOP slopes A and B 
#define	P_TCVOPCD		0xC4	// set TCVOP slopes c and d 
#define	P_TCDF			0xC5	// set divider frequency 
#define	P_DF8COLOR		0xC6	// set divider frequency 8-color mode 
#define	P_SETBS			0xC7	// set bias system 
#define	P_RDTEMP		0xC8	// temperature read back 
#define	P_NLI			0xC9	// n-line inversion 
#define	P_RDID1			0xDA	// read ID1 
#define	P_RDID2			0xDB	// read ID2 
#define	P_RDID3			0xDC	// read ID3 
 

*/















#endif   // Lcd_h

