#include <windows.h>
#include "stdafx.h"

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>

#include <CommDlg.h>

#include "Structures.h"
#include "S2000.H"
int MyTextOut(int x1a, int y1a, char *buf1, COLORREF  co1);

extern char rezultat[10] ;

extern int sumatimpialb;
extern int sumatimpinegru;

extern int nrmutari;

extern int timpmediualb;
extern int timpmediunegru;

extern int nodesmutarialb;
extern int nodesmutarinegru;

extern int nodesmediualb;
extern int nodesmediunegru;

extern char currentDirectory[2000];

extern int SqFromActive ;
extern int SqToActive ;
extern int nModels ;

HWND mainhwnd;


BOOL CALLBACK PawnDlgProc( HWND hwnd, unsigned msg, UINT wparam, LONG lparam );

int	MoveToString(int Move, char []);
int	MoveToStringPNG(int Move, char []);

int findopening;
int  backmove;

int GenQuik(int, int, int, int);

int	RedrawThinking();

int CallRedrawChessBoard();
int MoveToDoMoveValue = 0;
int ExecuteBackButton();
int ShowMate();
int TaWhiteCheck(int sq);
int TaBlackCheck(int sq);

extern int bellOnOff;

int nplyx;

extern int sta ;

extern FILE *fpgn;

extern HWND hWnd;

extern struct Opt Options;

extern int thinking;
extern int indRedraw;
extern int ValPiesaReal[];
#include "HASHNC.H"
#include "FUNCTIONS.H"


#include "GENTREE.H"

#include "SAVEGAME.H"
#include "SETSTRUCT.H"
#include "TESTSAH.H"
#include "OPENING.H"

#include "ENDGAME.H"

#include "MATETHREAT.H"
#include "REPOSITORY.H"
#include "TAKEONEMOVE.H"

#include "EVALUATE.H"
#include "GENLEVEL.H"
#include "GENQUIK.H"
#include "TRANSPOSITION.H"
//
// Chess Board Initialization
//

int InitChessBoard()
{
srand(100);

  SetKingKing();
  SetDirKing2();
  SetQueenKing();
  SetRookKing();
  SetBishopKing();
  SetKnightKing();
  SetTabCheck();
  InitSquareArray ( );

  for(int i=0;i< 120; i++)
  {
	RocadaFlagsMayChange[i] = 0;
  }
  RocadaFlagsMayChange[21] = 1;
  RocadaFlagsMayChange[28] = 1;
  RocadaFlagsMayChange[25] = 1;
  RocadaFlagsMayChange[91] = 2;
  RocadaFlagsMayChange[98] = 2;
  RocadaFlagsMayChange[95] = 2;

  pawnhash = 0;
  nopawnhash = 0;
  Stack[0].mateDetected = 0;

  time(&rawtime0);
  rawtime1 = rawtime0;

  WhiteToMove = 1;

  BlackToMove = 0;

  WhiteIsMan = 1;
  
  BlackIsMan = 0;

  strcpy(moveMessage,"");

  return 1;

}

