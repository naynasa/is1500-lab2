/*copy of IOShieldOled.h in thirdpartylibraries_io_shield/IOShieldOled/ reweitten to C for our purposes*/

#if !defined(IOSHIELDOLED_H)
#define IOSHIELDOLED_H

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

#include <inttypes.h>




/* ------------------------------------------------------------ */
/*					Object Declarations					*/
/* ------------------------------------------------------------ */


	
/* Class Constants
*/
static const int colMax = 128;	//number of columns in the display
static const int rowMax = 32;	//number of rows in the display
static const int pageMax = 4;	//number of display pages

static const int modeSet = 0;	//set pixel drawing mode
static const int modeOr  = 1;	//or pixels drawing mode
static const int modeAnd = 2;	//and pixels drawing mode
static const int modeXor = 3;	//xor pixels drawing mode

//IOShieldOledClass(); might need to uncomment this

/* Basic device control functions.
*/
void		begin(void);
void		end(void);
void		displayOn(void);
void		displayOff(void);
void		clear(void);
void		clearBuffer(void);
void		updateDisplay(void);

/* Character output functions.
*/
void		setCursor(int xch, int ych);
void		getCursor(int * pxch, int * pych);
int			defineUserChar(char ch, uint8_t * pbDef);
void		setCharUpdate(int f);
int			getCharUpdate(void);
void		putChar(char ch);
void		putString(char * sz);
/*void    putString(const char *);*/

/* Graphic output functions.
*/
void		setDrawColor(uint8_t clr);
void		setDrawMode(int mod);
int			getDrawMode();
uint8_t *	getStdPattern(int ipat);
void		setFillPattern(uint8_t * pbPat);

void		moveTo(int xco, int yco);
void		getPos(int * pxco, int * pyco);
void		drawPixel(void);
uint8_t		getPixel(void);
void		drawLine(int xco, int yco);
void		drawRect(int xco, int yco);
void		drawFillRect(int xco, int yco);
void		getBmp(int dxco, int dyco, uint8_t * pbBmp);
void		putBmp(int dxcp, int dyco, uint8_t * pbBmp);
void		drawChar(char ch);
void		drawString(char *sz);
/*void		drawString(const char *sz);*/

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
