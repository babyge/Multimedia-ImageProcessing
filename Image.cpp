// Image.cpp : Defines the entry point for the application.

#include "Image.h"

MyImage myImage;

#define MAX_LOADSTRING 100
#define MAX_COLOR_VARIATIONS 256
#define MAT_OLD 0
#define MAT_NEW 1
#define UPSAMPLE_LINEAR_INTERP 1
#define UPSAMPLE_SURROUND_PIXELS 0
#define GRAYSCALE 0
#define FLOYDSTEINBERG 0
#define AVERAGEDITHERING 0

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	int w = 352, h = 288;
	int why, you, we, queue;

	char ImagePath[_MAX_PATH];
	sscanf(lpCmdLine, "%s %d %d %d %d", &ImagePath, &why, &you, &we, &queue);
	myImage.setWidth(w);
	myImage.setHeight(h);
	myImage.setY(why);
	myImage.setU(you);
	myImage.setV(we);
	myImage.setQ(queue);
	myImage.setImagePath(ImagePath);

	myImage.FillInputRGBSpace();
	
	if(GRAYSCALE == 0 && FLOYDSTEINBERG == 0 && AVERAGEDITHERING ==0)
	{
		myImage.RGB2YUV();

		myImage.SubSample();
		
		if(UPSAMPLE_LINEAR_INTERP)
			myImage.UpSample();

		if(UPSAMPLE_SURROUND_PIXELS)
			myImage.UpSampleSurround();

		myImage.YUV2RGB();

		myImage.Quantize();
	}
	else if(GRAYSCALE == 1 && FLOYDSTEINBERG == 0 && AVERAGEDITHERING ==0)
	{
		myImage.GrayScale();
	}
	else if(GRAYSCALE == 1 && FLOYDSTEINBERG == 1 && AVERAGEDITHERING==0)
	{
		myImage.GrayScale();
		myImage.FloydSteinberg();
	}
	else if(GRAYSCALE == 1 && FLOYDSTEINBERG == 0 && AVERAGEDITHERING==1)
	{
		myImage.GrayScale();
		myImage.AverageDithering();
	}

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMAGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_IMAGE);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	
	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_IMAGE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_IMAGE;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
	
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	
	switch (message)
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			{
				hdc = BeginPaint(hWnd, &ps);
				// TODO: Add any drawing code here...
				RECT rt;
				GetClientRect(hWnd, &rt);

				// Top Text
				char text[1000];
				
				// Image Header setup
				BITMAPINFO bmi;
				CBitmap bitmap;
				memset(&bmi,0,sizeof(bmi));
				bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
				bmi.bmiHeader.biWidth = myImage.getWidth();
				bmi.bmiHeader.biHeight = -myImage.getHeight();  // Use negative height.  DIB is top-down.
				bmi.bmiHeader.biPlanes = 1;
				bmi.bmiHeader.biBitCount = 24;
				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biSizeImage = myImage.getWidth()*myImage.getHeight();
				

				// Draw Processed image
				if(GRAYSCALE == 0 && FLOYDSTEINBERG == 0 && AVERAGEDITHERING ==0)
				{
					strcpy(text, "\nfile.rgb -> display");
					DrawText(hdc, text, strlen(text), &rt, DT_LEFT);
					sprintf(text, "\n file.rgb -> YUV -> Sub Sampled (%d : %d : %d) -> Up sampled -> RGB -> Quantized ( %d )", myImage.getY(), myImage.getU(), myImage.getV(), myImage.getQ());
					DrawText(hdc, text, strlen(text), &rt, DT_RIGHT);

						// Draw image 
					SetDIBitsToDevice(hdc,
									  100,100,myImage.getWidth(),myImage.getHeight(),
									  0,0,0,myImage.getHeight(),
									  myImage.getBytesRGBSTART(),&bmi,DIB_RGB_COLORS);

					SetDIBitsToDevice(hdc,
									  300+myImage.getWidth()+50,100,myImage.getWidth(),myImage.getHeight(),
									  0,0,0,myImage.getHeight(),
									  myImage.getBytesRGBEND(),&bmi,DIB_RGB_COLORS);
				}
				else if(GRAYSCALE == 1 && FLOYDSTEINBERG == 0 && AVERAGEDITHERING==0)
				{
					strcpy(text, "\nfile.rgb -> display");
					DrawText(hdc, text, strlen(text), &rt, DT_LEFT);
					sprintf(text, "\n file.rgb -> Gray Scaled ", myImage.getY(), myImage.getU(), myImage.getV(), myImage.getQ());
					DrawText(hdc, text, strlen(text), &rt, DT_RIGHT);

					// Draw image 
					SetDIBitsToDevice(hdc,
									  100,100,myImage.getWidth(),myImage.getHeight(),
									  0,0,0,myImage.getHeight(),
									  myImage.getBytesRGBSTART(),&bmi,DIB_RGB_COLORS);

					SetDIBitsToDevice(hdc,
									  300+myImage.getWidth()+50,100,myImage.getWidth(),myImage.getHeight(),
									  0,0,0,myImage.getHeight(),
									  myImage.getBytesRGBGRAYSCALE(),&bmi,DIB_RGB_COLORS);
				}
				else if(GRAYSCALE == 1 && FLOYDSTEINBERG == 1 && AVERAGEDITHERING==0)
				{
					strcpy(text, "\n file.rgb -> Gray Scaled ");
					DrawText(hdc, text, strlen(text), &rt, DT_LEFT);
					sprintf(text, "\n file.rgb -> Gray Scaled -> Floyd Steinberg Dithering Algorithm", myImage.getY(), myImage.getU(), myImage.getV(), myImage.getQ());
					DrawText(hdc, text, strlen(text), &rt, DT_RIGHT);

					// Draw image 
					SetDIBitsToDevice(hdc,
									  100,100,myImage.getWidth(),myImage.getHeight(),
									  0,0,0,myImage.getHeight(),
									  myImage.getBytesRGBGRAYSCALE(),&bmi,DIB_RGB_COLORS);

					SetDIBitsToDevice(hdc,
									  300+myImage.getWidth()+50,100,myImage.getWidth(),myImage.getHeight(),
									  0,0,0,myImage.getHeight(),
									  myImage.getBytesRGBFLOYD(),&bmi,DIB_RGB_COLORS);
				}
				else if(GRAYSCALE == 1 && FLOYDSTEINBERG == 0 && AVERAGEDITHERING==1)
				{
					strcpy(text, "\n file.rgb -> Gray Scaled ");
					DrawText(hdc, text, strlen(text), &rt, DT_LEFT);
					sprintf(text, "\n file.rgb -> Gray Scaled -> Average Dithering Algorithm", myImage.getY(), myImage.getU(), myImage.getV(), myImage.getQ());
					DrawText(hdc, text, strlen(text), &rt, DT_RIGHT);

						// Draw image 
					SetDIBitsToDevice(hdc,
									  100,100,myImage.getWidth(),myImage.getHeight(),
									  0,0,0,myImage.getHeight(),
									  myImage.getBytesRGBGRAYSCALE(),&bmi,DIB_RGB_COLORS);

					SetDIBitsToDevice(hdc,
									  300+myImage.getWidth()+50,100,myImage.getWidth(),myImage.getHeight(),
									  0,0,0,myImage.getHeight(),
									  myImage.getBytesRGBAVG(),&bmi,DIB_RGB_COLORS);
				}

				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


// MyImage functions
void MyImage::FillInputRGBSpace()
{
	FILE *IN_FILE;
	int i;

	IN_FILE = fopen(ImagePath, "rb");

	if (IN_FILE == NULL)
	{
		fprintf(stderr, "Error");
		exit(0);
	}
	
	// Storing Format : [BGR][BGR]....[BGR]
	bytesRGBOUTSTART = new unsigned char[Width*Height*3];

	// Storing Format : [RRRRRR.....R][GGGGGG.....G][BBBBBB.....B]
	bytesRGB = new unsigned char[Width*Height*3];

	for(i = 0; i < Height*Width*3; i++)
		bytesRGB[i] = fgetc(IN_FILE);

	bytesRGBOUTSTART = new unsigned char[Width*Height*3];

	/* Convert Storing Format:
		FROM: [RRRRRR.....R][GGGGGG.....G][BBBBBB.....B]
		TO	: [BGR][BGR].....[BGR]
	*/
	for (i = 0; i < Height*Width; i++)
	{
		bytesRGBOUTSTART[3*i]	= bytesRGB[i+Height*Width*2];
		bytesRGBOUTSTART[3*i+1]	= bytesRGB[i+Height*Width];
		bytesRGBOUTSTART[3*i+2]	= bytesRGB[i];
	}
	
	fclose(IN_FILE);
}

void MyImage::RGB2YUV()
{
	float r_y = 0, g_u = 0, b_v = 0;

	// Storing Format : [RRRRRR.....R][GGGGGG.....G][BBBBBB.....B]
	floatsYUV = new float[Width*Height*3];
	floatsYUVSS = new float[Width*Height*3];
	floatsYUVUS = new float[Width*Height*3];

	for(int i = 0; i < Height*Width; i++)
	{
		r_y = bytesRGB[i];
		g_u = bytesRGB[i+Height*Width];
		b_v = bytesRGB[i+Height*Width*2];
		
		if(MAT_OLD)
		{
			floatsYUV[i] = (0.299 * r_y + 0.587 * g_u + 0.114 * b_v);
			floatsYUV[i+Height*Width] = ((-0.147 * r_y) + (-0.289 * g_u) + 0.436 * b_v);
			floatsYUV[i+Height*Width*2] = (0.615 * r_y + (-0.515 * g_u) + 0.312 * b_v);

			floatsYUVSS[i] = (0.299 * r_y + 0.587 * g_u + 0.114 * b_v);
			floatsYUVSS[i+Height*Width] = ((-0.147 * r_y) + (-0.289 * g_u) + 0.436 * b_v);
			floatsYUVSS[i+Height*Width*2] = (0.615 * r_y + (-0.515 * g_u) + 0.312 * b_v);

			floatsYUVUS[i] = (0.299 * r_y + 0.587 * g_u + 0.114 * b_v);
			floatsYUVUS[i+Height*Width] = ((-0.147 * r_y) + (-0.289 * g_u) + 0.436 * b_v);
			floatsYUVUS[i+Height*Width*2] = (0.615 * r_y + (-0.515 * g_u) + 0.312 * b_v);
		}
		else if(MAT_NEW)
		{
			floatsYUV[i] = (0.2990 * r_y + 0.5870 * g_u + 0.1140 * b_v);
			floatsYUV[i+Height*Width] = ((-0.1470 * r_y) + (-0.2890 * g_u) + 0.4360 * b_v); 
			floatsYUV[i+Height*Width*2] = (0.6150 * r_y + (-0.5140 * g_u) + (-0.1000 * b_v));	

			floatsYUVSS[i] = (0.2990 * r_y + 0.5870 * g_u + 0.1140 * b_v);
			floatsYUVSS[i+Height*Width] = ((-0.1470 * r_y) + (-0.2890 * g_u) + 0.4360 * b_v); 
			floatsYUVSS[i+Height*Width*2] = (0.6150 * r_y + (-0.5140 * g_u) + (-0.1000 * b_v));	

			floatsYUVUS[i] = (0.2990 * r_y + 0.5870 * g_u + 0.1140 * b_v);
			floatsYUVUS[i+Height*Width] = ((-0.1470 * r_y) + (-0.2890 * g_u) + 0.4360 * b_v); 
			floatsYUVUS[i+Height*Width*2] = (0.6150 * r_y + (-0.5140 * g_u) + (-0.1000 * b_v));	
		}
	}
}

void MyImage::YUV2RGB()
{
	float floatRGB[3];
	
	// Storing Format : [RRRRRR.....R][GGGGGG.....G][BBBBBB.....B]
	unsigned char *bytesTemp = new unsigned char[Width*Height*3];
	
	// Storing Format : [BGR][BGR]....[BGR]
	bytesRGBOUTEND = new unsigned char[Width*Height*3];
	
	for(int i = 0; i < Height*Width; i++)
	{
		if(MAT_OLD)
		{
			floatRGB[0] = (float)(0.5303 * floatsYUVUS[i] + (-0.9545 * floatsYUVUS[i+Height*Width]) + 1.1401 * floatsYUVUS[i+Height*Width*2]);
			floatRGB[1] = (float)(1.2392 * floatsYUVUS[i] + 0.0915 * floatsYUVUS[i+Height*Width] + (-0.5806 * floatsYUVUS[i+Height*Width*2])); 
			floatRGB[2] = (float)(1.0002 * floatsYUVUS[i] + 2.0324 * floatsYUVUS[i+Height*Width] + (-0.0005 * floatsYUVUS[i+Height*Width*2]));
		}
		else if(MAT_NEW)
		{
			floatRGB[0] = (float)(0.9989 * floatsYUVUS[i] + 0.0004 * floatsYUVUS[i+Height*Width] + 1.1405 * floatsYUVUS[i+Height*Width*2]);
			floatRGB[1] = (float)(1.0006 * floatsYUVUS[i] + (-0.3948 * floatsYUVUS[i+Height*Width]) + (-0.5808 * floatsYUVUS[i+Height*Width*2])); 
			floatRGB[2] = (float)(1.0000 * floatsYUVUS[i] + 2.0320 * floatsYUVUS[i+Height*Width] + (-0.0005 * floatsYUVUS[i+Height*Width*2]));
		}
		
		if(floatRGB[0] > 255)
			floatRGB[0] = 255;
		if(floatRGB[0] < 0)
			floatRGB[0] = 0;

		if(floatRGB[1] > 255)
			floatRGB[1] = 255;
		if(floatRGB[1] < 0)
			floatRGB[1] = 0;

		if(floatRGB[2] > 255)
			floatRGB[2] = 255;
		if(floatRGB[2] < 0)
			floatRGB[2] = 0;

		bytesTemp[i] = floatRGB[0];
		bytesTemp[i+Height*Width] = floatRGB[1];
		bytesTemp[i+Height*Width*2] = floatRGB[2];
	}

	/* Convert Storing Format:
		FROM: [RRRRRR.....R][GGGGGG.....G][BBBBBB.....B]
		TO	: [BGR][BGR].....[BGR]
	*/
	for (int i = 0; i < Height*Width; i++)
	{
		bytesRGBOUTEND[3*i]	= bytesTemp[i+Height*Width*2];
		bytesRGBOUTEND[3*i+1] = bytesTemp[i+Height*Width];
		bytesRGBOUTEND[3*i+2] = bytesTemp[i];
	}
}

/*-----------------------------------SUBSAMPLING-------------------------------------
	 
	Block Size: Number of pixels we are looking at each iteration of sampling

	Y, U & V  : the number of samples to be considered in each block

	Set "0" at all the pixels which are not a part of sub sampling along Y, U and V

--------------------------------------------------------------------------------------*/
void MyImage::SubSample()
{
	// totalY number of pixels in the image
	long int pixelArrayLen = Height * Width, i = 0;
	
	// Number of pixels we are looking at in each iteration of SUB SAMPLING along each of Y U V
	double blockYSize = pow(2.0, Y);
	double blockUSize = pow(2.0, U);
	double blockVSize = pow(2.0, V);
	
	//	Sampling Y : Set every pixel to zero except the first one in every block
	i = 0;
	while(i < pixelArrayLen && i >= 0 && Y != 0)
	{
		for(int blockIndex = i + 1; blockIndex < (i+(int)blockYSize); blockIndex++)
		{
			if(blockIndex < 3*pixelArrayLen)
			{
				floatsYUVSS[blockIndex] = 0;
				floatsYUVUS[blockIndex] = 0;
			}
		}
		
		i += (int)blockYSize;
	}
	
	// Sampling U
	i = pixelArrayLen;
	while(i < 2*pixelArrayLen && i >= 0 && U != 0)
	{
		for(int blockIndex = i + 1; blockIndex < (i+(int)blockUSize); blockIndex++)
		{
			if(blockIndex < 3*pixelArrayLen)
			{
				floatsYUVSS[blockIndex] = 0;
				floatsYUVUS[blockIndex] = 0;
			}
		}
		
		i += (int)blockUSize;
	}
	
	// Sampling V
	i = 2*pixelArrayLen;
	while(i < 3*pixelArrayLen && i >= 0 && V != 0)
	{
		for(int blockIndex = i + 1; blockIndex < (i+(int)blockVSize); blockIndex++)
		{
			if(blockIndex < 3*pixelArrayLen)
			{
				floatsYUVSS[blockIndex] = 0;
				floatsYUVUS[blockIndex] = 0;
			}
		}
		
		i += (int)blockVSize;
	}
}

/*------------------UPSAMPLING------------------------
 
			emptySlotCount (3)
			  ____|____
			  |	  |	  |
		------------------------------------
		|x	|?	|??	|???|z	|?	|??	|???|p	|
		------------------------------------
		  |			      |
	emptyBlockStart    emptyBlockEnd   

	diffEnd_Start = z - x
 
	stepAmount = diffEnd_Start / emptySlotCount+1
 
	? = x + stepAmount

	?? = ? + stepAmount

	??? = ?? + stepAmount
-------------------------------------------------------*/
void MyImage::UpSample()
{
	// totalY number of pixels in the image
	int pixelArrayLen = Height * Width, i = 0;
	int emptyBlockEnd, emptyBlockStart, emptySlotCount;
	float diffEnd_Start, stepAmount;
	
	// Number of pixels we are looking at in each iteration of SUB SAMPLING along each of Y U V
	double blockYSize = pow(2.0, Y);
	double blockUSize = pow(2.0, U);
	double blockVSize = pow(2.0, V);
	
	// Linear interpolation
	
	// Upsampling Y
	i = 0;
	while(i < pixelArrayLen  && i >= 0 && Y != 0)
	{
		emptyBlockEnd = i + (int)blockYSize;
		emptyBlockStart = i;
		emptySlotCount = (int)blockYSize;

		if(emptyBlockEnd > pixelArrayLen-1)
		{
			diffEnd_Start = emptySlotCount;
			stepAmount = 0;
		}
		else
		{
			diffEnd_Start = floatsYUVUS[emptyBlockEnd] - floatsYUVUS[emptyBlockStart];
			stepAmount = diffEnd_Start / emptySlotCount;
		}
		
		for(int k = emptyBlockStart+1; k <= emptyBlockEnd-1; k++)
		{
			if(floatsYUV[k] != 0 && k <= pixelArrayLen-1)
			   floatsYUVUS[k] = floatsYUVUS[k-1] + stepAmount;
		}
		
		i += (int)blockYSize;
	}
	
	// Upsampling U
	i = pixelArrayLen;
	while(i < 2*pixelArrayLen && i >= 0 && U != 0)
	{
		emptyBlockEnd = i + (int)blockUSize;
		emptyBlockStart = i;
		emptySlotCount = (int)blockUSize;

		if(emptyBlockEnd > 2*pixelArrayLen-1)
		{
			diffEnd_Start = emptySlotCount;
			stepAmount = 0;
		}
		else
		{
			diffEnd_Start = floatsYUVUS[emptyBlockEnd] - floatsYUVUS[emptyBlockStart];
			stepAmount = diffEnd_Start / emptySlotCount;
		}
		
		for(int k = emptyBlockStart+1; k <= emptyBlockEnd-1; k++)
		{
			if(floatsYUV[k] != 0 && k <= pixelArrayLen-1)
				floatsYUVUS[k] = floatsYUVUS[k-1] + stepAmount;
		}
		
		i += (int)blockUSize;
	}
	
	// Upsampling V
	i = 2*pixelArrayLen;
	while(i < (3*pixelArrayLen-blockVSize) && i >= 0 && V != 0)
	{
		emptyBlockEnd = i + (int)blockVSize;
		emptyBlockStart = i;
		emptySlotCount = (int)blockVSize;

		if(emptyBlockEnd > 3*pixelArrayLen-1)
		{
			diffEnd_Start = emptySlotCount;
			stepAmount = 0;
		}
		else
		{
			diffEnd_Start = floatsYUVUS[emptyBlockEnd] - floatsYUVUS[emptyBlockStart];
			stepAmount = diffEnd_Start / emptySlotCount;
		}
		
		for(int k = emptyBlockStart+1; k <= emptyBlockEnd-1; k++)
		{
			if(floatsYUV[k] != 0 && k <= pixelArrayLen-1)
				floatsYUVUS[k] = floatsYUVUS[k-1] + stepAmount;
		}
		
		i += (int)blockVSize;
	}
}

void MyImage::Quantize()
{
	int intervalSize = 0;
	int extra = 0, prev_interval = 0, next_interval = 0, val = 0;
	
	if(Q != 0)
	{
		intervalSize = MAX_COLOR_VARIATIONS / Q;

		for(int i = 0; i < Height*Width; i++)
		{
			extra = bytesRGBOUTEND[3*i+2]%intervalSize;
			if(extra < intervalSize)
			{
				bytesRGBOUTEND[3*i+2] = bytesRGBOUTEND[3*i+2] - extra;
			}
			else
			{
				prev_interval = bytesRGBOUTEND[3*i+2] - extra;
				next_interval = prev_interval + intervalSize;
				val = next_interval - bytesRGBOUTEND[3*i+2];

				bytesRGBOUTEND[3*i+2] += val;
			}

			extra = bytesRGBOUTEND[3*i+1]%intervalSize;
			if(extra < intervalSize)
			{
				bytesRGBOUTEND[3*i+1] = bytesRGBOUTEND[3*i+1] - extra;
			}
			else
			{
				prev_interval = bytesRGBOUTEND[3*i+1] - extra;
				next_interval = prev_interval + intervalSize;
				val = next_interval - bytesRGBOUTEND[3*i+1];

				bytesRGBOUTEND[3*i+1] += val;
			}


			extra = bytesRGBOUTEND[3*i]%intervalSize;
			if(extra < intervalSize)
			{
				bytesRGBOUTEND[3*i] = bytesRGBOUTEND[3*i] - extra;
			}
			else
			{
				prev_interval = bytesRGBOUTEND[3*i] - extra;
				next_interval = prev_interval + intervalSize;
				val = next_interval - bytesRGBOUTEND[3*i];

				bytesRGBOUTEND[3*i] += val;
			}
		}
	}
}


void MyImage::UpSampleSurround()
{
	int h = 0, w = 0;

	//memory allocated for  elements of each column.
	floatsYBlock = new float *[Height];
	floatsUBlock = new float *[Height];
	floatsVBlock = new float *[Height];

	for( int i = 0 ; i < Height ; i++ )
	{
		floatsYBlock[i] = new float[Width];
		floatsUBlock[i] = new float[Width];
		floatsVBlock[i] = new float[Width];
	}
	
	// Linear array into 2D array of values
	for(int i = 0; i < Height*Width; i++)
	{
		if(w < Width)
		{
			floatsYBlock[h][w] = floatsYUVUS[i];
			floatsUBlock[h][w] = floatsYUVUS[i+Height*Width];
			floatsVBlock[h][w] = floatsYUVUS[i+Height*Width*2];
			
			w++;
		}
		else
		{
			h++;
			w = 0;
			
			floatsYBlock[h][w] = floatsYUVUS[i];
			floatsUBlock[h][w] = floatsYUVUS[i+Height*Width];
			floatsVBlock[h][w] = floatsYUVUS[i+Height*Width*2];
			
			w++;
		}
	}

	float totalY = 0, totalU = 0, totalV = 0, surroundingValCount = 0;
	for(int row = 0; row < Height; row++)
	{
		for(int col = 0; col < Width; col++)
		{
			totalY = 0;
			surroundingValCount = 0;
			
			// If the current pixel is non zero in original YUV and is zero in sampled YUV, then upsample it.
				
			/* Max totalY is
				
				totalY = floatsYBlock[i][j+1] + floatsYBlock[i][j-1] +
				floatsYBlock[i+1][j] + floatsYBlock[i-1][j] +
				floatsYBlock[i-1][j-1] + floatsYBlock[i-1][j+1] +
				floatsYBlock[i+1][j-1] + floatsYBlock[i+1][j+1];
			*/

			// LEFT & RIGHT
			if(col == 0)
			{
				totalY = floatsYBlock[row][col+1];	// Only right
				totalU = floatsUBlock[row][col+1];	// Only right
				totalV = floatsVBlock[row][col+1];	// Only right
				surroundingValCount++;
			}
			else if(col == Width-1)
			{
				totalY = floatsYBlock[row][col-1];	// Only left
				totalU = floatsUBlock[row][col-1];	// Only left
				totalV = floatsVBlock[row][col-1];	// Only left
				surroundingValCount++;
			}
			else
			{
				totalY = floatsYBlock[row][col+1] + floatsYBlock[row][col-1];		// Both left and right
				totalU = floatsUBlock[row][col+1] + floatsUBlock[row][col-1];		// Both left and right
				totalV = floatsVBlock[row][col+1] + floatsVBlock[row][col-1];		// Both left and right
				surroundingValCount += 2;
			}
			
			// TOP & BOTTOM
			if(row == 0)
			{
				totalY += floatsYBlock[row+1][col];	// Only bottom
				totalU += floatsUBlock[row+1][col];	// Only bottom
				totalV += floatsVBlock[row+1][col];	// Only bottom
				surroundingValCount++;
			}
			else if(row == Height-1)
			{
				totalY += floatsYBlock[row-1][col];	// Only top
				totalU += floatsUBlock[row-1][col];	// Only top
				totalV += floatsVBlock[row-1][col];	// Only top
				surroundingValCount++;
			}
			else
			{
				totalY += floatsYBlock[row+1][col] + floatsYBlock[row-1][col];		// Both top and bottom
				totalU += floatsUBlock[row+1][col] + floatsUBlock[row-1][col];		// Both top and bottom
				totalV += floatsVBlock[row+1][col] + floatsVBlock[row-1][col];		// Both top and bottom
				surroundingValCount += 2;
			}
			
			// LEFT TOP & RIGHT TOP
			if(col != 0 && row != 0)
			{
				totalY += floatsYBlock[row-1][col-1];		// Left top
				totalU += floatsUBlock[row-1][col-1];		// Left top
				totalV += floatsVBlock[row-1][col-1];		// Left top
				surroundingValCount++;
			}

			if(col != (Width-1) && row != 0)
			{
				totalY += floatsYBlock[row-1][col+1];		// Right top
				totalU += floatsUBlock[row-1][col+1];		// Right top
				totalV += floatsVBlock[row-1][col+1];		// Right top
				surroundingValCount++;
			}
			
			// LEFT BOTTOM & RIGHT BOTTOM
			if(col != 0 && row != (Height-1))		// Left bottom
			{
				totalY += floatsYBlock[row+1][col-1];
				totalU += floatsUBlock[row+1][col-1];
				totalV += floatsVBlock[row+1][col-1];
				surroundingValCount++;
			}

			if(col != (Width-1) && row != (Height-1))// Right bottom
			{
				totalY += floatsYBlock[row+1][col+1];
				totalU += floatsUBlock[row+1][col+1];
				totalV += floatsVBlock[row+1][col+1];
				surroundingValCount++;
			}	

			if(surroundingValCount == 0)
				continue;

			if(floatsYUV[row*Width+col] !=0)
			{
				if(floatsYBlock[row][col] == 0)
					floatsYBlock[row][col] = totalY / surroundingValCount;

				if(floatsUBlock[row][col] == 0)
					floatsUBlock[row][col] = totalU / surroundingValCount;

				if(floatsVBlock[row][col] == 0)
					floatsVBlock[row][col] = totalV / surroundingValCount;
			}
		}
	}

	for(int row = 0; row < Height; row++)
	{
		for(int col = 0; col < Width; col++)
		{
			floatsYUVUS[(row*Width)+col] = floatsYBlock[row][col];
			floatsYUVUS[(Height*Width)+(row*Width)+col] = floatsUBlock[row][col];
			floatsYUVUS[(Height*Width*2)+(row*Width)+col] = floatsVBlock[row][col];
		}
	}
}

/* Convert a color image into a gray scale version 

	newR = newG = newB = (0.3 * oldR) + (0.59 * oldG) + (0.11 * oldB);
*/
void MyImage::GrayScale()
{
	// Storing Format : [BGR][BGR]....[BGR]
	bytesRGBOUTGRAYSCALE = new unsigned char[Width*Height*3];

	for (int i = 0; i < Height*Width; i++)
	{
		bytesRGBOUTGRAYSCALE[3*i] = (0.11 * bytesRGBOUTSTART[3*i]) + (0.59 * bytesRGBOUTSTART[3*i+1]) + (0.3 * bytesRGBOUTSTART[3*i+2]);
		bytesRGBOUTGRAYSCALE[3*i+1]	= bytesRGBOUTGRAYSCALE[3*i];
		bytesRGBOUTGRAYSCALE[3*i+2]	= bytesRGBOUTGRAYSCALE[3*i];
	}
}


void MyImage::FloydSteinberg()
{
	int h = 0, w = 0;
	
	bytesRGBOUTFLOYD = new unsigned char[3*Height*Width];

	//memory allocated for  elements of each column.
	bytesRBlock = new float *[Height];
	bytesGBlock = new float *[Height];
	bytesBBlock = new float *[Height];
	
	bytesROutBlock = new unsigned char *[Height];
	bytesGOutBlock = new unsigned char *[Height];
	bytesBOutBlock = new unsigned char *[Height];

	for( int i = 0 ; i < Height ; i++ )
	{
		bytesRBlock[i] = new float[Width];
		bytesGBlock[i] = new float[Width];
		bytesBBlock[i] = new float[Width];

		bytesROutBlock[i] = new unsigned char[Width];
		bytesGOutBlock[i] = new unsigned char[Width];
		bytesBOutBlock[i] = new unsigned char[Width];
	}
	
	// Linear array into 2D array of gray scale values
	for(int i = 0; i < Height*Width; i++)
	{
		if(w < Width)
		{
			bytesRBlock[h][w] = (float)(((float)bytesRGBOUTGRAYSCALE[3*i+2]) / (float)255);
			bytesGBlock[h][w] = (float)(((float)bytesRGBOUTGRAYSCALE[3*i+1]) / (float)255);
			bytesBBlock[h][w] = (float)(((float)bytesRGBOUTGRAYSCALE[3*i]) / (float)255);
			
			w++;
		}
		else
		{
			h++;
			w = 0;
			
			bytesRBlock[h][w] = (float)(((float)bytesRGBOUTGRAYSCALE[3*i+2]) / (float)255);
			bytesGBlock[h][w] = (float)(((float)bytesRGBOUTGRAYSCALE[3*i+1]) / (float)255);
			bytesBBlock[h][w] = (float)(((float)bytesRGBOUTGRAYSCALE[3*i]) / (float)255);
			
			w++;
		}
	}
	
	//Print();

	float newGrayValue = 0, quantError = 0;
	for(int row = 0; row < Height; row++)
	{
		for(int col = 0; col < Width; col++)
		{
			newGrayValue = FindNewPixel(bytesRBlock[row][col]);
			quantError = bytesRBlock[row][col] - newGrayValue;
			
			if(newGrayValue == 0)
			{
				bytesROutBlock[row][col] = 0;
				bytesGOutBlock[row][col] = 0;
				bytesBOutBlock[row][col] = 0;
			}
			else if(newGrayValue == 1)
			{
				bytesROutBlock[row][col] = 255;
				bytesGOutBlock[row][col] = 255;
				bytesBOutBlock[row][col] = 255;
			}

			if((row+1) <= (Height-1))
			{
				bytesRBlock[row+1][col] += (7/16) * quantError;
				bytesGBlock[row+1][col] += (7/16) * quantError; 
				bytesBBlock[row+1][col] += (7/16) * quantError; 
			}

			if((row-1) >= 0 && (col+1) <= (Width-1))
			{
				bytesRBlock[row-1][col+1] += (3/16) * quantError;
				bytesGBlock[row-1][col+1] += (3/16) * quantError; 
				bytesBBlock[row-1][col+1] += (3/16) * quantError; 
			}

			if((col+1) <= (Width-1))
			{
				bytesRBlock[row][col+1] += (5/16) * quantError;
				bytesGBlock[row][col+1] += (5/16) * quantError;
				bytesBBlock[row][col+1] += (5/16) * quantError;
			}
			
			if((row+1) <= (Height-1) && (col+1) <= (Width-1))
			{
				bytesRBlock[row+1][col+1] += (1/16) * quantError;
				bytesGBlock[row+1][col+1] += (1/16) * quantError; 
				bytesBBlock[row+1][col+1] += (1/16) * quantError;
			}
		}
	}

	/* Convert Storing Format:
		FROM: [RRRRRR.....R] Block
			  [GGGGGG.....G] Block
			  [BBBBBB.....B] Block
		TO	: [BGR][BGR].....[BGR]
	*/
	for(int row = 0; row < Height; row++)
	{
		for(int col = 0; col < Width; col++)
		{
			bytesRGBOUTFLOYD[3*(row*Width+col)]	= bytesROutBlock[row][col];
			bytesRGBOUTFLOYD[3*(row*Width+col)+1] = bytesGOutBlock[row][col];
			bytesRGBOUTFLOYD[3*(row*Width+col)+2] = bytesBOutBlock[row][col];
		}
	}
}

float MyImage::FindNewPixel(float oldGrayValue)
{
	if(oldGrayValue < 0.5)
		return 0;
	else
		return 1;
}

void MyImage::AverageDithering()
{
	int totalR = 0, totalG = 0, totalB = 0;
	unsigned char avgR = 0, avgG = 0, avgB = 0;

	bytesRGBOUTAVG = new unsigned char[3*Height*Width];

	for (int i = 0; i < Height*Width; i++)
	{
		totalR += bytesRGBOUTGRAYSCALE[3*i];
		totalG += bytesRGBOUTGRAYSCALE[3*i+1];
		totalB += bytesRGBOUTGRAYSCALE[3*i+2];
	}

	avgR = totalR / (Height*Width);
	avgG = totalG / (Height*Width); 
	avgB = totalB / (Height*Width); 

	for (int i = 0; i < Height*Width; i++)
	{
		if(bytesRGBOUTGRAYSCALE[3*i] >= avgB)
			bytesRGBOUTAVG[3*i] = 255;
		else
			bytesRGBOUTAVG[3*i] = 0;

		if(bytesRGBOUTGRAYSCALE[3*i+1] >= avgG)
			bytesRGBOUTAVG[3*i+1] = 255;
		else
			bytesRGBOUTAVG[3*i+1] = 0;

		if(bytesRGBOUTGRAYSCALE[3*i+2] >= avgR)
			bytesRGBOUTAVG[3*i+2] = 255;
		else
			bytesRGBOUTAVG[3*i+2] = 0;
	}
}