
#include <fstream>
#include <iostream>
#include <GL/glut.h>
#include "RGBpixmap.h"

#define checkImageWidth 512
#define checkImageHeight 512

using namespace std;

fstream inf;

static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

/**
 * Generate a simple black-and-white checkerboard 
 * pattern and store it in the checkImage array.
 */
void RGBpixmap::makeCheckImage() {
	int i, j, c;
	// iterate through each pixel in the checkImage array
	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			// set the pixel's color to white if either of its coordinates are 
			// multiples of 8 (black otherwise)
			c = ( ( (i & 0x8) == 0) ^ ((j & 0x8) == 0) ) * 255;

			// create grayscale color by assigning c to the RGB channels
			checkImage[i][j][0] = (GLubyte) c;
			checkImage[i][j][1] = (GLubyte) c;
			checkImage[i][j][2] = (GLubyte) c;

			// set the transparency (akpha) to 255 (opaque)
			checkImage[i][j][3] = (GLubyte) 255;
		}
	}
}


/**
 * Generate a checkerboard pattern and store it
 * in the pixel array.
 */
void RGBpixmap::makeCheckerBoard() {
	// make checkerboard pattern
	nRows = nCols = 64;
	pixel = new mRGB[3 * nRows * nCols];
	if (!pixel) {
		cout << "Out of memeory!";
		return;
	}

	long count = 0;
	// iterate through each row and col of the checkerboard
	for(int i = 0; i < nRows; i++)
		for(int j = 0; j < nCols; j++) {
			// calculate color value c: if the sum is even, c is
			// set to 255 (white); 0 (black) otherwise
			int c = (( (i / 8) + (j / 8) ) % 2) * 255;

			// set the RGB channels
			pixel[count].r = c;		//red
			pixel[count].g = c;		//green
			pixel[count++].b = 0;	//blue
		}
}


/**
 * Set up a 2D texture in OpenGL with the specified parameters.
 * 
 * @param textureName The name of the texture that will be set up.
 */
void RGBpixmap::setTexture(GLuint textureName) {
	// bind the given texture 
	glBindTexture(GL_TEXTURE_2D,textureName);

	// set the texture magnification and minification filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// set the texture wrap modes for S and T coordinates
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture coordinate generation modes for S and T coordinates
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

	// set texture environment mode
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// if the textureName is 1, use the checkImage data
	if (textureName == 1) {
		glTexImage2D(
			GL_TEXTURE_2D,		// target texture being specified
			0,					// level of detail (mipmap level)
			GL_RGBA,			// internal format of the texture
			checkImageWidth,	// texture image width
			checkImageHeight,	// texture image height
			0,					// border width
			GL_RGBA,			// pixel data format
			GL_UNSIGNED_BYTE,	// texel value data type
			checkImage			// pixel data
		);
	} else {
		// otherwise use the pixel data stored in the object
		glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			GL_RGBA, 
			nRows, 
			nCols, 
			0, 
			GL_RGBA, 
			GL_UNSIGNED_BYTE, 
			pixel
		);

		// enable texture coordinate generation for S and T coordinates
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}

}


/**
 * Read two bytes from an input stream and combine them
 * to form a 16-bit unsigned short integer.
 */
ushort getShort() {
	char ic;
	ushort ip;
	inf.get(ic);
	ip = ic;
	inf.get(ic);
	ip |= ((ushort)ic << 8);
	return ip;
}


/**
 * Read four bytes from an input stream and combine them
 * to form a 32-bit unsigned long integer.
 */
ulong getLong() {
	ulong ip = 0;
	char ic = 0;
	unsigned char uc = ic;
	inf.get(ic);
	uc = ic; ip = uc;
	inf.get(ic); uc = ic; ip |= ((ulong)uc << 8);
	inf.get(ic); uc = ic; ip |= ((ulong)uc << 16);
	inf.get(ic); uc = ic; ip |= ((ulong)uc << 24);
	return ip;
}


/**
 * Read a BMP (Windows Bitmap) image file and extract pixel information from it.
 *
 * @param fname The file name (including path) of the BMP image file to be read.
 */
void RGBpixmap::readBMPFile( char* fname) {
	// open the given file in binary mode
	inf.open(fname, ios::in | ios::binary); 
	if(!inf) {
		cout <<"cannot open file!!!!" << fname << endl;
		return;
	}

	int k, row, col, numPadBytes, nBytesInRow;

	// BMP file header information
	char ch1, ch2;
	inf.get(ch1);
	inf.get(ch2); 
	ulong fileSize = getLong();
	ushort reserved1 = getShort();		// always 0
	ushort reserved2 = getShort();		// always 0
	ulong offBits = getLong();			// offset to image unreliable
	ulong headerSize = getLong();		// always 40
	ulong numCols = getLong();			// number of colums in image
	ulong numRows = getLong();			// number of rows in image
	ushort planes = getShort();			// always 1
	ushort bitsPerPixel = getShort();	// 8 or 24; allow only 24 here
	ulong compression = getLong();		// must be 0 for uncompressed
	ulong imageSize = getLong();		// total byte in image
	ulong xPels = getLong();			// always 0
	ulong yPels = getLong();			// always 0
	ulong numLUTentries = getLong();	// 256 for 8 bit, otherwise 0
	ulong impColors = getLong();		// always 0

	// verify that the image is 24-bit and uncompressed
	if (bitsPerPixel != 24 || compression != 0) {
		cout << "Not a 24 bit pixel image or is compressed" << endl;
		inf.close();
		return ;
	}
	
	// calculate padding and set class data members
	nBytesInRow = ((3 * numCols + 3) / 4) * 4;
	numPadBytes = nBytesInRow - 3 * numCols;
	nRows = numRows; 
	nCols = numCols;

	// allocate space for the pixel array
	pixel = new mRGB[nRows * nCols];

	// check if out of memory
	if(!pixel)
		return; 

	long count = 0;
	char dum;

	// read pixel values
	for(row = 0; row < nRows; row++) {
		for (col = 0; col < nCols; col++) {
			char r,g,b;

			// read bytes
			inf.get(b); inf.get(g); inf.get(r); 

			// place them in colors
			pixel[count].r = r; 
			pixel[count].g = g;
			pixel[count++].b = b;
		}

		//skip padBytes at the end of each row
		for (k = 0; k < numPadBytes; k++) {
			inf >> dum;
		}
	}

	inf.close();
	return;
}
