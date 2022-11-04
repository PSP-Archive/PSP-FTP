

#define IS_ALPHA(value) (((value)&0xff000000)==0)
#define	PSP_LINE_SIZE 512   // in u32
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272
#define		PIXELSIZE	1				//in short
#define		FRAMESIZE	0x44000			//in byte

#define pg_vramtop ((char *)0x04000000)

long pg_drawframe;


u32* pg_vramtopu32 = (u32*)(0x04000000+0x40000000);
u32* pgGetVramAddru32(unsigned long x,unsigned long y)
{
	return pg_vramtopu32+(y * PSP_LINE_SIZE)+x+(pg_drawframe?PSP_LINE_SIZE*SCREEN_HEIGHT:0);
}

char *pgGetVramAddr(unsigned long x,unsigned long y)
{
	return pg_vramtop+(pg_drawframe?FRAMESIZE:0)+x*PIXELSIZE*2+y*PSP_LINE_SIZE*2+0x40000000;
}

const char* makeLocalFilename(const char* path, const char* filename)
{
	static char buf[256];
	strcpy(buf, path);
	strcat(buf, filename);
	return buf;
}

void writeByte(int fd, unsigned char data)
{
	sceIoWrite(fd, &data, 1);
}

void createscreenshot(char* path, char* name)
{
	const char *filename = makeLocalFilename(path, name);
	const char tgaHeader[] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	const int width = SCREEN_WIDTH;
	const int lineWidth = PSP_LINE_SIZE;
	const int height = SCREEN_HEIGHT;
	unsigned char lineBuffer[width*4];
	u32* vram = pgGetVramAddru32(0, 0);
	int x, y;
	int fd = sceIoOpen(filename, PSP_O_CREAT | PSP_O_TRUNC | PSP_O_WRONLY, 0777);
	if (fd) {
		sceIoWrite(fd, tgaHeader, sizeof(tgaHeader));
		writeByte(fd, width & 0xff);
		writeByte(fd, width >> 8);
		writeByte(fd, height & 0xff);
		writeByte(fd, height >> 8);
		writeByte(fd, 24);
		writeByte(fd, 0);
		for (y = height - 1; y >= 0; y--) {
			for (x = 0; x < width; x++) {
				u32 color = vram[y * lineWidth + x];
				unsigned char red = color & 0xff;
				unsigned char green = (color >> 8) & 0xff;
				unsigned char blue = (color >> 16) & 0xff;
				lineBuffer[3*x] = blue;
				lineBuffer[3*x+1] = green;
				lineBuffer[3*x+2] = red;
			}
			sceIoWrite(fd, lineBuffer, width * 3);
		}
		sceIoClose(fd);
	}
}
