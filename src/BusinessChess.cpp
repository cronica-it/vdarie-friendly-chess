// BusinessChess.cpp : Defines the entry point for the application.
//
 
#include "stdafx.h"
#include "resource.h"

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <io.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "Structures.h"
extern int agresivity;
extern int MoveToDoMoveValue;
extern int MoveToDoTa;
extern int zerowindow;
int nzerowindow = 0;
int okzerowindow = 0;
  
extern int tablaRoot[120];
extern int BestVal;

extern HWND mainhwnd;

BOOL CALLBACK PawnDlgProc( HWND hwnd, unsigned msg, UINT wparam, LONG lparam );

ATOM RegisterClassEx(

CONST WNDCLASSEX *lpwcx
	// address of structure with class data
);
#define MAX_LOADSTRING 100
#define ICONSIZE 64

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

struct bmp_header
   {
   long  file_length;
   long  rez;
   long  image_offset;
   long  header_size;
   long  width;
   long  heigth;
   short nb_planes;
   short bites_per_pixel;
   long  compression_type;
   long  bitmap_length;
   long  Hresolution;
   long  Vresolution;
   long  nb_colors;
   long  main_colors;
   long  palette[256 ];
   } bmp_input;


struct Opt Options;

char currentDirectory[2000];

int nModels = 4;
char modelList[4][20] = {"model4.bmp","model1.bmp","model2.bmp","model3.bmp"};


BITMAP bitmap_input;

FILE *f0,*f1, *f3, *fpgn;

int PrecSqMouse = 0;

char rezultat[10] ;

int sumatimpialb;
int sumatimpinegru;

int nrmutari;

int timpmediualb;
int timpmediunegru;

int nodesmutarialb;
int nodesmutarinegru;

int nodesmediualb;
int nodesmediunegru;

extern int  findopening;
extern int  backmove;

extern int  indExchange;

extern int  buttonPressed;
extern char moveMessage[60] ;
extern char openingMessage[60] ;

extern int  tabla[120];
extern int  tabla0[120];
extern int  ksup;
extern int  lgMax;
extern int  lgMaxKmax;
extern time_t rawtimestart, rawtime0, rawtime1;
extern int  gameDepth;

extern int who; // who is to move who == 0 (white)   who == 1(black)

extern long nodes;

extern char stime[20];

extern int endOfGame;

extern int BlackToMove;
extern int WhiteToMove;
extern int savedKmax;
extern int MiMa2;

extern struct MoveLong CurrentMove;



extern int SchemaOptimalMove;
extern int indexOptimalMove;
extern int indexBaseLayer;
extern int nbBaseLayer;
extern int OptimalMoveMoveValue;
extern int saveKsup;
extern int limDepth;
extern int endOfBranch;
extern int iDepth;
extern int saveTreeDepth;
extern int algorithm; // == 0 clasic, == 1 is easy play

int thinkingxx;
int indRedraw = 1;

char bufimg[2000];

char bu[100];

int setup = 0;
int setup1 = 0;
int pieceType = -1;
int bellOnOff= 1;

int xc,yc;
int   maxx;    /* maximum coloane pe ecran */
int   maxy;    /* maximum linii   pe ecran */


int xmin,xmax,ymin,ymax,gmax,gmay;

int w3,w, sh, ehy0,ehx0, h;

int begin_dates;

int nBy,nRotStoc;

int lastDrawnBoard[120];

int sta;

RECT Rxy;


HDC hdc;                 /* local device context */
int oldDc;               /* save context */

HBRUSH hBrush, hOldBrush;    

// Bitmap Header Definition 

/*
typedef struct tagBITMAP
  {
    LONG        bmType;
    LONG        bmWidth;
    LONG        bmHeight;
    LONG        bmWidthBytes;
    WORD        bmPlanes;
    WORD        bmBitsPixel;
    LPVOID      bmBits;
  } BITMAP,
*/

int SQSIZE;
int SZSCALE ;
int SQBUTTON;

int nMaxIcons;
BITMAP IconBitmap[200]; // for max 200 icons

int SqSize = SQSIZE;
int IconBits[3*ICONSIZE*ICONSIZE*200]; // 3 colors  of SqSize x SqSize icons for 200 icons


int X0Board ;
int Y0Board ;


// Thinking Area Parameters
int X0Thinking ;
int Y0Thinking ;

int ThWidth;
int ThHeight;

// Options Area

int X0Options ;
int Y0Options ;

int OptWidth;
int OptHeight;

// Screen rezolution
int cx, cy;
int ax,ay; // scaling factors for drawings

// Move List Area

int X0MoveList ;
int Y0MoveList ;

int MvLstWidth;
int MvLstHeight;

// Taken Pieces Area
int   X0Taken ;
int   Y0Taken ;
int   TaWidth ;
int   TaHeight;


// Messages Area

int   X0Buttons;
int   Y0Buttons;

int   ButtWidth;
int   ButtHeight;

int SqFromActive;
int SqToActive;
int SqMouse;

int IconSet = 1;
int nrIconSet = 1;

extern int WhiteIsMan;
extern int BlackIsMan;
extern int noTime;

// Forward - declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void SaveGame(HWND hWnd);
int GenerateEndGameRepository();
int ReadRepositoryEnd(int ); 

int image_read(char bu[], int nIcon, int IconSet);

int MyClearWind(void);
int MyBarRect(int x1a,int y1a,int x2b,int y2b,COLORREF co1);
int MyBarRect1(int x1a,int y1a,int x2b,int y2b,COLORREF  co1);
int MyTextOut(int x1a, int y1a, char *buf1, COLORREF  co1);

int MyLine(int x1a, int y1a, int x2b, int y2b, COLORREF  co2);
int MyCircle(int xcentru,int ycentru,int raza, COLORREF col1);

 int InitChessBoard();
 
int DrawChessPiece(int IconSet,int Square,int pie);
int RedrawChessBoard();

int SaveGame();
int SetKingKing();

int	InitChessBoard();
int	InitGame();
int InitValPiesa(int level);

int ProcSetup();

int	ChangePiecesSet();
int EnterSetupProcedure();
int ExecuteLoadPiecesClear();
int ExecuteLoadPiecesInitial();
int ExecuteStopLoadPieces();
int ExecuteSetupButton();

int TakePiecesSet();
int TakeBackGround();
int HumanPlayWithBlacks();
int SetThinkingOnOff();
int DisplayTakenPiecesOnOff();

int GamePlaying(int mv);
int CallRedrawChessBoard();

int	RedrawThinking();
int	RedrawTakenPieces();
int	RedrawMoveList();
int	RedrawOptions();
int RedrawButtons();
int TestButtons();
int ProcessOpeningFile();

int	ExecutePlayButton();
int	ExecuteBackButton();
int	ExecuteForwardButton();
int	ExecuteForceButton();
int	ExecuteManManButton();
int	ExecuteResignButton();
int	ExecuteCompCompButton();

int RedrawSetupButtons();


int MoveToString(int Move, char moveout[]);


extern struct StackElement Stack[500];
extern struct StackElement2 Stack2[500];
extern int MoveNumber;
extern int treeDepth;
extern int max_attacks;
extern int BestLine[];

extern int kmax;

extern int OptimalMove;

int WINAPI WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nShowCmd
);
LRESULT CALLBACK event(HWND, UINT, WPARAM, LPARAM)
{
    return 0;
}
int WINAPI WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{


extern int tabla0[120];
  
int i;
 SetKingKing(); 
 InitChessBoard();
	GetCurrentDirectory (4195, currentDirectory );

	cx = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CYSCREEN);
	ax = 1;
	ay = 1;

      SQSIZE = 60 ;
  	  
	if(cx != 1600) 
	{
		cx = 1280;
		cy = 960;
		SQSIZE = 54 * ax;
	}



   SZSCALE  = 20 * ax;
   SQBUTTON = 48 * ax;

    SqSize = SQSIZE;


	X0Board  = 10 * ax;
	Y0Board  = 10 * ay -20;

	InitGame();

// Pieces Set 1
	image_read("bking.bmp",0,1);    // read black king icon from a pieces set
	image_read("wpawn.bmp",1,1);    // read white pawn icon
	image_read("wrook.bmp",2,1);    // read white rook icon
	image_read("wknight.bmp",3,1);  // read white knight icon
	image_read("wbishop.bmp",4,1);  // read white bishop icon
	image_read("wqueen.bmp",5,1);   // read white queen icon
	image_read("wking.bmp",6,1);    // read white king icon
	
	image_read("bpawn.bmp",7,1);    // read black pawn icon
	image_read("brook.bmp",8,1);    // read black rook icon
	image_read("bknight.bmp",9,1);  // read black knight icon
	image_read("bbishop.bmp",10,1); // read black bishop icon
	image_read("bqueen.bmp",11,1);  // read black queen icon
	image_read("bking.bmp",12,1);   // read black king icon

