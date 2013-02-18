
#if !defined(AFX_IMAGE_H__E34B6FBB_5FED_457A_B3D1_D4844DCD4951__INCLUDED_)
#define AFX_IMAGE_H__E34B6FBB_5FED_457A_B3D1_D4844DCD4951__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "stdafx.h"
#include "resource.h"
#include <math.h>
#include <stdio.h>

// Class structure of Image 
class MyImage 
{

private:
	int		Width;
	int		Height;
	int		Y;
	int		U;
	int		V;
	int		Q;
	char	ImagePath[_MAX_PATH];
	unsigned char* bytesRGB;
	unsigned char* bytesRGBOUTSTART;
	unsigned char* bytesRGBOUTEND;
	unsigned char* bytesRGBOUTGRAYSCALE;
	unsigned char* bytesRGBOUTFLOYD;
	unsigned char* bytesRGBOUTAVG;

	// Used to perform Upsampling using the surrounding pixels. Storing the single Y row
	// into Height number of rows, each with Width values 
	float** floatsYBlock;
	float** floatsUBlock;
	float** floatsVBlock;

	float** bytesRBlock;
	float** bytesGBlock;
	float** bytesBBlock;

	unsigned char* bytesRPalette;
	unsigned char* bytesGPalette;
	unsigned char* bytesBPalette;

	unsigned char** bytesROutBlock;
	unsigned char** bytesGOutBlock;
	unsigned char** bytesBOutBlock;

	float* floatsYUV;
	float* floatsYUVSS;
	float* floatsYUVUS;

public:
	MyImage() {};
	~MyImage() { if(bytesRGB) delete bytesRGB; };
	
	// SET methods
	void	setWidth(int w)  { Width = w; }; 
	void	setHeight(int h) { Height = h; }; 
	void	setBytesRGB(unsigned char *img ) { bytesRGB = img; };
	void	setImagePath(char *path) { strcpy(ImagePath, path); }
	void	setY(int y) {Y = y;}
	void	setU(int u) {U = u;}
	void	setV(int v) {V = v;}
	void	setQ(int q) {Q = q;}

	// GET methods
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	unsigned char*	getBytesRGBSTART() { return bytesRGBOUTSTART; };
	unsigned char*	getBytesRGBEND() { return bytesRGBOUTEND; };
	unsigned char*	getBytesRGBGRAYSCALE() { return bytesRGBOUTGRAYSCALE; };
	unsigned char*	getBytesRGBFLOYD() { return bytesRGBOUTFLOYD; };
	unsigned char*	getBytesRGBAVG() { return bytesRGBOUTAVG; };
	char*	getImagePath() { return ImagePath; }
	int		getY() { return Y; };
	int		getU() { return U; };
	int		getV() { return V; };
	int		getQ() { return Q; };

	// Computation methods
	void	FillInputRGBSpace();
	void	RGB2YUV();
	void	YUV2RGB();
	void	SubSample();
	void	UpSample();
	void	Quantize();
	void	UpSampleSurround();
	void	GrayScale();
	void	FloydSteinberg();
	float	FindNewPixel(float oldGrayValue);
	void	AverageDithering();
	void	FillPalette();

	void	Print();
};

#endif // !defined(AFX_IMAGE_H__E34B6FBB_5FED_457A_B3D1_D4844DCD4951__INCLUDED_)
