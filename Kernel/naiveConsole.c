#include "naiveConsole.h"
#include <stdint.h>
static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25;
static const uint32_t widthHalf=37;
static const uint32_t rightStart=42*2;
static  uint32_t currentLeft=0;
static  uint32_t currentRight=41*2;
static  uint32_t leftHeight=0;
static  uint32_t rightHeight=0;

enum fds{STDOUT = 1, STDERR, STDDER, STDIZQ, STDBOTH, STDERRDER, STDERRIZQ, STDERRBOTH};


void ncScroll(){
    for (int i=0; i<width*(height-1)*2; i++)
        video[i]=video[i + (width*2)];

    for(int t=width*(height-1)*2; t<(width*height*2); t+=2)
        video[t]=' ';

    currentVideo-=width*2;
}


void ncScrollLeft(){
    for(int i=0; i<height-1; i++){
        for( int j=0; j<widthHalf*2+2; j++)
            video[j+i*width*2]=video[j+(i+1)*width*2];
    }

    for (int t = width*(height-1)*2; t < width*(height-1)*2+widthHalf*2;t+=2)
        video[t]=' ';

    leftHeight=24;

}
void ncScrollRight(){
    for(int i=0; i<height-1; i++){
        for( int j=rightStart; j<width*2; j++)
            video[j+i*width*2]=video[j+(i+1)*width*2];
    }

    for (int t = width*(height-1)*2+rightStart; t < width*height*2 ;t+=2)
        video[t]=' ';

    rightHeight=24;

}

void checkScroll(){
    if((currentVideo-video) >= (width*2*height)){
        ncScroll();
    }
}

void ncPrintFdAttribute(uint8_t fd, const char * string , int color, int backColor){
    switch (fd){
        case STDOUT:
            ncPrintAttribute(string, color, backColor);
            break;
        case STDERR:
            ncPrintAttribute(string, color, backColor);
            break;
        case STDBOTH:
            ncPrintLeftAttribute(string,color,backColor);
            ncPrintRightAttribute(string, color, backColor);
            break;
        case STDDER:
            ncPrintRightAttribute(string, color, backColor);
            break;
        case STDIZQ:
            ncPrintLeftAttribute(string,color,backColor);
            break;
        case STDERRBOTH:
            ncPrintLeftAttribute(string,color,backColor);
            ncPrintRightAttribute(string, color, backColor);
            break;
        case STDERRDER:
            ncPrintRightAttribute(string, color, backColor);
            break;
        case STDERRIZQ:
            ncPrintLeftAttribute(string, color, backColor);
            break;
    }
}

void ncPrint(const char * string)
{
    for (int i = 0; string[i] != 0; i++)
        ncPrintChar(string[i]);
}

void ncPrintLeft(const char * string){
    for (int i = 0; string[i] != 0; i++)
        ncPrintCharLeftAttribute(string[i], White, Black);
}

void ncPrintRight(const char * string){
    for (int i = 0; string[i] != 0; i++)
        ncPrintCharRightAttribute(string[i], White, Black);
}

void ncPrintAttribute(const char * string, int color, int backColor){
    int i;

    for (i = 0; string[i] != 0; i++)
        ncPrintCharAttribute(string[i], color, backColor);
}

void ncPrintCharAttribute(char character, int chColor, int backColor)
{
    checkScroll();
    *currentVideo = character;
    currentVideo++;
    *currentVideo = backColor<<4 | chColor;
    currentVideo++;
}
void ncPrintCharFdAttribute(uint8_t fd, char character, int color, int backColor){
    switch (fd){
        case STDOUT:
            ncPrintCharAttribute(character, color, backColor);
            break;
        case STDERR:
            ncPrintCharAttribute(character, color, backColor);
            break;
        case STDBOTH:
            ncPrintCharLeftAttribute(character,color,backColor);
            ncPrintCharRightAttribute(character, color, backColor);
            break;
        case STDDER:
            ncPrintCharRightAttribute(character, color, backColor);
            break;
        case STDIZQ:
            ncPrintCharLeftAttribute(character,color,backColor);
            break;
        case STDERRBOTH:
            ncPrintCharLeftAttribute(character,color,backColor);
            ncPrintCharRightAttribute(character, color, backColor);
            break;
        case STDERRDER:
            ncPrintCharRightAttribute(character, color, backColor);
            break;
        case STDERRIZQ:
            ncPrintCharLeftAttribute(character, color, backColor);
            break;
    }
}
void ncPrintChar(char character)
{
    checkScroll();
	*currentVideo = character;
	currentVideo += 2;
}
void ncPrintLeftAttribute(const char * string, int chColor, int backColor){
    int i;
    for (i = 0; string[i] != 0; i++)
        ncPrintCharLeftAttribute(string[i], chColor, backColor);
}

