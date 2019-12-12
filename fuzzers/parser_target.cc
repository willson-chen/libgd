// Copyright 2018 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
/////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include "gd.h"

#define PASTE(x) gdImageCreateFrom ## x ## Ptr
#define CREATE_IMAGE(FORMAT) PASTE(FORMAT)

void fromGd2PartTest(int a, int b, int c, int d) {
	FILE *fp;
	gdImagePtr im =  gdImageCreate(a, b);
	if (im) {
		gdImageColorAllocate(im, 0xFF, 0xFF, 0xFF);
		fp = fopen("gdImage.gd2", "wb");
		gdImageGd2(im, fp, c, d);
		if (!fp) {
			fprintf(stderr, "gdImage.gd2 dose not exist!\n");
			perror("fopen");
			exit (1);
		}
		gdImagePtr im1 = gdImageCreateFromGd2Part(fp, a, b, c, d); 
		if (im1) gdImageDestroy(im1);
	}
	if (im) gdImageDestroy(im);
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
	if (Size > 5) {
		int x = Data[0] % 2;
		int y = Data[0] % 2;
		int z = Data[0] % 2;
		int m = Data[0] % 2;
		Data +=4;
		Size -=4;
		
		gdImagePtr im = CREATE_IMAGE(FUZZ_GD_FORMAT)(Size, (void*) Data);
		if (im) {
			gdImageGetPixel(im, x, y);
			gdImageEllipse(im, x, y, z, m, 0x00ff00);
		}
		fromGd2PartTest(x, y, z, m);
		if (im) gdImageDestroy(im);
	}
    return 0;
}