//
// Init New Game
//
int InitGame()
{

int  m;

SqFromActive = 0;
SqToActive = 0;

Stack[0].mateDetected = 0;

time(&rawtime0);
rawtime1 = rawtime0;

//indExchange = 0;

algorithm = 0;

for(i=0;i<13;i++)
  ValPiesa[i] = ValPiesaReal[i];

for(i=0;i < 120; i++) 
   tabla[i] = tabla0[i];


for(i=0;i < 120; i++) 
	ta[i] = tabla[i];


for(i=0;i < 120; i++) 
   tablaRoot[i] = tabla[i];


// Clear Stack[].explicitMove.SqFrom
for(i=0;i<500;i++)
{
  Stack[i].whiteKingSquare = 0 ;  // current position of white king position
  Stack[i].blackKingSquare = 0;  // current position of black king position
  Stack[i].RocadeIndicators = 0;  // Rocade flags

  Stack[i].depthReduced = 0; // must nor reduce depth in the same branch
  Stack[i].ponder = 0;             // cumul of pieces ponder in stack memory
  Stack[i].whitePower = 0;         // white pieces power sum
  Stack[i].blackPower = 0;         // black pieces power sum
  Stack[i].WCastle = 0;            //  flag if white rocade made
  Stack[i].BCastle = 0;            //  flag if black rocade made
  Stack[i].nPieces = 0;           // total number of pieces on board
  Stack[i].mateDetected = 0;      // if == 1 mate is detected and valable till the end of the game
  Stack[i].suplimDepth = 0;      // supplimentary depth caused by taken pieces
 
  Stack[i].nWP = 0;               // count of white pieces without WK and WP
  Stack[i].nBP = 0;               // count of black pieces without BK and BP
  Stack[i].nBMinors = 0;          // number of black minors on table
  Stack[i].nWMinors = 0;          //  -//-  white minors

  for(int j=0;j<13;j++)
  {
   Stack[i].pawnWOnFiles[j] = 0;  // how many white pawns on each line
   Stack[i].pawnBOnFiles[j] = 0;  // how many black pawns on each line
   Stack[i].pieceCounts[j];   // counts for each type of pieces
   Stack[i].pieceAPosition[i][j];  // a square where there is a piece of that type
  }
  Stack[i].positionVal;      // sum of position values of each piece on the board
  Stack[i].PawnHash;   // Current Pawn Hash initiated at each search and updated on PlayMove
  Stack[i].HashKey;          // Hash key - utilizat la repetitii
  Stack[i].HashCheck;        // Hash Check
	
 
  Stack2[i].SqFrom = 0;
  Stack2[i].SqTo = 0;
  Stack2[i].sortedlevel = 0;
  Stack2[i].PieceMoved = 0;
  Stack2[i].PieceTaken = 0;
  Stack2[i].Indicators = 0;
  Stack2[i].IsCheck = 0;

  Stack2[i].bestmove;      // best move with depth - 2
  Stack2[i].IsCheckOwn;         // if != 0 the enemy king can be taken
  Stack2[i].BestWay[0];	 // pointer to zone to store bestmoves line
  Stack2[i].BestMove = 0;		 // pointer to zone to store bestmoves line
  Stack2[i].MaxOnLevel = 0;    // MiniMax on this level
  Stack2[i].nbAttacks = 0;          // number of attack moves (and rocades, transform etc)
  for(int j = 0;j< 100;j++)
  {
	  Stack2[i].Attacks[j] = 0;  // all moves al a level
	  Stack2[i].ponders[j] = 0; // list of ponders corecponding to Attacks list, indexed by rankOnLevel
  }
  Stack2[i].flagsOfGame = 0;       // some flags of the game
  Stack2[i].reps = 0;              // repetitions;
  Stack2[i].BestLineDepth = 0;     // the depth of the generated best line vector
  Stack2[i].MaxDepth = 0;          // the max value for analized for move in tree
  Stack2[i].EffectiveAnalysed = 0; // how many moves where analysed on level 0
  Stack2[i].rankOnLevel = 0;       // how moves were analyzed on this level
  Stack2[i].rankAttacks = 0;       // explored moves from attacks

  Stack2[i].movesOnLevel = 0;      // nbAttacks + nbNoAttacks
  Stack2[i].MoveValue = 0;

  Stack2[i].enPassant = 0;         // 0 or pawn move 2 steps
  Stack2[i].eChecks = 0;          // how many moves are traited whem in check
  Stack2[i].recap = 0;            // ind of recapture
  Stack2[i].killers[0];
  Stack2[i].killers[1];  
  Stack2[i].killers_count[0] = 0; 
   Stack2[i].killers_count[1] = 0; 
  Stack2[i].move = 0;
  Stack2[i].flagsOfGame = 0;	
  Stack2[i].BestWay[0]     = 0;
}

InitValPiesa(Options.level);

who = 0;

ksup = Options.nTime;
limitSuplimDepth = 2;
limHigt = ksup * 2;

OptimalMoveMoveValue = 0;

Stack2[0].BestWay[0] = 0;

endOfGame = 0;

MoveNumber = 1;

gameDepth = 0;
treeDepth = 0;



//ClearRepository();
ClearMoveRepository();

Clear_Hash ( );
Create_HashFunction ( );


Stack[0].WCastle = 0;    //  flag if white rocade made
Stack[0].BCastle = 0;    //  flag if black rocade made

 // clear stack memory for best ways

 for(m=0;m<500;m++)
{

  strcpy(Stack2[m].info,"");
  
  for(i=0;i<50;i++)
  {
	  Stack2[m].BestWay[i] = 0;
  }
}

SetStruct();

lastTreeDepth = 0;

lgBestWay = 0;
lgMax = 0;
lgMaxKmax = 0;
noTime = 0;

sumatimpialb = 0;
sumatimpinegru = 0;

nrmutari = 0;

timpmediualb = 0;
timpmediunegru = 0;

nodesmutarialb = 0;
nodesmutarinegru = 0;

nodesmediualb = 0;
nodesmediunegru = 0;

Stack[gameDepth].RocadeIndicators = 63; // all rights to rocade

strcpy(moveMessage,"");

return 1;

}