// Pieces Set 2
	image_read("bking.bmp",0,2);    // read black king icon from a pieces set
	image_read("wpawn.bmp",1,2);    // read white pawn icon
	image_read("wrook.bmp",2,2);    // read white rook icon
	image_read("wknight.bmp",3,2);  // read white knight icon
	image_read("wbishop.bmp",4,2);  // read white bishop icon
	image_read("wqueen.bmp",5,2);   // read white queen icon
	image_read("wking.bmp",6,2);    // read white king icon
	
	image_read("bpawn.bmp",7,2);    // read black pawn icon
	image_read("brook.bmp",8,2);    // read black rook icon
	image_read("bknight.bmp",9,2);  // read black knight icon
	image_read("bbishop.bmp",10,2); // read black bishop icon
	image_read("bqueen.bmp",11,2);  // read black queen icon
	image_read("bking.bmp",12,2);   // read black king icon

    nrIconSet = 2;

    for(i=0;i<120;i++)
	{
		lastDrawnBoard[i] = tabla0[i];
		if(lastDrawnBoard[i] >= 0)
			lastDrawnBoard[i] = 111;
	}
  
	sta = 0;

// Thinking Area
    X0Thinking = X0Board;
    Y0Thinking = Y0Board + 10 * SQSIZE;
	ThWidth = 20 * SQSIZE + 10;
	ThHeight = 1.5  * SQSIZE;

// Taken Pieces Area
    X0Taken = X0Board;
    Y0Taken = Y0Board + 12 * SQSIZE - 16 * ax;
	TaWidth = 20 * SQSIZE + 10;
	TaHeight = 1 * SQSIZE - 12 * ax;


// Options Area
 
    X0Options = X0Board + 10 * SqSize;
    Y0Options = Y0Board + SQSIZE;
 
    OptWidth  = 9 * SQSIZE + 60;
    OptHeight = 2 * SQSIZE;
 
	Options.nLevel = 10;
	Options.level = 9;
	Options.agresivity = 5;
	Options.nTime = 5;
    Options.mode = 1;
	Options.thinking = 1;
	Options.taken = 1;
	Options.nModel = 0;
	Options.sound = 1;
	Options.blacks = 0;

    ProcessOpeningFile();

   
// Move List Area

    X0MoveList = X0Board + 10 * SQSIZE;
    Y0MoveList = Y0Board + 4 * SQSIZE;

    MvLstWidth  = 4 * SQSIZE;
    MvLstHeight = 5 * SQSIZE;
 
// Buttons Area

    X0Buttons = X0Board + 15 * SQSIZE + 50 ;
    Y0Buttons = Y0Board + 4 * SQSIZE;

    ButtWidth  = 4 * SQSIZE;
    ButtHeight = 5 * SQSIZE;

	SqFromActive = 0;
	SqToActive = 0;
	SqMouse = 0;
 
	thinkingxx = 0;

//	WhiteIsMan = 0;
//	BlackIsMan = 1;



//	InitChessBoard();

	InitGame();

 GenerateEndGameRepository();
 ReadRepositoryEnd(30);

    sta++;

//	setup = 1;

	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_BUSINESSCHESS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

// InitInstance (hInstance, nCmdShow);

    RedrawChessBoard();

	InitValPiesa(Options.level);

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_BUSINESSCHESS);

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
	/*
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_BUSINESSCHESS);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_BUSINESSCHESS;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
*/	
	    // Register the window class.
   	 
WNDCLASSEX wc    = {0};  // make sure all the members are zero-ed out to start
wc.cbSize        = sizeof(wc);
wc.style         = CS_HREDRAW | CS_VREDRAW;
wc.lpfnWndProc   = WndProc;
wc.hInstance     = hInstance;
wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
 
wc.lpszClassName  = "ME";
wc.hIconSm        = LoadIcon(wc.hInstance, IDI_APPLICATION);
ATOM xxx;
xxx = RegisterClassEx(&wc);
if (!xxx) {
    MessageBox(NULL, "Class registration has failed!",
               "Error!", MB_OK | MB_ICONERROR);
    return 0;
}
return 1;
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
HWND hWnd;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; // Store instance handle in our global variable


 
   hWnd = CreateWindowEx(0, "ME",
                  "Friendly Chess - Program de Sah Prietenos Pentru Fiecare Vers. 2023 3.1",
                  WS_OVERLAPPEDWINDOW,
                  0, 0, /* left corner position */
                  cx , cy , /* width & height */
                  NULL, NULL, hInstance, NULL);

   if (!hWnd) return FALSE;

   hInst=  hInstance;

   mainhwnd = hWnd; // set global pointer to main window
//   MyBarRect(1 ,1, cx  , cy , RGB(200,200,200));
 
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
    int ret;
    int i;
	float x;


	PAINTSTRUCT ps;
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
				case ID_FILE_SAVEGAME:
					SaveGame(hWnd);
					break;

				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
        case WM_WINDOWPOSCHANGED:

			for(i=0;i<120;i++)
				{
				lastDrawnBoard[i] = tabla0[i];
				if(lastDrawnBoard[i] >= 0)
					lastDrawnBoard[i] = 111;
				}

	            InvalidateRect(hWnd, NULL, FALSE);    
	
		        return 0L;

        case WM_MOUSEMOVE:

              return(DefWindowProc(hWnd, message, wParam, lParam));

		case 133:

        case WM_SHOWWINDOW:
		    for(i=0;i<120;i++)
				{
				lastDrawnBoard[i] = tabla0[i];
				if(lastDrawnBoard[i] >= 0)
					lastDrawnBoard[i] = 111;
				}

			sta = 1;
//			MyBarRect(1,1,1400, 1000,COLORREF RGB(200,200,200));

			indRedraw = 1;
			InvalidateRect(hWnd, NULL, FALSE);    
			indRedraw = 1;
	
		        return 0L;


//              return(DefWindowProc(hWnd, message, wParam, lParam));


        case WM_LBUTTONDOWN:


			xc = LOWORD(lParam);
            yc = HIWORD(lParam);

			if(setup != 0)
			{
				ProcSetup(); // setup buttons
			}
