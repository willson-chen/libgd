#include "gd.h"
#include "gdtest.h"

int main()
{
	gdImagePtr im;
	im = gdImageCreate(64, 64);
	int white;

	gdImageColorAllocate(im, 0, 0, 0);
	white = gdImageColorAllocate(im, 255, 255, 255);
	gdImageDashedLine(im, 0, 0, 64, 64, white);
	gdImageDashedLine(im, 10, 0, 60, 63, white);
	gdImageDestroy(im);
	return 0;
}