//
// Clear Repository
//
/*
int ClearRepository()
{

int m, i;

// Clear deposit of Best Lines

 for(m=0;m<25000;m++)
{
  pm = (struct Move *)&BestWayDeposit[m].savedBestWay[0];
  BestWayDeposit[m].maxLevelAnalyzed = 0;
  BestWayDeposit[m].finishedLevel    = 0;
  BestWayDeposit[m].positionCheckSum = 0;
  for(i=0;i<50;i++)
  {
	  ClearPlace();
	  pm++;
  }
}

return 0;

}
*/
//
// Clear Move Repository
//
int ClearMoveRepository()
{

// int m;

// Clear deposit of Moves
/*
 for(m=0;m<1024 * 1024;m++)
{
  MoveDeposit[m].maxLevelAnalyzed = 0;
  MoveDeposit[m].finishedLevel    = 0;
  MoveDeposit[m].move             = 0;
  MoveDeposit[m].prune            = 0;
  MoveDeposit[m].positionCheckSum = 0;
}
*/
return 0;

}


//
// Make back a pair of white and black moves
//

int ExecuteBackButton()
{
/*
	if(indRedraw == 3) 
		{
			buttonPressed = 6;
			return 1;
		}
*/
			moveMessage[0] = 0;
			buttonPressed = 6;
			if(lastTreeDepth == 0)
				lastTreeDepth = MoveNumber;

			if(gameDepth > treeDepth) return 1; // MoveBack delayed to Gentree()

			for(;gameDepth > treeDepth;)
			{
				MoveBack();
			}
			if (gameDepth > 0) 
			{
				buttonPressed = 0;
				MoveBack();

	            MoveNumber  = MoveNumber - 1;
				treeDepth   = gameDepth;
				WhiteToMove = (WhiteToMove + 1) & 1; 
				BlackToMove = (BlackToMove + 1) & 1;
				Stack2[gameDepth].BestWay[0]     = Stack2[gameDepth].move;
				
				OptimalMove     = Stack2[gameDepth].move;
                OptimalMoveIsCheck    = Stack2[gameDepth].IsCheck;
                OptimalMoveMoveValue  = Stack2[gameDepth].MoveValue;
				Stack2[gameDepth].BestWay[1] = 0;
			}
/*
			if (gameDepth != 0) 
			{
				MoveBack();
	            MoveNumber = MoveNumber - 1;
			}
*/
		for(i=0;i < 120; i++) 
			tablaRoot[i] = tabla[i];

			treeDepth = gameDepth;
			findopening = 0;
			sta = 1;
			backmove = 1;
//			ClearRepository();
			indRedraw = 1;
			CallRedrawChessBoard();
			endOfGame = 0; // may move again if was mat
			return 1;

}


int	ExecuteForwardButton()
{
    if(indRedraw == 3) 
	{
	//	if(bellOnOff == 1) Beep(700,200);
		return 1;
	}

	if(endOfGame == 1) return 1;

	for(;;)
	{
		if(MoveNumber < lastTreeDepth)
		{
		MoveToDo     = Stack2[gameDepth].move;
		MoveToDoIsCheck    = Stack2[gameDepth].IsCheck;
		MoveToDoMoveValue  = Stack2[gameDepth].MoveValue;
		Stack2[gameDepth].MoveValue = 0;
	 
		PlayMove(MoveToDo);
		MoveNumber = MoveNumber + 1;
//		ClearRepository();

		for(i=0;i < 120; i++) 
			tablaRoot[i] = tabla[i];

		treeDepth = gameDepth;
		WhiteToMove = (WhiteToMove + 1) & 1; 
		BlackToMove = (BlackToMove + 1) & 1;
      
		sta = 1;

		indRedraw = 1;
		CallRedrawChessBoard();
		}
	break;	
	}

	return 1;
}