// Reload xc,yc because ProSetup consum them
			xc = LOWORD(lParam);
            yc = HIWORD(lParam);

			// Test if mouse touch the Chess Board Area

            if(( xc > (X0Board + SQSIZE)) && (xc < (X0Board + 9 * SQSIZE)) &&
				( yc > (Y0Board + SQSIZE)) && (yc < (Y0Board + 9 * SQSIZE))
				   && (indRedraw != 3))
			{
	// Convert from xc,yc to square number
				xc = xc - X0Board - SQSIZE;
				yc = yc - Y0Board - SQSIZE;

				int xcol = (xc - (xc  % SQSIZE ) ) / SQSIZE + 1;
				int xd =(xc % SQSIZE )  ;
				if (xd < 0 || xd > (SQSIZE - 4)) xcol = 0;
			
				int ylin = 8 - (yc -  ( yc % SQSIZE ) ) / SQSIZE ;
				int yd = (yc % SQSIZE )  ;
				
				if (yd < 0 || yd > (SQSIZE - 4)) ylin = 0;

				if (xcol == 0 || ylin == 0) SqMouse = 0;
		        else
					SqMouse = ylin * 10 + 10 + xcol ;

				if((setup > 0) && (setup1 > 0))
				{
					// put a piece on Chess Board
					if(pieceType >= 0)
					{
						tabla[SqMouse] = pieceType;
						tablaRoot[SqMouse] = pieceType;
					}
					else
					{
					//	if(bellOnOff == 1) Beep(700,200);
						SqMouse = 0;
					}
		            InvalidateRect(hWnd, NULL, FALSE);    
	
			        return 0L;

					break;
				}
				if(endOfGame > 0)
				{
					SqMouse = 0;
					// if(bellOnOff == 1) Beep(700,200);
					 PrecSqMouse = 0;
				}
			int xx = PrecSqMouse;
			PrecSqMouse = SqMouse;
	            if(SqMouse > 0 )
				{ // if SqMouse == 0 then invalidate mouse click

					if( SqFromActive > 0 && SqMouse != xx)
						{
					    
							SqToActive = SqMouse;
						}
					else
						{
							if(tabla[SqMouse] > 0)
							 SqFromActive = SqMouse;
							else
							{
							    // if(bellOnOff == 1) Beep(700,200);
								 PrecSqMouse = 0;
								SqFromActive = 0;
							}
						}
				}

				if((SqFromActive > 0) && ( SqToActive > 0))
				{
/*
					if(who == 1)
					{
						BlackIsMan = 1;
						WhiteIsMan = 0;
						BlackToMove = 0;
						WhiteToMove = 1;
					}
					else
					{
						BlackIsMan = 0;
						WhiteIsMan = 1;
						BlackToMove = 1;
						WhiteToMove = 0;
					}
*/
					ret = GamePlaying(2);

					// if ((ret != 1)  && (bellOnOff == 1) ) Beep(900,200);
					PrecSqMouse = 0;
					SqFromActive = 0;
					SqToActive = 0;

				}
	            InvalidateRect(hWnd, NULL, FALSE);    
	
		        return 0L;

			} // end if XC, YC inside Chess Board

			TestButtons();

     if((setup == 0) && (setup1 != 2))
	 {
            // Test if some buttons are pressed
			x = SQBUTTON / 2;
			if(( xc > X0Buttons) && (xc < (X0Buttons + ButtWidth)) &&
				( yc > (Y0Buttons)) && (yc < (Y0Buttons + ButtHeight)))
			{
				// Play Button - Partida Noua
				if((xc > (X0Buttons + 5 * ax )) && (xc < (X0Buttons + 10 * ax + 1 * x)) &&
				   (yc > (Y0Buttons + 10 * ay)) && (yc < (Y0Buttons + 10 * ay + 20 * ay)))
				{
					
					ExecutePlayButton();
				}
				// Back Button
				if((xc > (X0Buttons + 5 * ax + 1 * x)) && 
					(xc < (X0Buttons + 10 * ax + 2 * x)) &&
				   (yc > (Y0Buttons + 40 * ay)) && (yc < (Y0Buttons + 40 * ay + 20 * ay)))
				{
					moveMessage[0] = 0;
					ExecuteBackButton();
				}

				// Forward Button
				if((xc > (X0Buttons + 5 * ax + 1 * x)) && 
					(xc < (X0Buttons + 10 * ax + 2 * x)) &&
				   (yc > (Y0Buttons + 70 * ax)) && (yc < (Y0Buttons + 70 * ay + 20 * ay)))
				{

					ExecuteForwardButton();
				}


				// Force Move Button
				if((xc > (X0Buttons + 5 * ax + 1 * x)) && 
					(xc < (X0Buttons + 10 * ax + 2 * x)) &&
				   (yc > (Y0Buttons + 100 * ay)) && (yc < (Y0Buttons + 100 * ay + 20 * ay)))
				{

					ExecuteForceButton();
				}

				// Play Man vs. Man
				if((xc > (X0Buttons + 10 * ax )) && 
					(xc < (X0Buttons + 10 * ax + 1 * x)) &&
				   (yc > (Y0Buttons + 130 * ay)) && 
				   (yc < (Y0Buttons + 130 * ay + 20 * ay)))
				{
					ExecuteManManButton();
				}
				// SETUP Button
				if((xc > (X0Buttons + 5 * ax )) && (xc < (X0Buttons + 10 * ax+ 1 * x)) &&
				   (yc > (Y0Buttons + 160 * ay )) && (yc < (Y0Buttons + 160 * ay + 20 * ay)))
				{

					ExecuteSetupButton();
				}

				// Play Computer vs. Computer Button
				if((xc > (X0Buttons + 5 * ax )) && (xc < (X0Buttons + 10 * ax+ 1 * x)) &&
				   (yc > (Y0Buttons + 190 * ay)) && (yc < (Y0Buttons + 190 * ay + 20 * ay)))
				{

					ExecuteCompCompButton();
				}

				InvalidateRect(hWnd, NULL, FALSE);    
				// Play Computer vs. Computer Button
				if((xc > (X0Buttons + 5 * ax )) && (xc < (X0Buttons + 10 * ax+ 1 * x)) &&
				   (yc > (Y0Buttons + 220 * ay)) && (yc < (Y0Buttons + 220 * ay + 20 * ay)))
				{

					SaveGame (hWnd );
				}

				InvalidateRect(hWnd, NULL, FALSE);    

		        return 0L;

			} // end if XC, YC inside Commands Area

	 } // if setup == 0
	if(setup1 == 2)
		 setup1 = 0;

	   // Thinking ON/OFF button

	 MyBarRect(X0Options,Y0Options, X0Options + OptWidth, Y0Options + OptHeight,COLORREF RGB(240,240,240));
			if(( xc > X0Thinking + ThWidth - 15 * ax) && 
				(xc < (X0Thinking + ThWidth)) &&
				( yc > (Y0Thinking)) && (yc < (Y0Thinking + 15 * ay)))
			{
				Options.thinking = (Options.thinking + 1) & 1;
				InvalidateRect(hWnd, NULL, FALSE);    
	
		        return 0L;
			}
			 return(DefWindowProc(hWnd, message, wParam, lParam));

        case WM_CREATE:
             hdc    = GetDC(hWnd);
//             GetTextMetrics(hdc, &tm);
//             cxChar = tm.tmAveCharWidth;
//             cyChar = tm.tmHeight+tm.tmExternalLeading;
             
             ReleaseDC(hWnd, hdc);
             return 0L;
             

	    case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...

//			RECT rt;
//			GetClientRect(hWnd, &rt);
//			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);

