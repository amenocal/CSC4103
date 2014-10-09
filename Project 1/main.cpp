//#include <QCoreApplication>
#include <vector>
#include <math.h>
#include "bitmap.h"

int sobel_x[3][3] = { { 1, 0,-1},
                      { 2, 0,-2},
                      { 1, 0,-1}};

int sobel_y[3][3] = { { 1, 2, 1},
                      { 0, 0, 0},
                      {-1,-2,-1}};

/// Decleration of functions.
unsigned char* findEdge(const unsigned char*inData, const unsigned int w,
                        const unsigned int h, const unsigned int imgSize);


// Qt entry point!
int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    char* bmpFile;
    if( argc < 2)
      {
	bmpFile = "4103.bmp";
	printf("Taking 4103.bmp file at current diretory!\n");
	//return 0;
      }
    else
      bmpFile = argv[2]; 
    Bitmap *image = new Bitmap();
    //const char* bmpFile = "/Users/Bidur/Public/F2013_4103/FindEdge/4103.bmp";
    unsigned char*data = image->readGrayBitmap(bmpFile);
    unsigned char*edged = findEdge(data, image->bmpWidth, image->bmpHeight,
                                   image->bmpSize);
    image->writeGrayBmp(edged);
    return 0;
    //return a.exec();
}


unsigned char* findEdge(const unsigned char*inData,
                        const unsigned int w,
                        const unsigned int h,
                        const unsigned int size)
{
    int gradient_X = 0;
    int gradient_Y = 0;
    int value = 0;

//    unsigned int w = image.bmpWidth;
//    unsigned int h = image.bmpHeight;

    std::vector<unsigned char> outData;
    outData.clear();
    outData.resize(size, 255);

    /******************************************/
    // Reimplement computations inside FOR loop in multi-threading, using
    // either Pthread or Java threads. The FOR loop apply Sobel operator
    // to bitmap image data in per-pixel level.
    for(unsigned int y = 1; y < h-1; ++y)
        for(unsigned int x = 1; x < w-1; ++x)
        {
            // Compute gradient in +ve x direction
            gradient_X = sobel_x[0][0] * inData[ (x-1) + (y-1) * w ]
                    + sobel_x[0][1] * inData[  x    + (y-1) * w ]
                    + sobel_x[0][2] * inData[ (x+1) + (y-1) * w ]
                    + sobel_x[1][0] * inData[ (x-1) +  y    * w ]
                    + sobel_x[1][1] * inData[  x    +  y    * w ]
                    + sobel_x[1][2] * inData[ (x+1) +  y    * w ]
                    + sobel_x[2][0] * inData[ (x-1) + (y+1) * w ]
                    + sobel_x[2][1] * inData[  x    + (y+1) * w ]
                    + sobel_x[2][2] * inData[ (x+1) + (y+1) * w ];

            // Compute gradient in +ve y direction
            gradient_Y = sobel_y[0][0] * inData[ (x-1) + (y-1) * w ]
                    + sobel_y[0][1] * inData[  x    + (y-1) * w ]
                    + sobel_y[0][2] * inData[ (x+1) + (y-1) * w ]
                    + sobel_y[1][0] * inData[ (x-1) +  y    * w ]
                    + sobel_y[1][1] * inData[  x    +  y    * w ]
                    + sobel_y[1][2] * inData[ (x+1) +  y    * w ]
                    + sobel_y[2][0] * inData[ (x-1) + (y+1) * w ]
                    + sobel_y[2][1] * inData[  x    + (y+1) * w ]
                    + sobel_y[2][2] * inData[ (x+1) + (y+1) * w ];

            value = (int)ceil(sqrt( gradient_X * gradient_X +
                                    gradient_Y * gradient_Y));
            outData[ x + y*w ] = 255 - value;
        }
    return &outData[0];
}