//
// GemePlaying - loop of two playes - one made by man and the computer answer
//
int GamePlaying(int nply)
{

int tasta;


int tt;

nplyx = nply;

for(tt=0;tt<nply;tt++)
{
	treeDepth = gameDepth;

	 Stack[gameDepth].HashKey  = Hash_Board ();
     Stack[gameDepth].HashCheck = Hash_Board2 ();

	 Stack[gameDepth].PawnHash = (int)Pawn_HashBoard ();

	Stack2[gameDepth].flagsOfGame = 0;
	Stack2[gameDepth].reps = TestRepeat();

	if (who == 0)
	{
		BestVal = -MAT;
	}
	else
	{
		BestVal = MAT;
	}
	EvaluateAttacks(0,0,0,0,0,0);	  // setup attacks tables and estimate exchanges

	if ( WhiteToMove )
	{
        SetStruct(); // Pieces list for whites and blacks
        
		if((Stack[gameDepth].pieceCounts[WK] == 0) || (Stack[gameDepth].pieceCounts[BK] == 0))
		{
			return 1;
		}
		if(abs(OptimalMoveMoveValue) >= 19997 || gameDepth > 0 && OptimalMove == 0 )
		{
			return -1;
		}

		if((totalMatePower <= 2) && (Stack[gameDepth].pieceCounts[WB] <= 1) 
			&& (Stack[gameDepth].pieceCounts[BB] <= 1) && abs(Stack[gameDepth].ponder) < 340)
		{
		SaveGame(hWnd);			
           strcpy(moveMessage, " REMIZA - NU MAI SUNT PIESE PENTRU MAT");
		   MessageBox(hWnd," REMIZA - NU MAI SUNT PIESE PENTRU MAT ", " REMIZA ",  MB_OK);

		   return -1;
		}

  	    tasta = GetWhiteMove();
        if (tasta == 0 || gameDepth > 0 && MoveToDo == 0 ) return 0;
        if(tasta > -1) 
		{                
	        MoveNumber++;
		
			WhiteToMove = 0;
			BlackToMove = 1;

  
			PlayMove(MoveToDo);
	 Stack[gameDepth].HashKey  = Hash_Board ();
     Stack[gameDepth].HashCheck = Hash_Board2 ();

	 Stack[gameDepth].PawnHash = (int)Pawn_HashBoard ();
	 Stack2[gameDepth].reps = TestRepeat();

		}
		treeDepth = gameDepth;
		indRedraw = 3;

		for(i=0;i < 120; i++) 
			tablaRoot[i] = tabla[i];
		

		CallRedrawChessBoard();

		Stack2[gameDepth].BestWay[0] = 0;

		if(moveMessage[0] > 0)
			{
				// if(bellOnOff == 1) Beep(800,100);
//				Sleep(5000);
//				MessageBox(hWnd, moveMessage, " MAT APROPIAT ", MB_OK);
//				strcpy(moveMessage,"");
			}

		indRedraw = 1;

		gTotal = 0;

		who = 1;

	 // Check if no 3 times position repetition
		if (Stack2[gameDepth].flagsOfGame == 44)
            {
		SaveGame(hWnd);					
           MessageBox(hWnd, " REPETARE POZITIE DE 3 ORI ", "  SFARSIT PARTIDA ", MB_OK);
		    endOfGame = 1;
			return -2;
		    }

	   if(tasta <= -1 ) return tasta;

	   if(BlackIsMan == 1) return 1;

	   if(nply == 999) return 1;
	   
	   continue;
	}
	else
	{
        SetStruct(); // Pieces list for whites and blacks

		if(Stack2[gameDepth].flagsOfGame == 66 || ((totalMatePower <= 2) && (Stack[gameDepth].pieceCounts[WB] <= 1) 
			&& (Stack[gameDepth].pieceCounts[BB] <= 1) && abs(Stack[gameDepth].ponder) < 340))
		{
		SaveGame(hWnd);				
           strcpy(moveMessage, " REMIZA - NU MAI SUNT PIESE PENTRU MAT");
		   MessageBox(hWnd," REMIZA - NU MAI SUNT PIESE PENTRU MAT ", " REMIZA ",  MB_OK);
		   return -1;
		}
		if(abs(OptimalMoveMoveValue) >= 19997 || gameDepth > 0 && OptimalMove == 0 )
		{
			return -1;
		}
 
		tasta = GetBlackMove();

        if (tasta == 0 || gameDepth > 0 && MoveToDo == 0 ) return 0;
        if(tasta > -1 ) 
		{		
			MoveNumber++;
		
			WhiteToMove = 1;
			BlackToMove = 0;

	
			PlayMove(MoveToDo);
	 Stack[gameDepth].HashKey  = Hash_Board ();
     Stack[gameDepth].HashCheck = Hash_Board2 ();

	 Stack[gameDepth].PawnHash = (int)Pawn_HashBoard ();
	 Stack2[gameDepth].reps = TestRepeat();
		}
		treeDepth = gameDepth;

		indRedraw = 3;

		for(i=0;i < 120; i++) 
			tablaRoot[i] = tabla[i];
		
//		UpdateWindow(hWnd);


		CallRedrawChessBoard();
		
		Stack2[gameDepth].BestWay[0] = 0;
		
		if(moveMessage[0] > 0)
			{

			//	if(bellOnOff == 1) Beep(800,100);
//				Sleep(5000);
//				MessageBox(hWnd, moveMessage, " MAT APROPIAT ", MB_OK);
//				strcpy(moveMessage,"");
			}

		indRedraw = 1;

		gTotal = 0;
	       
		who = 0;
	 // Check if no 3 times position repetition
		if (Stack2[gameDepth].flagsOfGame == 44)
            {
		SaveGame(hWnd);					
           MessageBox(hWnd, " REPETARE POZITIE DE 3 ORI ", "  SFARSIT PARTIDA ", MB_OK);
  	       endOfGame = 1;
		   return -2;
		    }

	   if(tasta <= -1 ) 
	   {
  	       endOfGame = 1;
		   return tasta;
	   }

	    if(nply == 999) return 1;

		if(WhiteIsMan == 1) return 1;

		continue;
	}
} // end for

    return 1;
}