//			if(indRedraw == 1)
	            RedrawChessBoard();
			indRedraw = 0;

			EndPaint(hWnd, &ps);


			if((moveMessage[0] > 0) && (gameDepth == treeDepth))
			{
//				if(bellOnOff == 1) Beep(900,200);
//				Sleep(5000);
//				MessageBox(hWnd, moveMessage, " MAT APROPIAT ", MB_OK);
//				strcpy(moveMessage,"");
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

//
// Test Buttons
//
int TestButtons()
{
int x;

    // Test if change an option
	if(( xc > X0Options) && (xc < (X0Options + OptWidth)) &&
		( yc > (Y0Options)) && (yc < (Y0Options + OptHeight)))
			{
				// Chess Level
				x = SZSCALE;
				if((xc > (X0Options + 8 * x )) && 
				   (xc < (X0Options + 8 * x + 10 * x)) &&
				   (yc > (Y0Options + 10 * ay)) && (yc < (Y0Options + 25 * ay)))
				{

					Options.level = (int)(xc - X0Options - 8 * x ) / x;
					if(Options.level < 0) Options.level = 0;
					if(Options.level >= 10) Options.level = 9;
					
					InitValPiesa(Options.level);
					noTime = 1;

				}

				// Time Level
				if((xc > (X0Options + 8 * x)) && 
				   (xc < (X0Options + 8 * x + 10 * x)) &&
				   (yc > (Y0Options + 35 * ay)) && (yc < (Y0Options + 50 * ay)))
				{
					Options.nTime = (int)(xc - X0Options - 8 * x) / x;
					if(Options.nTime < 0) Options.nTime = 0;
					if(Options.nTime >= 10) Options.nTime = 9;
					ksup = Options.nTime;
					noTime = 1;
				}
				// Agresivity Level
				if((xc > (X0Options + 8 * x)) && 
				   (xc < (X0Options + 8 * x + 10 * x)) &&
				   (yc > (Y0Options + 50 * ay)) && (yc < (Y0Options + 75 * ay)))
				{
					Options.agresivity = (int)(xc - X0Options - 8 * x) / x;
					if(Options.agresivity < 0) Options.agresivity = 0;
					if(Options.agresivity >= 10) Options.agresivity = 9;
					noTime = 1;
				}
				
				if(indRedraw == 3) return 0; // call from GenTree()

				indRedraw = 1;

	            InvalidateRect(hWnd, NULL, FALSE);    
	
		        return 0L;

		}
	return 0L;
}

int	ExecutePlayButton()
{
int aa;
int i;


	// Computer plays with White pieces
    if(indRedraw == 3) 
	{
		buttonPressed = 1;
		return 1;
	}

	aa = MessageBox(hWnd, " Doriti sa jucati o noua partida ? "," ATENTIE ",  MB_YESNO);

	if( aa != 6) return 1;

	InitChessBoard();

	InitGame();

	SqToActive = 0;
	SqFromActive = 0;

	who = 0;

    sta++;

    for(i=0;i<120;i++)
	{
		lastDrawnBoard[i] = tabla0[i];
		if(lastDrawnBoard[i] >= 0)
			lastDrawnBoard[i] = 111;
	}

    InvalidateRect(hWnd, NULL, FALSE);    

	sta = 1;

    InvalidateRect(hWnd, NULL, FALSE);    

    InvalidateRect(hWnd, NULL, FALSE);    

//	GamePlaying(1);

	return 1;
}


int	ExecuteForceButton()
{
    if(indRedraw == 3) 
	{
		buttonPressed = 2;
		return 1;
	}
	if(endOfGame == 0)
	{
		GamePlaying(999);
	}
	indRedraw = 1;
	return 1;
}

//
// Play Man vs. Man
//
int	ExecuteManManButton()
{
/*
  SetKingKing();
  SetQueenKing();
  SetRookKing();
  SetBishopKing();
  SetKnightKing();
*/
 
  Stack[gameDepth].mateDetected = 0;

  time(&rawtime0);
  rawtime1 = rawtime0;

  WhiteToMove = 1;

  BlackToMove = 0;


  strcpy(moveMessage,"");

  WhiteIsMan = 1;
  BlackIsMan = 1;

  InitGame();

  return 1;
}

int ExecuteSetupButton()
{
	setup = 1;
	setup1 = 0;

	indRedraw = 1;

	sta = 1;

	InvalidateRect(hWnd, NULL, FALSE);    

	indRedraw = 1;

	return 1;
}


int	ExecuteResignButton()
{
	MessageBox( hWnd, " CEDEZ - Totusi putem sa continuam partida", " FELICITARI, ATI CASTIGAT ! ", MB_OK);

    if(indRedraw == 3) 
	{
		buttonPressed = 4;
		return 1;
	}

//	InitChessBoard();

//	InitGame();

	indRedraw = 1;

    sta = 1;
	return 1;
}


int	ExecuteCompCompButton()
{
	int aa;

    if(indRedraw == 3) 
	{
//		if(bellOnOff == 1) Beep(700,200);
		return 1;
	}

	WhiteIsMan = 0;
	BlackIsMan = 0;

//    InitChessBoard();
//    InitGame();

	   for(;;)
	   {
		   aa = GamePlaying(200);
		   if((aa == -1) || ( aa == -2)) break;
	   }

	WhiteIsMan = 1;
	BlackIsMan = 0;

//	InitGame();

	return 1;
}



//
// Call Redraw Chess Board
//

int CallRedrawChessBoard()
{


    InvalidateRect(hWnd, NULL, FALSE);    
   MyBarRect(0 ,0, cx  , cy , RGB(200,200,200));	
//	RedrawChessBoard();
    UpdateWindow(hWnd);

	indRedraw = 1;

	return 0L;

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


//
// Redraw Chess Board
//


int RedrawChessBoard()
{

int k;
int i;


int xa,xb,ya,yb;

if ( sta == 0) return 0;

/*
if ( thinking == 1)
{
	RedrawThinking();
	thinkingxx = 0;
	return 0;
}
*/
 if(sta == 1)
 {				
    if (indRedraw < 3)
	{
 //		MyBarRect(1,1,1400, 1000,COLORREF RGB(200,200,200));

		    for(i=0;i<120;i++)
				{
				lastDrawnBoard[i] = tabla0[i];
				if(lastDrawnBoard[i] >= 0)
					lastDrawnBoard[i] = 111;
				}


	HBITMAP hbm = (HBITMAP) LoadImage(hInst, modelList[Options.nModel], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if(hbm)
		{
		 hBrush = CreatePatternBrush(hbm);
		 DeleteObject(hbm);

		 xa = 1;
		 ya = 1;
		 xb = cx;
		 yb = cy;
  
		 SetBkColor(hdc,RGB(200,220,200));

		 SelectObject(hdc, hBrush);

		 Rectangle(hdc,xa+ 120,ya - 20,xb+120,yb - 20);

		 DeleteObject(hBrush);

		}
//	  MyBarRect(X0Options,Y0Options, X0Options + OptWidth, Y0Options + OptHeight,COLORREF RGB(240,240,255));

//	  MyBarRect(X0MoveList,Y0MoveList, X0MoveList + MvLstWidth, Y0MoveList + MvLstHeight, RGB(200,240,200));
//	  MyBarRect(X0Buttons,Y0Buttons, X0Buttons + ButtWidth, Y0Buttons + ButtHeight, RGB(200,200,255));

	}

	sta++;

 }


			MyTextOut(X0Board + 1 * SqSize + SqSize / 2, Y0Board  + SqSize - SqSize / 3, "A", RGB(0,0,0));
			MyTextOut(X0Board + 2 * SqSize + SqSize / 2, Y0Board  + SqSize - SqSize / 3, "B", RGB(0,0,0));
			MyTextOut(X0Board + 3 * SqSize + SqSize / 2, Y0Board  + SqSize - SqSize / 3, "C", RGB(0,0,0));
			MyTextOut(X0Board + 4 * SqSize + SqSize / 2, Y0Board  + SqSize - SqSize / 3, "D", RGB(0,0,0));
			MyTextOut(X0Board + 5 * SqSize + SqSize / 2, Y0Board  + SqSize - SqSize / 3, "E", RGB(0,0,0));
			MyTextOut(X0Board + 6 * SqSize + SqSize / 2, Y0Board  + SqSize - SqSize / 3, "F", RGB(0,0,0));
			MyTextOut(X0Board + 7 * SqSize + SqSize / 2, Y0Board  + SqSize - SqSize / 3, "G", RGB(0,0,0));
			MyTextOut(X0Board + 8 * SqSize + SqSize / 2, Y0Board  + SqSize - SqSize / 3, "H", RGB(0,0,0));

			MyTextOut(X0Board + 1 * SqSize + SqSize / 2, Y0Board  + 9 * SqSize + SqSize / 6, "A", RGB(0,0,0));
			MyTextOut(X0Board + 2 * SqSize + SqSize / 2, Y0Board  + 9 * SqSize + SqSize / 6, "B", RGB(0,0,0));
			MyTextOut(X0Board + 3 * SqSize + SqSize / 2, Y0Board  + 9 * SqSize + SqSize / 6, "C", RGB(0,0,0));
			MyTextOut(X0Board + 4 * SqSize + SqSize / 2, Y0Board  + 9 * SqSize + SqSize / 6, "D", RGB(0,0,0));
			MyTextOut(X0Board + 5 * SqSize + SqSize / 2, Y0Board  + 9 * SqSize + SqSize / 6, "E", RGB(0,0,0));
			MyTextOut(X0Board + 6 * SqSize + SqSize / 2, Y0Board  + 9 * SqSize + SqSize / 6, "F", RGB(0,0,0));
			MyTextOut(X0Board + 7 * SqSize + SqSize / 2, Y0Board  + 9 * SqSize + SqSize / 6, "G", RGB(0,0,0));
			MyTextOut(X0Board + 8 * SqSize + SqSize / 2, Y0Board  + 9 * SqSize + SqSize / 6, "H", RGB(0,0,0));

			
			MyTextOut(X0Board + 1 * SqSize - SqSize / 3, Y0Board  + 1 * SqSize + SqSize / 3, "8", RGB(0,0,0));
			MyTextOut(X0Board + 9 * SqSize + SqSize / 4, Y0Board  + 1 * SqSize + SqSize / 3, "8", RGB(0,0,0));

			MyTextOut(X0Board + 1 * SqSize - SqSize / 3, Y0Board  + 2 * SqSize + SqSize / 3, "7", RGB(0,0,0));
			MyTextOut(X0Board + 9 * SqSize + SqSize / 4, Y0Board  + 2 * SqSize + SqSize / 3, "7", RGB(0,0,0));

			MyTextOut(X0Board + 1 * SqSize - SqSize / 3, Y0Board  + 3 * SqSize + SqSize / 3, "6", RGB(0,0,0));
			MyTextOut(X0Board + 9 * SqSize + SqSize / 4, Y0Board  + 3 * SqSize + SqSize / 3, "6", RGB(0,0,0));

			MyTextOut(X0Board + 1 * SqSize - SqSize / 3, Y0Board  + 4 * SqSize + SqSize / 3, "5", RGB(0,0,0));
			MyTextOut(X0Board + 9 * SqSize + SqSize / 4, Y0Board  + 4 * SqSize + SqSize / 3, "5", RGB(0,0,0));

			MyTextOut(X0Board + 1 * SqSize - SqSize / 3, Y0Board  + 5 * SqSize + SqSize / 3, "4", RGB(0,0,0));
			MyTextOut(X0Board + 9 * SqSize + SqSize / 4, Y0Board  + 5 * SqSize + SqSize / 3, "4", RGB(0,0,0));

			MyTextOut(X0Board + 1 * SqSize - SqSize / 3, Y0Board  + 6 * SqSize + SqSize / 3, "3", RGB(0,0,0));
			MyTextOut(X0Board + 9 * SqSize + SqSize / 4, Y0Board  + 6 * SqSize + SqSize / 3, "3", RGB(0,0,0));

			MyTextOut(X0Board + 1 * SqSize - SqSize / 3, Y0Board  + 7 * SqSize + SqSize / 3, "2", RGB(0,0,0));
			MyTextOut(X0Board + 9 * SqSize + SqSize / 4, Y0Board  + 7 * SqSize + SqSize / 3, "2", RGB(0,0,0));


			MyTextOut(X0Board + 1 * SqSize - SqSize / 3, Y0Board  + 8 * SqSize + SqSize / 3, "1", RGB(0,0,0));
			MyTextOut(X0Board + 9 * SqSize + SqSize / 4, Y0Board  + 8 * SqSize + SqSize / 3, "1", RGB(0,0,0));

			if((indRedraw < 3) || (gameDepth == treeDepth))
			{

				for(k=21;k<99;k++)
				{
					if(tabla[k] < 0) continue;

                if(lastDrawnBoard[k] != tablaRoot[k])
						DrawChessPiece(IconSet,k,tablaRoot[k]);
					lastDrawnBoard[k] = tablaRoot[k];
				}
			

				MyLine(X0Board + SQSIZE,Y0Board  + SQSIZE,X0Board + SQSIZE,Y0Board  + 9 * SQSIZE, RGB(0,0,0));
				MyLine(X0Board + SQSIZE,Y0Board  + 9 * SQSIZE,X0Board + 9 * SQSIZE,Y0Board  + 9 * SQSIZE, RGB(0,0,0));
				MyLine(X0Board + 9 * SQSIZE,Y0Board  + 9 * SQSIZE,X0Board + 9 * SQSIZE,Y0Board  + SQSIZE, RGB(0,0,0));
				MyLine(X0Board + 9 * SQSIZE,Y0Board  + SQSIZE,X0Board + SQSIZE,Y0Board  + SQSIZE, RGB(0,0,0));

			}
            if(indRedraw <= 3)
			{
				if((gameDepth == treeDepth) || (sta == 1))
				{ 
                    if(setup == 0)
					{
						RedrawButtons();
					}
					else
					{
						RedrawSetupButtons();
					}
					RedrawTakenPieces();
					RedrawMoveList();
					RedrawOptions();
				}
				RedrawThinking();
			}
			indRedraw = 1;
   return 1;

}
//
// Draw a Piece Icon on Chess Board
//

int DrawChessPiece(int IconSet, int square, int pie)
{
			int ind0,ind;
			int i,j,i1,j1,r,g,b, rgbBk;

            int x,y;
            double dSq;

			i1 = (square % 10) - 1; // column
			j1 = (square / 10) - 2; // line

			if ((( i1 + j1 ) % 2) == 0)
					{
						MyBarRect1((i1 + 1) * SqSize + X0Board , (7 - j1 + 1) * SqSize + Y0Board , 
							(i1 + 2) * SqSize + X0Board, (7 - j1 + 2) * SqSize + Y0Board , RGB(170,170,170));					
					}
			else
					{
						MyBarRect1((i1 + 1) * SqSize + X0Board, (7 - j1 + 1) * SqSize + Y0Board , 
							(i1 + 2) * SqSize + X0Board, (7 - j1 + 2) * SqSize + Y0Board , RGB(220,220,220));					
					}
			if(pie == 0) return 1;
		

			dSq = (double)SqSize/(double)ICONSIZE ;
			ind0 = IconSet * ICONSIZE * ICONSIZE * 3 * 13 + ICONSIZE * ICONSIZE * 3 * pie;
			ind = ind0 + ICONSIZE * 3 * 2 + 3 * 2 ; // a point near corner of icon not just on edge

			rgbBk = RGB((int)IconBits[ind], (int)IconBits[ind + 1], (int)IconBits[ind + 2]);
			
			for(j = 0; j< ICONSIZE; j++)
				{
				 for(i= 0;i<ICONSIZE;i++)
				 {
		             ind = ind0 + j * 3 * ICONSIZE + i * 3;
					 r = (int)IconBits[ind];
					 g = (int)IconBits[ind + 1];
					 b = (int)IconBits[ind + 2];
					 x = (int)(X0Board  + dSq * ( ICONSIZE * (i1 + 1) + i));
					 y = (int)(Y0Board + dSq * ( ICONSIZE * (8 - j1) + 63 - j));

					 if(RGB(r,g,b) != (COLORREF)rgbBk)
						SetPixel(hdc, x,y,RGB(r,g,b));
				 }
				}

    return 1;

}

//
// Redraw Options Graphics
//
int RedrawOptions()
{
	int n;
    char stri[10];

    COLORREF co1;


	    MyBarRect(X0Options,Y0Options, X0Options + OptWidth, Y0Options + OptHeight,COLORREF RGB(240,240,240));

		// Redraw Scale of Chess Degrees in Chess

		MyTextOut(X0Options + 10 * ax,Y0Options  + 5 * ay, " Nivel sahist   :",RGB(0,0,0));

		for(n=0;n<Options.nLevel;n++)
		{
			co1 = RGB(200,200,200);
			if(n == Options.level)
				co1 = RGB(0,255,0);

			
			MyCircle(X0Options + 8 * SZSCALE + n * SZSCALE + 10 * ax, Y0Options + 17 * ay, 10 * ax, co1);

			sprintf(stri," %d",n+ 1);
			MyTextOut(X0Options + 8 * SZSCALE + n * SZSCALE, Y0Options + 9 * ay, stri,RGB(0,0,0));

		}

		MyTextOut(X0Options + 10 * ax,Y0Options  + 30 * ay, " Nivel de timp :",RGB(0,0,0));

		for(n=0;n<Options.nLevel;n++)
		{
			co1 = RGB(200,200,200);
			if(n == Options.nTime)
				co1 = RGB(0,255,0);
			
			MyCircle(X0Options + 8 * SZSCALE + n * SZSCALE + 10 * ax, Y0Options + 42 * ay, 9 * ax, co1);

			sprintf(stri," %d",n + 1);
			if(n == Options.nLevel - 1)
				strcpy(stri," T");
			MyTextOut(X0Options + 8 * SZSCALE + n * SZSCALE, Y0Options + 35 * ay, stri,RGB(0,0,0));
		}

		MyTextOut(X0Options + 10 * ax,Y0Options  + 55 * ay, " Agresivitate   :",RGB(0,0,0));

		for(n=0;n<Options.nLevel;n++)
		{
			co1 = RGB(200,200,200);
			if(n == Options.agresivity)
				co1 = RGB(0,255,0);
			
			MyCircle(X0Options + 8 * SZSCALE + n * SZSCALE + 10 * ax, Y0Options + 67 * ay, 9 * ax, co1);

			sprintf(stri," %d",n + 1);
			MyTextOut(X0Options + 8 * SZSCALE + n * SZSCALE, Y0Options + 60 * ay, stri,RGB(0,0,0));
		}
	


  return 1;

}

//
// Redraw Info About Current and optimal Thinking
//
int	RedrawThinking()
{

	
	int numberFrom;
	int mv, xcol;
	char whitemove[20];
	char blackmove[20];
    int n;
    char movesstring[200];
    char strline[200];
    int aa;    
    int bb;
    int cc;
	double vv;
	int td;

	if(setup > 0) return 1; // no thinking when SETUP

// if(generatedTotal < 100) return 1;

	MyBarRect(X0Thinking,Y0Thinking, X0Thinking + ThWidth, Y0Thinking + ThHeight, RGB(220,220,220));

// Draw a X to cancel thinking info
	MyTextOut(X0Thinking + ThWidth - 10 * ax, Y0Thinking , "x", RGB(255,0,0));

    aa = Stack2[treeDepth].rankOnLevel + 1;
    bb = Stack2[treeDepth].movesOnLevel;
	if(aa > bb) aa = bb;
	if(bb > 100) bb = 1;
    cc = lgMax + 1;
    int dd = lgMaxKmax + 1;
	
    vv = OptimalMoveMoveValue ;
    vv = vv / 100.;
	
    td = treeDepth;
      
	if(treeDepth == gameDepth) 
	{
		aa = Stack2[treeDepth-1].rankOnLevel + 1;
		bb = Stack2[treeDepth-1].movesOnLevel;
		if(aa > bb) aa = bb;
		if(bb > 100) bb = 1;
	}
 
    sprintf(strline," Nivel : %d  Nr mutari generate : %d  Evaluare : %4.2f  Mutarea analizata : %d/%d Nivel maxim = %d Ramura Max. = %d   ",
		             kmax, nodes, vv, aa, bb, dd, cc);

	MyTextOut(X0Thinking + 10 * ax, Y0Thinking + 10 * ay, strline, RGB(0,0,128));
 
// Draw time on thinking
	time(&rawtime1);
 
	sprintf(stime,"Timp(sec): %d ",rawtime1 - rawtime0);

	MyTextOut(X0Thinking + ThWidth - 120 * ax, Y0Thinking + 10 * ay, stime, RGB(255,0,0));

	if(Options.thinking == 0) return 1;
	
	for(n=0;n<200;n++)
		movesstring[n] = ' '; // make clear the string

    SetBkMode(hdc,OPAQUE);

	numberFrom = 0 ;
    xcol = 0;
	strcpy(strline ," Varianta curenta   : ");


	for(n=numberFrom;Stack2[td + n].move > 0 ;n=n+2)
	{
if(findopening > 0 || backmove > 0) break;
	    strcpy(whitemove,"          ");
	    strcpy(blackmove,"          ");

		if(gameDepth == td) 
		{
			strcpy(strline ," Mutare calculator  :  ");
			MoveToString(OptimalMove, whitemove);
	        sprintf(movesstring, " %s ", whitemove);
			strcat(strline,movesstring);
			break;
		}


		mv = n / 2 + 1;
        
		if(Stack2[td + n].move == 0) break;
        aa = 0;
		MoveToString(Stack2[td + n].move, whitemove);
		if(Stack2[td + 1].move > 0)
		{  
			aa++;
			MoveToString(Stack2[td + n + 1].move, blackmove);
		}
        sprintf(movesstring, " %3d. %s   %s ", mv, whitemove, blackmove);
		strcat(strline,movesstring);
		xcol++;
		if(aa == 0) break;
		if(xcol > 2) break;
	}
if(findopening == 0 && backmove == 0)
	MyTextOut(X0Thinking + 10 * ax , Y0Thinking + 30 * ay, strline, RGB(0,0,0));

	if((moveMessage[0] > 0) && (gameDepth == td))
		{
			strcpy(strline ," ATENTIE  :  ");
			MoveToString(OptimalMove, whitemove);
	        sprintf(movesstring, " %s ", whitemove);
			strcat(movesstring,moveMessage);
			strcat(strline,movesstring);
			MyTextOut(X0Thinking + 10 * ax  + 500 * ax, 
				Y0Thinking + 30 * ay, strline, RGB(255,0,0));

		}

	
	if((openingMessage[0] > 0) && (findopening > 0))
		{
			strcpy(strline ," DESCHIDERE  :  ");
	        sprintf(movesstring, " %s ", openingMessage);
			strcat(strline,movesstring);
			MyTextOut(X0Thinking + 10 * ax + 500 * ax,
				Y0Thinking + 30 * ay, strline, RGB(255,0,0));

		}
	
	if(findopening > 0 && backmove == 0) 
		{
			strcpy(strline ," Mutare calculator  :  ");
			MoveToString(OptimalMove, whitemove);
	        sprintf(movesstring, " %s ", whitemove);
			strcat(strline,movesstring);
			MyTextOut(X0Thinking + 10 * ax , Y0Thinking + 30 * ay, strline, RGB(0,0,0));
//			findopening = 0;
		}


    int ix;
	numberFrom = 0 ;
    xcol = 0;
	strcpy(strline ," Varianta optima    : ");

	ix = treeDepth;
	if(treeDepth == gameDepth)
		ix--;

	for(n=numberFrom;BestLine[n] > 0 ;n=n+2)
	{
	    strcpy(whitemove,"          ");
	    strcpy(blackmove,"          ");

		mv = n / 2 + 1;
		MoveToString(BestLine[n    ], whitemove);
        aa = 0;
		if(BestLine[n] == 0) break;

		if(BestLine[n + 1] > 0)
		{   aa++;
		MoveToString(BestLine[n  + 1  ], blackmove);
		}
        sprintf(movesstring, " %3d. %s   %s ", mv, whitemove, blackmove);
		strcat(strline,movesstring);
		xcol++;
		if(aa == 0) break;
		if(xcol >= 4) break;
	}


	MyTextOut(X0Thinking + 10 * ax, Y0Thinking + 50 * ay, strline, RGB(0,0,0));


    return 1;
}

//
// Redraw Figures of the Taken Pieces in Order these were taken
//

int	RedrawTakenPieces()
{
	int ind0,ind;
	int i,j,r,g,b, rgbBk;

    int x,y;
    double dSq;
	int nw,nb;
	int n;
	int pie;

int PieceMoved;
int PieceTaken;
int SqFrom;
int SqTo;



    if(gameDepth > treeDepth) return 1;


if((setup == 0) && (setup1 == 0))
{
	nw = 0;
	nb = 0;

	MyBarRect(X0Taken ,Y0Taken - 16 * ay, X0Taken + TaWidth , 
		Y0Taken + TaHeight + 1 * ay + 10, RGB(220,220,220));

for(n=0;n<MoveNumber - 1;n++)
{

	SqFrom = (Stack2[n].move >> 8 ) & 255;
	SqTo   = Stack2[n].move & 255;

	PieceTaken = (Stack2[n].move >> 16 ) & 15;

	PieceMoved = (Stack2[n].move >> 20 ) & 15;


	pie = PieceTaken;

    if (pie == 0) continue;


	dSq = (double)1 / (double)2 * ax;
	ind0 = IconSet * ICONSIZE * ICONSIZE * 3 * 13 + ICONSIZE * ICONSIZE * 3 * pie;
	ind = ind0 + ICONSIZE * 3 * 2 + 3 * 2 ; // a point near corner of icon not just on edge

	rgbBk = RGB((int)IconBits[ind], (int)IconBits[ind + 1], (int)IconBits[ind + 2]);
			
	for(j = 0; j< ICONSIZE; j++)
		{
			 for(i= 0;i<ICONSIZE;i++)
			 {
	             ind = ind0 + j * 3 * ICONSIZE + i * 3;
				 r = (int)IconBits[ind];
				 g = (int)IconBits[ind + 1];
				 b = (int)IconBits[ind + 2];
				 if( pie > 6)
				 {
//			 x = (int)(X0Board + dSq * ( ICONSIZE * (i1 + 1) + i));
//			 y = (int)(Y0Board + dSq * ( ICONSIZE * (8 - j1) + 63 - j));
					 x = (int)(X0Taken + dSq * ( ICONSIZE * (nb )  + i));
					 y = (int)(Y0Taken + dSq * (-30 + 63 - j));
				 }
				 else
				 {
					 x = (int)(X0Taken + dSq * ( ICONSIZE * (nw) + i));
					 y = (int)(Y0Taken + dSq * ( ICONSIZE  - 30 + 63 - j));
				 }

			 if(RGB(r,g,b) != (COLORREF)rgbBk)
						SetPixel(hdc, x ,y ,RGB(r,g,b));
			 }
		}
	if(pie <= 6) nw++;
	if(pie >  6) nb++;

	} // end for
}
else
{

	nw = 1;
	nb = 1;

	MyBarRect(X0Taken ,Y0Taken - 60 * ay, X0Taken + TaWidth , 
		Y0Taken + TaHeight + 25 * ay, RGB(200,240,200));

	MyTextOut(X0Taken + TaWidth / 2.6 * ax, Y0Taken - 50 * ay, 
		" S E T A R E   P O Z I T I E  ", RGB(0,0,255));



	MyBarRect(X0Taken + TaWidth / 2.6 * ax ,Y0Taken - 10 * ay, 
			X0Taken + TaWidth / 2.6 * ax + 120 * ax, 
		Y0Taken + 10 * ay , RGB(0,0,200));



	MyTextOut(X0Taken + TaWidth / 2.6 * ax, Y0Taken - 10 * ay, 
		" Alt Set Piese ", RGB(255,255,0));


	MyBarRect(X0Taken + TaWidth / 2.6 * ax + 140 * ax ,Y0Taken - 10 * ay, 
			X0Taken + TaWidth / 2.6 * ax + 260 * ax, 
		Y0Taken + 10 * ay , RGB(0,0,200));

	MyTextOut(X0Taken + TaWidth / 2.6 * ax + 140 * ax, Y0Taken - 10 * ay, 
		" Setare Pozitie ", RGB(255,255,0));

	MyBarRect(X0Taken + TaWidth / 2.6 * ax + 300 * ax ,Y0Taken - 10 * ay, 
			X0Taken + TaWidth / 2.6 * ax + 420 * ax, 
		Y0Taken + 10 * ay , RGB(0,0,200));

	MyTextOut(X0Taken + TaWidth / 2.6 * ax + 300 * ax, Y0Taken - 10 * ay, 
		" Sterge Tabla ", RGB(255,255,0));

	MyBarRect(X0Taken + TaWidth / 2.6 * ax + 440 * ax,Y0Taken - 10 * ay, 
			X0Taken + TaWidth / 2.6 * ax + 560 * ax, 
		Y0Taken + 10 * ay , RGB(0,0,200));

	MyTextOut(X0Taken + TaWidth / 2.6 * ax + 440 * ax, Y0Taken - 10 * ay, 
		" Pozitia Initiala ", RGB(255,255,0));

	MyBarRect(X0Taken + TaWidth / 2.6 * ax + 600 * ax ,Y0Taken - 10 * ay, 
			X0Taken + TaWidth / 2.6 * ax + 720 * ax, 
		Y0Taken + 10 * ay , RGB(0,0,200));

	MyTextOut(X0Taken + TaWidth / 2.6 * ax + 600 * ax, Y0Taken - 10 * ay, 
		" Setare OK ", RGB(255,255,0));

	MyTextOut(X0Taken + TaWidth - 700 * ax, Y0Taken + 20 * ay, 
		" <-- Alegeti cu Mouse Tipul Piesei din aceasta caseta, apoi click pe Tabla de Sah", RGB(0,0,255));


	// Redtaw pieces for SETUP Chess Position
for(n=0;n<13;n++)
{

	pie = n;

	dSq = (double)1 / (double)2;

	if((pie == 0) || (pie == pieceType))
	{
		for(j = 0; j< ICONSIZE; j++)
		{
			 for(i= 0;i<ICONSIZE;i++)
			 {
				 if(pie == 0)
				 {
					r = 100;
					g = 255;
					b = 100;
				 }
				 else
				 {
					r = 100;
					g = 100;
					b = 255;
				 }

				 if( pie > 6)
				 {
					 x = (int)(X0Taken + dSq * ( ICONSIZE * (nb )  + i));
					 y = (int)(Y0Taken + dSq * (-30 + 63 - j));
				 }
				 else
				 {
					 x = (int)(X0Taken + dSq * ( ICONSIZE * (nw) + i));
					 y = (int)(Y0Taken + dSq * ( ICONSIZE  - 30 + 63 - j));
				 }
				
				 SetPixel(hdc, x,y,RGB(r,g,b));

			 }
		} // end for
		if(pie == 0)
		{
			nw = 3;
			nb = 3;
			continue;
		}
	}

	dSq = (double)1 / (double)2 * ax;
	ind0 = IconSet * ICONSIZE * ICONSIZE * 3 * 13 + pie * ICONSIZE * ICONSIZE * 3;
	ind = ind0 + ICONSIZE * 3 * 2 + 3 * 2 ; // a point near corner of icon not just on edge

	rgbBk = RGB((int)IconBits[ind], (int)IconBits[ind + 1], (int)IconBits[ind + 2]);
			
	for(j = 0; j< ICONSIZE; j++)
		{
			 for(i= 0;i<ICONSIZE;i++)
			 {

				 ind = ind0 + j * 3 * ICONSIZE + i * 3;
				 r = (int)IconBits[ind];
				 g = (int)IconBits[ind + 1];
				 b = (int)IconBits[ind + 2];
				 if( pie > 6)
				 {
//			 x = (int)(X0Board + dSq * ( ICONSIZE * (i1 + 1) + i));
//			 y = (int)(Y0Board + dSq * ( ICONSIZE * (8 - j1) + 63 - j));
					 x = (int)(X0Taken + dSq * ( ICONSIZE * (nb )  + i));
					 y = (int)(Y0Taken + dSq * (-30 + 63 - j));
				 }
				 else
				 {
					 x = (int)(X0Taken + dSq * ( ICONSIZE * (nw) + i));
					 y = (int)(Y0Taken + dSq * ( ICONSIZE  - 30 + 63 - j));
				 }

			 if(RGB(r,g,b) != (COLORREF)rgbBk)
						SetPixel(hdc, x,y,RGB(r,g,b));
			 }
		}
	if(pie <= 6) nw++;
	if(pie >  6) nb++;

	} // end for
}

    return 1;
}	

//
// Redraw Moves List
//
int	RedrawMoveList()
{

   
	int numberFrom;
	int mv, xrow;
	char whitemove[20];
	char blackmove[20];
    int n;

    char movesstring[30];

	if(setup == 0)
   {
	MyBarRect(X0MoveList,Y0MoveList - 16, X0MoveList + 80 + MvLstWidth , Y0MoveList + 20  + MvLstHeight, RGB(200,220,200));

	
	for(n=0;n<16;n++)
		movesstring[n] = ' '; // make clear the string
   
    SetBkMode(hdc,OPAQUE);
 

	numberFrom = (MoveNumber - 12) ;
    numberFrom = numberFrom - (numberFrom & 1);
	if (numberFrom < 0) numberFrom = 0;
	xrow = 1;
	for(n=numberFrom;n<=(MoveNumber - 2);n=n+2)
	{
	    strcpy(whitemove,"          ");
	    strcpy(blackmove,"          ");

		mv = n / 2 + 1;
		MoveToString(Stack2[n    ].move, whitemove);
        if(n<=(MoveNumber - 3))
			MoveToString(Stack2[n + 1].move, blackmove);
        sprintf(movesstring, " %3d. %s    ", mv, whitemove);
		MyTextOut(X0MoveList + 10 * ax, Y0MoveList  + xrow * 20 * ay, movesstring, RGB(0,0,0));
		MyTextOut(X0MoveList + 120 * ax, Y0MoveList  + xrow * 20 * ay, blackmove, RGB(0,0,0));
		
	  sprintf(movesstring, "%s %s", Stack2[n].info, Stack2[n+1].info);
	  //if(strcpy(movesstring, "() ()") !=0 ) strcpy(movesstring,"");
		MyTextOut(X0MoveList + 30 * ax, Y0MoveList  + (xrow + 1) * 20 * ay, movesstring, RGB(0,0,255));	  
		xrow= xrow + 2;;
	}
	indRedraw = 1;

    return 1;
   }
	else
	{
    // Draw Model Options
/*
	xrow = 1;
	MyBarRect(X0MoveList,Y0MoveList, X0MoveList + MvLstWidth, Y0MoveList + MvLstHeight, RGB(200,220,200));
	for(int nn=0;nn<=nModels;nn=nn++)
	{
        n1 = nn % 2;
		hbm = (HBITMAP) LoadImage(hInst, modelList[n], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	    if(hbm)
		{
		 hBrush = CreatePatternBrush(hbm);
		 DeleteObject(hbm);

		 xa = X0MoveList + 40 * ax + n1 * 50 * ax;
		 ya = Y0MoveList + 10 * ay + 40 * (nn - (nn % 2)) / 2 * ay;
		 xb = X0MoveList + 40 * ax + 40 * ax + n1 * 50 * ax;
		 yb = Y0MoveList + 10 * ay + 40 * (nn - (nn % 2)) / 2 * ay + 35 * ay;
  
		 SetBkColor(hdc,RGB(200,220,200));

		 SelectObject(hdc, hBrush);

		 Rectangle(hdc,xa,ya,xb,yb);

		 DeleteObject(hBrush);

		}
	} // end for

		MyTextOut(X0MoveList + 5 * ax , Y0MoveList + MvLstHeight * ay - 40 * ay, 
				" ALEGETI MODEL DECOR ", RGB(0,0,255));

*/
	} // end else
	return 1;
}


//
// Redraw Buttons
// 
int RedrawButtons()
{
    X0Buttons = X0Board + 15 * SQSIZE + 50;
    Y0Buttons = Y0Board + 4 * SQSIZE;

    ButtWidth  = 4 * SQSIZE;
    ButtHeight = 5 * SQSIZE;

	MyBarRect(X0Buttons,Y0Buttons, X0Buttons + ButtWidth, Y0Buttons + ButtHeight, RGB(200,200,255));


	MyCircle(X0Buttons  + 15 * ax, Y0Buttons + 20 * ay, 8 * ax, RGB(100,100,250));
	MyTextOut(X0Buttons + 15 * ax + 0.5 * SQSIZE, Y0Buttons + 10 * ay + 2, " Partida noua  ", RGB(0,0,0));

	MyCircle(X0Buttons  + 15 * ax + 0.5 * SQSIZE, Y0Buttons + 50 * ay, 8 * ax, RGB(100,250,100));
	MyTextOut(X0Buttons + 15 * ax + SQSIZE, Y0Buttons + 40 * ay + 2, " Mutare Inapoi     ", RGB(0,0,0));

	MyCircle(X0Buttons  + 15 * ax + 0.5 * SQSIZE, Y0Buttons + 80 * ay, 8 * ax, RGB(100,250,100));
	MyTextOut(X0Buttons + 15 * ax + SQSIZE, Y0Buttons + 70 * ay + 2,  " Mutare Inainte    ", RGB(0,0,0));

	MyCircle(X0Buttons  + 15 * ax + 0.5 * SQSIZE, Y0Buttons + 110 * ay, 8 * ax, RGB(100,250,100));
	MyTextOut(X0Buttons + 15 * ax + SQSIZE, Y0Buttons + 100 * ay + 2, " Mutare Progr ", RGB(0,0,0));


	MyCircle(X0Buttons  + 15 * ax, Y0Buttons + 140 * ay, 8 * ax, RGB(100,100,250));
	MyTextOut(X0Buttons + 15 * ax + 0.5 * SQSIZE, Y0Buttons + 130 * ay + 2, " Om vs Om ", RGB(0,0,0));

	MyCircle(X0Buttons  + 15 * ax, Y0Buttons + 170 * ay, 8 * ax, RGB(100,100,250));
	MyTextOut(X0Buttons + 15 * ax + 0.5 * SQSIZE, Y0Buttons + 160 * ay + 2, " SETARI       ", RGB(0,0,0));

	MyCircle(X0Buttons  + 15 * ax, Y0Buttons + 200 * ay, 8 * ax, RGB(100,100,250));
	MyTextOut(X0Buttons + 15 * ax + 0.5 * SQSIZE, Y0Buttons + 190 * ay + 2, " Progr vs Progr", RGB(0,0,0));
	
	MyCircle(X0Buttons  + 15 * ax, Y0Buttons + 230 * ay, 8 * ax, RGB(100,100,250));
	MyTextOut(X0Buttons + 15 * ax + 0.5 * SQSIZE, Y0Buttons + 220 * ay + 2, " Salvare partida", RGB(0,0,0));
	

	return 1;
}

//
// Convert Move Structure to String
//

extern char ListPieces[];

int MoveToString(int Move, char moveout[])
{

int k;
int sq1,sq2,p1,p2,indicators;
int xx;

sq1 = (Move >> 8 ) & 255;
sq2 = (Move & 255);
p1  = (Move >> 20) & 15;
p2  = (Move >> 16) & 15;
indicators = (Move >> 24) & 15;

if (sq1 < 21 || sq1 > 100 || sq2 < 21 || sq2 > 100)
{
	 moveout[0] = 0;
  return 0;	 
}
 if(sq1 == 0)
 { 
  strcpy(moveout,"      ");

 }

for(k=0;moveout[k] = ' ', k < 10; k++);

moveout[1] = (sq1 % 10 ) + 64 + 32;
moveout[2] = (sq1 / 10 ) + 48 - 1;

moveout[3] = '-'; // intre cele doua patrate este '-' sau 'x' daca se ia ceva
if (p2 != 0)
{
	moveout[3] = 'x';
}
moveout[4] = (sq2 % 10 ) + 64 + 32;

moveout[5] = (sq2 / 10 ) + 48 - 1;

moveout[0] = ListPieces[p1];
moveout[6] = ' ';
//if(move->IsCheck > 0) moveout[6] = '+';
//if(move->IsCheck ==  99) moveout[7] = '+';
k = indicators;
if((k == 2) || (k == 3))
   moveout[6] = 'p';
if((sq1 == 25) && (sq2 == 27) && (p1 == WK)) strcpy(moveout," 0 - 0 ");
if((sq1 == 95) && (sq2 == 97) && (p1 == BK)) strcpy(moveout," 0 - 0 ");
if((sq1 == 25) && (sq2 == 23) && (p1 == WK)) strcpy(moveout," 0-0-0 ");
if((sq1 == 95) && (sq2 == 93) && (p1 == BK)) strcpy(moveout," 0-0-0 ");


moveout[7] = 0;

return 1;

}

//
// Variat to PNG format
//
int MoveToStringPNG(int, char[]);
char ListPieces2[] = "0PRNBQKPRNBQK";
int	MoveToStringPNG(int Move, char moveout[])
{

int k;
int i;
int sq1,sq2,p1,p2,indicators;
int xx;

sq1 = (Move >> 8 ) & 255;
sq2 = (Move & 255);
p1  = (Move >> 20) & 15;
p2  = (Move >> 16) & 15;
indicators = (Move >> 24) & 15;


 if(sq1 == 0)
 { 
  strcpy(moveout,"      ");
  return 0;
 }

for(k=0;moveout[k] = ' ', k < 10; k++);

moveout[1] = (sq1 % 10 ) + 64 + 32;
moveout[2] = (sq1 / 10 ) + 48 - 1;
i = 3;
//moveout[3] = '-'; // intre cele doua patrate este '-' sau 'x' daca se ia ceva
if (p2 != 0)
{
	moveout[i] = 'x';
	i++;
}
moveout[i] = (sq2 % 10 ) + 64 + 32;

moveout[i+ 1] = (sq2 / 10 ) + 48 - 1;
i = i + 2;
moveout[0] = ListPieces2[p1];

//if(move->IsCheck > 0) moveout[6] = '+';
//if(move->IsCheck ==  99) moveout[7] = '+';
k = indicators;
/*
if((k == 2) || (k == 3))
{
  moveout[i] = 'p';
  i++;
}
*/
 moveout[i] = 0; 
if((sq1 == 25) && (sq2 == 27) && (p1 == WK)) strcpy(moveout,"0-0");
if((sq1 == 95) && (sq2 == 97) && (p1 == BK)) strcpy(moveout,"0-0");
if((sq1 == 25) && (sq2 == 23) && (p1 == WK)) strcpy(moveout,"0-0-0");
if((sq1 == 95) && (sq2 == 93) && (p1 == BK)) strcpy(moveout,"0-0-0");

if(indicators == 5) strcat(moveout,"=Q");
if(indicators == 6) strcat(moveout,"=R");
if(indicators == 7) strcat(moveout,"=B");
if(indicators == 8) strcat(moveout,"=N");
if(indicators == 9) strcat(moveout,"=Q");
if(indicators == 10) strcat(moveout,"=R");
if(indicators == 11) strcat(moveout,"=B");
if(indicators == 12) strcat(moveout,"=N");

// moveout[7] = 0;

return 1;
	
}

int MyBarRect(int x1a,int y1a,int x2b,int y2b,COLORREF  co1)
{
static HBRUSH hBrush;
int xa,xb,ya,yb;

static LPPOINT lpPoint;


   xa = x1a;
   ya = y1a;
   xb = x2b;
   yb = y2b;
  
 

   SetBkMode(hdc,TRANSPARENT );
   SetTextColor(hdc,co1);
   SetBkColor(hdc,co1);

   hBrush = CreateSolidBrush(co1);
   SelectObject(hdc, hBrush);


   Rectangle(hdc,xa,ya,xb,yb);

   DeleteObject(hBrush);

   return 0;
}

int MyBarRect1(int x1a,int y1a,int x2b,int y2b,COLORREF  co1)
{
int xa,xb,ya,yb;
int j;

static LPPOINT lpPoint;


   xa = x1a;
   ya = y1a;
   xb = x2b;
   yb = y2b;
  
 
   for(j=xa;j<=xb;j++)
   {
	   MyLine(j,ya,j,yb, co1);
   }

 
   return 0;
}



int MyTextOut(int x1a, int y1a, char *buf1, COLORREF  co1)
{
//     SelectObject(hdc,GetStockObject(OEM_FIXED_FONT));
     SetBkMode(hdc,TRANSPARENT);
     SetTextColor(hdc,co1);
     TextOut(hdc,x1a,y1a, buf1, strlen(buf1));
     SetBkMode(hdc,OPAQUE );

     return 0;
     
}

int MyLine(int x1a, int y1a, int x2b, int y2b, COLORREF  co1)
{

static LPPOINT lpPoint;

static HPEN hPen,hOldPen;

     hPen    = CreatePen(PS_SOLID,0, co1);

     SelectObject(hdc, hPen);

     MoveToEx(hdc, x1a, y1a,lpPoint);
     LineTo(hdc, x2b,  y2b);

     DeleteObject(hPen);

     return 0;
}


int MyCircle(int xce,int yce,int raz,
   COLORREF colo1)

{
static HBRUSH hBrush,hOldBrush;

int xa,ya;


   xa = xce;
   ya = yce;

   hBrush    = CreateSolidBrush(RGB(28,28,28));

   SelectObject(hdc, hBrush);

   Ellipse(hdc, xa-raz-2, ya-raz-2, xa+raz-2, ya+raz-2);

   DeleteObject(hBrush);

   hBrush    = CreateSolidBrush(colo1);

   SelectObject(hdc, hBrush);


   Ellipse(hdc, xa-raz, ya-raz, xa+raz, ya+raz);


   DeleteObject(hBrush);

   return 0;    
}   


int My2TextOut(int xw11, int yw11, int hiw1,int dwx1,
              char *buf4a, COLORREF co3a)
{

static LOGFONT logf;
static HFONT hFont, hfOld;


     logf.lfHeight = hiw1;
     logf.lfWidth  = (int)((double)dwx1/(double)strlen(buf4a));
     logf.lfEscapement = 0; // zecimi de grade
     logf.lfOrientation = 0;
     logf.lfWeight = FW_NORMAL;
     if (hiw1 > 25)
       logf.lfWeight = FW_BOLD;
     logf.lfItalic = FALSE;
     logf.lfUnderline = FALSE;
     logf.lfStrikeOut = FALSE;
     logf.lfCharSet = OEM_CHARSET;
     logf.lfOutPrecision = OUT_DEFAULT_PRECIS;
     logf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
     strcpy(logf.lfFaceName,"FF_DONTCARE");
     logf.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;

     hFont = CreateFontIndirect(&logf);

//     hfOld = SelectObject(hdc, hFont);
     
     SetTextColor(hdc,co3a);
     TextOut(hdc,xw11,yw11, buf4a, strlen(buf4a));

     SelectObject(hdc,hfOld);
     DeleteObject(hFont);

     SetBkMode(hdc,TRANSPARENT);

     return 0;
    
}              

//
// IMAGE_READ - Read a given .bmp image into memory
//

int image_read(char bu[], int pie, int IconSet)
{

char bupath[100];

//strcpy(bupath,currentDirectory);
//strcpy(bupath,"D:\\DISCUL E\\FRIENDLY CHESS\\BusinessChess\\");
// strcat(bupath,"\\");

strcpy(bupath,bu);

int begin_dates;
int nIcon;


int nread, sist, w, h;


int i,j;

int indi;

long dd;

int indi0;

   nIcon = IconSet * 13 + pie;

   f1 = fopen(bupath,"rb");


   if ( f1 == NULL) return 0;


   nread = fread((char *)&sist,1,2,f1); // read bitmap header length

   nread = fread((char *)&bmp_input,1,sizeof(struct bmp_header),f1); // read the header

   if( nread < sizeof(struct bmp_header))
     {
      fclose(f1);
      return 0;
     }
	fclose(f1);

   f1 = fopen(bupath,"rb");
   if ( f1 == NULL) return 0;

   begin_dates = bmp_input.image_offset;
   /* supose a n-bit image */
	w = bmp_input.width;
	h = bmp_input.heigth;

	IconBitmap[nIcon].bmType = 1;
	IconBitmap[nIcon].bmWidth = w;
	IconBitmap[nIcon].bmHeight = h;

	IconBitmap[nIcon].bmPlanes = 1;
	IconBitmap[nIcon].bmBitsPixel = 24;

	indi0 = IconSet * ICONSIZE * ICONSIZE * 3  * 13 + pie * ICONSIZE * ICONSIZE * 3;
	IconBitmap[nIcon].bmBits = &IconBits[indi0];

    
   /* supose a n-bit image */

   w3 = 3 * w;

   if ( (w3 & 1) == 1) 
	{
		w3 = w3 + 1;
		
	}
   if ( (w3 & 2) == 2) 
		w3 = w3 + 2;


   IconBitmap[nIcon].bmWidthBytes = w3;

   for(i=0;i<h;i++)
	{
		
	dd =  i * w3 + begin_dates;

	fseek(f1, dd, SEEK_SET);
	nread = fread(bufimg,1,w3,f1);
		
	for(j = 0; j < w ; j = j + 1)
		{ 
			indi = indi0 + i * w3 + j * 3 ; 
			IconBits[indi    ] = ((int)bufimg[j * 3 + 2]) & 255 ;
			IconBits[indi + 1] = ((int)bufimg[j * 3 + 1]) & 255;
			IconBits[indi + 2] = ((int)bufimg[j * 3 + 0]) & 255;
		}
   }

 return 1;

} // image_read



#include "SETUP.H"
