#include "gd.h"
#include "gdtest.h"

int main()
{
	gdImagePtr im;
	im = gdImageCreate(64,64);
	int white;


	gdImageColorAllocate(im, 0, 0, 0);
	white = gdImageColorAllocate(im, 255, 255, 255);
	gdImageDashedLine(im, 0, 64, 64, 0, white);
	gdImageDashedLine(im, 0, 0, 10, 63, white);
	// generate png file code
	//FILE *pngout;
	//pngout = fopen("gd/test.png", "wb");
	//pngout = gdTestFileOpen2("gd", "test.png");
	//gdImagePng(im, pngout);
	//fclose(pngout);

	gdAssertImageEqualsToFile("gd/test.png", im);

	gdImageDestroy(im);
	return 0;
}