//
// Show Mate if any
//
int ShowMate()
{

char buff[80];

SetStruct();

strcpy(moveMessage, "");


if (Stack[gameDepth].nPieces == 2)
		{
		SaveGame(hWnd);				
          strcpy(moveMessage, "");
		  MessageBox(hWnd," REMIZA - NU MAI SUNT PIESE","SFARSIT PARTIDA", MB_OK );
 	      endOfGame = 1;
		  return -1;
		}

if (Stack[gameDepth].pieceCounts[BK] == 0)
		{
		SaveGame(hWnd);				
          strcpy(moveMessage, "");
		  MessageBox(hWnd,"  EROARE POZITIE - REGE NEGRU LIPSA ","SFARSIT PARTIDA", MB_OK );
 	      endOfGame = 1;
		  return -1;
		}

if (Stack[gameDepth].pieceCounts[WK] == 0)
		{
		SaveGame(hWnd);				
          strcpy(moveMessage, "");
		  MessageBox(hWnd,"  EROARE POZITIE - REGE ALB LIPSA ","SFARSIT PARTIDA", MB_OK );
 	      endOfGame = 1;
		  return -1;
		}

strcpy(rezultat, "[1/2-1/2]");

if (Stack2[gameDepth].flagsOfGame == 55)
		{
		SaveGame(hWnd);				
          strcpy(moveMessage, " REMIZA - PAT");
		  MessageBox(hWnd," REMIZA - PAT","SFARSIT PARTIDA", MB_OK );
 	      endOfGame = 1;
		  return -1;
		}
if (kmax == 20 && OptimalMoveMoveValue == 0)
		{
		SaveGame(hWnd);				
          strcpy(moveMessage, " SFARSIT PARTIDA");
		  MessageBox(hWnd," ","SFARSIT PARTIDA", MB_OK );
 	        endOfGame = 1;
		  return -1;
		}
if (Stack2[gameDepth].flagsOfGame == 44)
		{
		SaveGame(hWnd);				
          strcpy(moveMessage, " REMIZA - REPETITIE DE 3 ORI");
		  MessageBox(hWnd," REMIZA - REPETITIE DE 3 ORI","SFARSIT PARTIDA", MB_OK );
 	      endOfGame = 1;
		  return -1;
		}

if (Stack2[gameDepth].flagsOfGame == 33)
		{
		SaveGame(hWnd);				
          strcpy(moveMessage, " REMIZA - REGULA CELOR 50 MUTARI ");
		  MessageBox(hWnd," REMIZA - REGULA CELOR 50 MUTARI","SFARSIT PARTIDA", MB_OK );
 	      endOfGame = 1;
		  return -1;
		}
if (treeDepth > 250)
		{
		SaveGame(hWnd);				
          strcpy(moveMessage, " PARTIDA PREA LUNGA - 125 mutari ");
		  MessageBox(hWnd,moveMessage,"SFARSIT PARTIDA", MB_OK );
 	      endOfGame = 1;
		  return -1;
		}


if (OptimalMoveMoveValue > (MAT - 100))
		{
			
		sprintf(buff,"  Albul da MAT in %d mutari ",(MAT - OptimalMoveMoveValue) / 2 );
		if(OptimalMoveMoveValue >= (MAT - 2)) 
			SaveGame(hWnd);
		
		if (OptimalMoveMoveValue >= (MAT - 2))
		  {   
		SaveGame(hWnd);				  
				strcpy(rezultat, "1-0]");
             strcpy(moveMessage," SFARSIT PARTIDA - MAT ");
			 MessageBox(hWnd,"MAT", " SFARSIT PARTIDA ",MB_OK); 
		     endOfGame = 1;
			 return -1;
		}
		else
		  {
             strcpy(moveMessage, buff);
//			 MessageBox(hWnd,buff," MAT APROPIAT ",MB_OK); 
//			 MessageBox(hWnd,buff," MAT APROPIAT ",1); 
			 return 1;
		  }
		}

if (OptimalMoveMoveValue < (-MAT + 100))
		{
		if(OptimalMoveMoveValue <= (-MAT + 2)) 
			SaveGame(hWnd);		
		sprintf(buff,"  Negrul da MAT in %d mutari ",(MAT + OptimalMoveMoveValue) / 2 );
		if (OptimalMoveMoveValue >= (-MAT + 100))
			sprintf(buff,"  Posibil MAT Negrul in  %d mutari ",(MAT + OptimalMoveMoveValue - 100) / 2 );
 
		if (OptimalMoveMoveValue <= (-MAT + 2))
		  {   
		SaveGame(hWnd);				  
			strcpy(rezultat, "[0-1]");
             strcpy(moveMessage," SFARSIT PARTIDA - MAT ");
			 MessageBox(hWnd,"MAT"," SFARSIT PARTIDA ",MB_OK); 
		     endOfGame = 1;
			 return -1;
		}
		else
		  {
             strcpy(moveMessage, buff);
//			 MessageBox(hWnd,buff," MAT APROPIAT ",MB_OK); 
//			 MessageBox(hWnd,buff," MAT APROPIAT ",1); 
			 return 1;
		  }
		}

	return 1;

}

