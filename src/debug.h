#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG_USE_STATIC 0

#define CHAR_ALL		0
#define CHAR_LETTERS	1
#define CHAR_SPECIAL	2
#define CHAR_NUMBERS	3
#define CHAR_IP			4

// various color formats
#define RGB888(r,g,b) ( (b<<16)  | (g<<8) | (r) | 0xff000000 )
#define RGB444(r,g,b) ( ((b<<4)&0xf00)  | (g&0xf0) | ((r>>4)&0xf) | 0xf000 )
#define RGB565(r,g,b) ( ((b<<8) & 0xf800) | ((g<<3) & 0x7e0) | ((r>>3) & 0x1F) )

// various colors
#define WHITE RGB888(255,255,255)
#define GREY  RGB888(127,127,127)
#define RED   RGB888(255,0,0)
#define GREEN RGB888(0,255,0)
#define BLUE  RGB888(0,0,255)
#define BLACK RGB888(0,0,0)
#define YELLOW RGB888(255,255,0)
#define ORANGE RGB888(255,127,0)
#define VIOLET RGB888(255,0,255)
#define TEAL RGB888(0,255,255)

#define CMDSIZE 13		// number of debug commands available
#define STARTLINE	3	// line to begin debugLine info on
#define MAXLINES 	28	// maximum # of debugLines to show
#define MAXLENGTH 	200	// maximum length of debugLines
#define INPUT_LINE1		30	// line to show inputted info
#define INPUT_LINE2		32	// input info line
#define DEBUG_COLOR1	YELLOW	// default color
#define DEBUG_COLOR2	RED	// selected/active color

// structure for char sets
typedef struct charset {
	int		selected;
	char 	chars[75];
	char 	name[10];
} charset;

void debugClearLn(int num);
void debugTitle(char* info);
void debugFooter(void);
int debugCurrent(void);
void debugCMDInc(void);
void debugCMDDec(void);
void debugLine(char *fmt, ...);
int debugInput(char* input, char* desc, char* defaultval, int charset);

#endif
