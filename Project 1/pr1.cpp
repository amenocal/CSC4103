//Name: Alejandro Menocal
//File: pr1.cpp
//Instructor: Brygg Ullmer
//Class and section: cs4103
//LogonID: cs410315


//#include <QCoreApplication>
#include <vector>
#include <math.h>
#include "cscbitmap.h"
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

int sobel_x[3][3] = { { 1, 0,-1},
                      { 2, 0,-2},
                      { 1, 0,-1}};

int sobel_y[3][3] = { { 1, 2, 1},
                      { 0, 0, 0},
                      {-1,-2,-1}};

/// Decleration of functions.
typedef struct
{  unsigned int s_row; unsigned int e_row;
}th_param;
void* findEdge(void*p);
unsigned int h;
unsigned int w;
unsigned int Num_Threads;

/// Memory to hold input image data
unsigned char* inData;
std::vector<unsigned char> image_sobeled;


// Qt entry point!
// Create and run a thread inside main function.
// Also assign region of image to a thread, and 
// call thread join for synchronization.

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

	char* Num_Th;
    char* bmpFile;
    if( argc < 2)
      {
	bmpFile = "4103.bmp";
	printf("Taking 4103.bmp file at current diretory!\n");
	//return 0;
      }
    else{
      bmpFile = argv[1];
	  Num_Th = argv[2];
	}
	  
    /// Open and read bmp file.
    Bitmap *image = new Bitmap();
    unsigned char*data = image->readGrayBitmap(bmpFile);
	
    image_sobeled.clear();
    image_sobeled.resize(image->bmpSize, 255);
    inData = data;
    
    //Initialization of Num of Threads, Height and Width
    Num_Threads = atoi(Num_Th);
	h = image->bmpHeight;
	w = image->bmpWidth;
    
    //Making an Array of Threads
	pthread_t Tid[Num_Threads];
	th_param prm[Num_Threads];
	
    //For Loops that tells where the threads are going to start and end according to the Number of Threads, and creating the threads
	for(int th = 0; th < Num_Threads; th++)
		{	
			prm[th].s_row = h/Num_Threads * th;
			prm[th].e_row = prm[th].s_row + h/Num_Threads;
			pthread_create(&Tid[th], NULL, findEdge, (void*)&prm[th]);
		}
    //findEdge(image->bmpWidth, image->bmpHeight);
    
    //Joining threads
	for(int th = 0; th < Num_Threads; th++)
		pthread_join(Tid[th], NULL);

    /// Write image data passed as argument to a bitmap file
    image->writeGrayBmp(&image_sobeled[0]);
    image_sobeled.clear();
    delete data;
	
    return 0;
    //return a.exec();
}

/// Function that implements Sobel operator.
/// Returns image data after applyig Sobel operator to the original image.
/// Reimplement findEdge such that it will run in a single thread
/// and can process on a region/group of pixels
void* findEdge(void*p)
{
    int gradient_X = 0;
    int gradient_Y = 0;
    int value = 0;
    
    //Getting Values from the struct and assigning it to the start_row and end_row
	th_param* prm = (th_param*)p;
	unsigned int start_row = prm->s_row;
	unsigned int end_row = prm->e_row;
    
	// The FOR loop apply Sobel operator
    // to bitmap image data in per-pixel level.
	for(unsigned int y = start_row+1; y < end_row-1; ++y)
	{
		if(y == 0)
			continue;
		for(unsigned int x = 1; x < w-1; ++x)
		{		 
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
            image_sobeled[ x + y*w ] = 255 - value;
		}
	}
	//Letting Pthreads to be joined together before the program ends
	pthread_exit(0);
	
    // Visual Studio requires this to be present; and should not 
    // cause issues for other compilers. 
    // Thanks to Thomas Peters.
    return 0;
}