extern int SqFromActive;
extern int SqToActive;

int GetWhiteMove()
{
int rr;
backmove = 0;

if ((WhiteIsMan) && (nplyx != 999))
	{
		rr = GetMoveString(MoveToDo);

		if ( rr != 1) return rr;
		OptimalMove = MoveToDo;
		OptimalMoveMoveValue = MoveToDoMoveValue;
//		CopyMove(&MoveToDo,&OptimalMove);

		kmax = 1; // needed by GenLevelMoves
		noTime = 0;

		Stack2[gameDepth].IsCheckOwn = IsCheck(who);
  
		GenLevelMoves(0);
        		
		return AllowedMove(MoveToDo, MoveToDo);
	}
else {
	 rr = GetOptimalMove();

	 if(buttonPressed == 6)
		{
		ExecuteBackButton();
		buttonPressed = 0;
		 thinking = 0;
		 return 0;
		}
 

	 thinking = 0;
	 if ( rr == 1)
	 {
/*
		 PutMoveString(OptimalMove.SqFrom, OptimalMove.SqTo,OptimalMove.PieceMoved,
		 OptimalMove.PieceTaken,OptimalMove.Indicators);
     printf(" %s  MiMax = %5d  Nodes = %10d ",moveout, 
		 OptimalMove.MoveValue,generatedTotal );
*/
	 }
	 return rr; // if rr == -1 3 time repetition
	}

}

int GetBlackMove()
{
int ee;
backmove = 0;

if ((BlackIsMan) && (nplyx != 999))
	{
		ee = GetMoveString(MoveToDo);
		OptimalMove = MoveToDo;
		OptimalMoveMoveValue = MoveToDoMoveValue;

//		CopyMove(&MoveToDo,&OptimalMove);

		if ( ee != 1) return ee;
        kmax = 1;
		noTime = 0;
	
		Stack2[gameDepth].IsCheckOwn = IsCheck(who);

		GenLevelMoves(0);
		
		return AllowedMove(MoveToDo, MoveToDo);
	}
else {
	 
	 ee = GetOptimalMove();

	 if(buttonPressed == 6)
		{
		ExecuteBackButton();
		buttonPressed = 0;
		 thinking = 0;
		 return 0;
		}

	 thinking = 0;
	 if (ee == 1 )
	 {
/*     	
	 PutMoveString(OptimalMove.SqFrom, OptimalMove.SqTo,OptimalMove.PieceMoved,
		 OptimalMove.PieceTaken,OptimalMove.Indicators);
     printf("\n      %s  MiMax = %5d  Nodes = %10d ",moveout, OptimalMove.MoveValue,generatedTotal );
//	 PlayMove(&OptimalMove);
*/
	 }
	 return ee;
	}
}