void ncPrintCharLeftAttribute(char character, int chColor, int backColor){
    if(character=='\n'){
        ncNewlineLeft();
        return;
    }

    if(currentLeft>widthHalf*2){
        currentLeft=0;
        leftHeight++;
        if(leftHeight>24){
            ncScrollLeft();
        }
    }

    video[leftHeight*width*2+currentLeft]=character;
    currentLeft++;
    video[leftHeight*width*2+currentLeft]= backColor<<4 | chColor;
    currentLeft++;
}

void ncPrintRightAttribute(const char * string, int chColor, int backColor){
    int i;
    for (i = 0; string[i] != 0; i++)
        ncPrintCharRightAttribute(string[i], chColor, backColor);
}

void ncPrintCharRightAttribute(char character, int chColor, int backColor){
    if(character=='\n'){
        ncNewlineRight();
        return;
    }
    if(currentRight>(width-1)*2){
        currentRight=rightStart;
        rightHeight++;
        if(rightHeight>24){
            ncScrollRight();
        }
    }
    video[rightHeight*width*2+currentRight]=character;
    currentRight++;
    video[rightHeight*width*2+currentRight]= backColor<<4 | chColor;
    currentRight++;
}

void ncPrintCharBothAttribute(char character, int chColor, int backColor){
    ncPrintCharLeftAttribute(character, chColor, backColor);
    ncPrintCharRightAttribute(character, chColor, backColor);
}
void ncNewLineFd(uint8_t fd){
    switch (fd){
        case STDERR:
        case STDOUT:
            ncNewline();
            break;
        case STDERRBOTH:
        case STDBOTH:
            ncNewlineLeft();
            ncNewlineRight();
            break;
        case STDERRDER:
        case STDDER:
            ncNewlineRight();
            break;
        case STDERRIZQ:
        case STDIZQ:
            ncNewlineLeft();
            break;
    }
}

void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncNewlineLeft(){
    do {
        ncPrintCharLeftAttribute(' ', White,Black);
    }
    while(currentLeft<(widthHalf+1)*2);
}

void ncNewlineRight(){
    do {
        ncPrintCharRightAttribute(' ', White, Black);
    }
    while(currentRight<width*2);
}

void ncNewlineBoth(){
    ncNewlineLeft();
    ncNewlineRight();
}

void ncPrintDec(uint64_t value){
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value){
    ncPrint("0x");
	ncPrintBase(value, 16);
}

void ncPrintHexFdAttribute(uint8_t fd, uint64_t value, int color, int backColor){
    ncPrintFdAttribute(fd,"0x", color, backColor);
    ncPrintBaseFdAttribute(fd, value, 16, color, backColor);
}

void ncPrintBin(uint64_t value){
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base){
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncPrintBaseFdAttribute(uint8_t fd, uint64_t value, uint32_t base, int color, int backColor){
    uintToBase(value, buffer, base);
    ncPrintFdAttribute(fd, buffer, color, backColor);
}

void ncClearFd(uint8_t fd){
    switch (fd){
        case STDERR:
        case STDOUT:
            ncClear();
            break;
        case STDERRBOTH:
        case STDBOTH:
            ncClearLeft();
            ncClearRight();
            break;
        case STDERRDER:
        case STDDER:
            ncClearRight();
            break;
        case STDERRIZQ:
        case STDIZQ:
            ncClearLeft();
            break;

    }
}

void ncClear(){
	int i;
	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';

	currentVideo = video;
    currentLeft=0;
    currentRight=rightStart;
    rightHeight=0;
    leftHeight=0;
}

void ncClearRight(){
    for(int i=0; i<height-1; i++){
        for( int j=rightStart; j<width*2; j+=2){
            video[j+i*width*2]=' ';
        }
    }
    currentRight=rightStart;
    rightHeight=0;
    return;
}

void ncClearLeft(){
    for(int i=0; i<height-1; i++){
        for( int j=0; j<widthHalf*2+2; j+=2){
            video[j+i*width*2]=' ';
        }
    }
    currentLeft=0;
    leftHeight=0;
    return;
}

void ncDeleteChar(){
    if(currentVideo >= video + 2){
        currentVideo -= 2;
        *currentVideo = ' ';
    }
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base){
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}