// Getc() while nrchar >= 4 and then verify input string and get a square if OK
int GetMoveString( int &MoveToDo )
{

	int xx;

int PieceMoved;
int PieceTaken;
int SqFrom;
int SqTo;
int Indicators;


    SqFrom = 0;
    SqTo = 0;
	PieceMoved = 0;
	PieceTaken = 0;
    MoveToDoMoveValue = 0;

  SqFrom = SqFromActive;
  SqTo   = SqToActive;
  PieceMoved = tabla[SqFrom];
  PieceTaken = tabla[SqTo];
  Indicators = 0;
  if ( (SqFrom == 25) && (SqTo == 27 ) && (PieceMoved == WK))
  {
     Indicators = 1; // white rocade
  }
  if ( (SqFrom == 25) && (SqTo == 23 ) && (PieceMoved == WK))
  {
     Indicators = 1; // white rocade
  }
  if ( (SqFrom == 95) && (SqTo == 97 ) && (PieceMoved == BK))
  {
     Indicators = 2; // black rocade
  }
  if ( (SqFrom == 95) && (SqTo == 93 ) && (PieceMoved == BK))
  {
     Indicators = 2; // black rocade
  }

  xx = SqFrom - SqTo;
 
  xx = SqFrom - SqTo;
 
  if( ((xx == 9) || (xx == 11) || xx == 10) && 
	  (PieceMoved == BP) && (SqTo < 30)) 
  {
	Indicators = 9; // black transform in quen
  }
  if( ((xx == -9) || (xx == -11) || xx == -10) && 
	  (PieceMoved == WP)&& (SqTo > 90) ) 
  {
	Indicators = 5; // white transform in quen
  }

  MoveToDo = (Indicators << 24) | (SqFrom << 8) | SqTo;

  SqFromActive = 0;
  SqToActive = 0;

  return 1;

}

int MoveToStringPNG(int, char[]);

void SaveGame ( HWND hwnd )
	{
	char buffer[4196];


	int numberFrom;
	int mv;
	char whitemove[20];
	char blackmove[20];
    int n;

    char movesstring[30];

//	int xa,ya,xb,yb;

strcpy (buffer, "Salvare partida.pgn");
//if (GetFileName (hwnd, 1, buffer, saveFile) == TRUE)
 {
 time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];
  char tbuffer [80];
  time (&rawtime);
  timeinfo = localtime (&rawtime);

  for(n=0;n<24;n++)
		movesstring[n] = ' '; // make clear the string
  
  sprintf (tbuffer,"Date %s \n",ctime (&rawtime));
    sprintf (tbuffer,"[Date \"1984.??.??\" ] \n");
  sprintf (buffer,"Partida la Nivelul %2d Agesivitate %2d  .pgn", Options.nTime + 1, Options.agresivity + 1);

	fpgn = fopen(buffer,"wt+");
	
	strcpy(buffer,"[Event \" Test \"] \n");
	fprintf(fpgn,buffer);
	
	strcpy(buffer,"[Site \"Home\" ]\n");
	fprintf(fpgn,buffer);
	
	fprintf(fpgn,tbuffer);
	
	strcpy(buffer,"[White \"White\"]\n");
	fprintf(fpgn,buffer);
	
	strcpy(buffer,"[Black \"Black\"]\n");
	fprintf(fpgn,buffer);
	//sprintf(buffer,"[Result \"%s\"]\n",rezultat);
	 strcpy(buffer,"[Result \"1/2-1/2\" ]\n");
	fprintf(fpgn,buffer);
	
	strcpy(buffer,"[White Elo \"2000\" ]\n");
	fprintf(fpgn,buffer);
	
	strcpy(buffer,"[Black Elo \"2000\" ]\n");
	fprintf(fpgn,buffer);
	
	strcpy(buffer,"[ECO \"E17\" ]\n");
	fprintf(fpgn,buffer);
	
	if(fpgn > 0)
	{
// write header of game
/*  
[Event "World Championship 31th-KK1"]
[Site "Moscow"]
[Date "1984.??.??"]
[Round "2"]
[White "Kasparov, Gary"]
[Black "Karpov, Anatoly"]
[Result "1/2-1/2"]
[WhiteElo "2715"]
[BlackElo "2705"]
[ECO "E17"]	
*/
	
	for(n=0;n<24;n++)
		movesstring[n] = ' '; // make clear the string
	int ncrt = 0;
	numberFrom = 0 ;
	for(n=numberFrom;n<=(treeDepth - 1);n=n+2)
		{
			ncrt++;
			if(ncrt > 300) break;
	    strcpy(whitemove,"          ");
	    strcpy(blackmove,"          ");

		mv = n / 2 + 1;
		MoveToStringPNG(Stack2[n    ].move, whitemove);
        if(n<=(treeDepth - 2))
			MoveToStringPNG(Stack2[n + 1].move, blackmove);
        sprintf(movesstring, " %3d. %s  %s  ", mv, whitemove, blackmove);
	int no = 0;
 	  if((whitemove[0] == '0' && whitemove[1] != '-') || ( blackmove[0] == '0' && blackmove[1] != '-'))
		  no = 1;
	if(no == 0)	  
	 {
		  fprintf(fpgn,"%s ",movesstring);
		
		  sprintf(movesstring, "  %s  %s  \n",Stack2[n].info, Stack2[n+1].info);
		  fprintf(fpgn,"%s ",movesstring);	   
	  }
	}
 	
	fprintf(fpgn,"[Alb - Timp     : Total = %d sec, Mediu = %d sec ] \n",sumatimpialb,timpmediualb);
	fprintf(fpgn,"[      Mutari   : Total = %d K  , Mediu = %d K] \n",nodesmutarialb,nodesmediualb);	
	fprintf(fpgn,"[Negru - Timp   : Total = %d sec, Mediu = %d sec ] \n",sumatimpinegru,timpmediunegru);
	fprintf(fpgn,"[        Mutari : Total = %d K  , Mediu = %d K] \n",nodesmutarinegru,nodesmediunegru);		
 /*
		fprintf(fpgn,"[  \n    Alte informatii : \n");	
	fprintf(fpgn,"[        Coduri < 100 = %d K ]\n",(int)(over_100/ 1000.));
	fprintf(fpgn,"[       Zero Level = %d  K ]\n",(int)(zero_level/ 1000.));	
	fprintf(fpgn,"[        Run From Attack = %d K ] \n",(int)(from_attack / 1000.));
	fprintf(fpgn,"[       Found HI = %d K ]\n",(int)(egal_hi / 1000.));	
	fprintf(fpgn,"[        Take big = %d K ] \n",(int)(take_big / 1000.));
	fprintf(fpgn,"[       Take small = %d K ] \n",(int)(take_small / 1000.));	
	fprintf(fpgn,"[        Take Under attack = %d K ]\n",(int)(under_attack / 1000.));
	fprintf(fpgn,"[       Call Lite = %d K ] \n",(int)(egal_lite / 1000.));	
	fprintf(fpgn,"[       Move to attack = %d K] \n",(int)(egal_penalties / 1000.));
*/
	 
	fprintf(fpgn,"[   \n\n   Nodes Exchenge = %d K] \n",(int)(mutari_excenge / 1000.));

	fprintf(fpgn,"[     \n\n");
	

	fprintf(fpgn,"[   \n    Total_abserch = %d K ] \n",(int)(total_abserch / 1000.));
	
	fprintf(fpgn,"[      Total_absearch_peste_kmax = %d K] \n",(int)(total_absearch_peste_kmax / 1000.));
	
	fclose(fpgn);
	MessageBox(hWnd," Salvare partida in fisierul .pgn este OK"," Salvare Partida", MB_OK);
	return ;
	}
else
	{
	MessageBox(hWnd,"Eroare salvare partida in fisier"," Salvare Partida", MB_OK);
	return ;
	}
 }
}

// ==============================================================
//   Pawn Promotion Dialog Procedure
// ==============================================================
BOOL CALLBACK PawnDlgProc( HWND hwnd, UINT msg,  UINT wparam, LONG lparam )
{
 lparam = lparam;                    
  int upgrade = WQ;
 switch( msg ) {
    case WM_INITDIALOG:

        return( TRUE );

    case WM_COMMAND:
 
        if( LOWORD( wparam ) == 100 )  
                          { upgrade = WQ;
                            EndDialog( hwnd, TRUE );
                            return( TRUE );
                          }
        if( LOWORD( wparam ) == 101 )  
                          { upgrade = WR;
                            EndDialog( hwnd, TRUE );
                            return( TRUE );
                          }
        if( LOWORD( wparam ) == 102 )  
                          { upgrade = WB;
                            EndDialog( hwnd, TRUE );
                            return( TRUE );
                          }
        if( LOWORD( wparam ) == 103 )  
                          { upgrade = WN;
                            EndDialog( hwnd, TRUE );
                            return( TRUE );
                          }                                                    
           
} // switch  
 return (FALSE);
}
