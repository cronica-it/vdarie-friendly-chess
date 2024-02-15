
int totPower ;
int MoveFromCheck;
int GenMovesList(int indExchange);
//
// G e n L e v e l M o v e s - Generare moves on an given table status
//

// Se apeleaza doar functii din acest modul

int GenLevelMoves(int indExchange)
{

    GenMovesList(indExchange);


	Stack2[gameDepth].nbAttacks = pma - pma0;
	Stack2[gameDepth].movesOnLevel = Stack2[gameDepth].nbAttacks ;

	Stack2[gameDepth].rankOnLevel   = 0;
	Stack2[gameDepth].rankAttacks   = 0;
	return 0;
}

//
// Generation Moves List for all chessboard
//

int GenMovesList(int indExchange)
{
int i,j;

int np;

	pma0 = (int *)&Stack2[gameDepth].Attacks[0];
	pma = pma0;

  if(who == 0)
	{

		for(i=1;i<=6;i++)
		{
			np = Stack[gameDepth].pieceCounts[i];
			if(np == 0) continue;

			for(j=0;j<np;j++)
			{
				GenMovesForAWhitePiece(indExchange,i, Stack[gameDepth].pieceAPosition[i][j]);
			}
		}
	}
	else
	{
		for(i=7;i<=12;i++)
		{
			np = Stack[gameDepth].pieceCounts[i];
			if(np == 0) continue;

			for(j=0;j<np;j++)
			{
				GenMovesForABlackPiece(indExchange,i, Stack[gameDepth].pieceAPosition[i][j]);
			}
		}
	}
/*
  if(who == 0)
	{
		for(i=98;i>=21;i--)
		{
			if(tabla[i] <= 0 || tabla[i] >= BP) continue;
			GenMovesForAWhitePiece(tabla[i], i);
		}
	}
	else
	{
		for(i=21;i<=98;i++)
		{
			if(tabla[i] < BP) continue;
			GenMovesForABlackPiece(tabla[i], i);
		}
	}
*/
return 0;
}

//
// Moves Generation for a single White piece
//

int inline GenMovesForAWhitePiece(int indExchange,int PieceMoved,int SqFrom)
{
int p3;
int cc;

int roc;

//int bad;

int increment;

int PieceTaken;
int SqTo;

//MoveFromCheck = Stack2[gameDepth].IsCheckOwn;

cc = (PieceMoved << 20) | (SqFrom << 8);
if (PieceMoved == WP)
{

if(SqFrom < 40)
{


	if (( tabla[SqFrom + 10] == 0) && (tabla[SqFrom + 20] == 0) )
	   {
		   if(indExchange == 0)
		   {

			*pma = cc +  (SqFrom + 20);
			pma++;
		   }
	   }
}

if ( tabla[SqFrom + 10] == 0 )
	   {
		   if(SqFrom < 80)
		   {
			   if(indExchange == 0 )
			   {

			      *pma = cc |  (SqFrom + 10);
				pma++;
			   }
		   }
		   else
		   {
		   // transform

			   *pma = cc | (5 << 24) | (SqFrom + 10);
				pma++;

if(indExchange == 0)
{
				*pma = cc | (6 << 24) | (SqFrom + 10);
				pma++;


			   *pma = cc | (7 << 24) | (SqFrom + 10);
				pma++;


			   *pma = cc | (8 << 24) | (SqFrom + 10);
				pma++;
}
		   }
	   }
	if ( tabla[SqFrom + 9] > WK)
	   {
	   PieceTaken = tabla[SqFrom + 9];

	   if(SqFrom < 80)
	   {

		   *pma = cc | (PieceTaken << 16) | (SqFrom + 9);
			pma++;
	   }
	   else
		   {
		   // transform

		   *pma = cc | (5 << 24) | (PieceTaken << 16) | (SqFrom + 9);
			pma++;

if(indExchange == 0)
{
		   *pma = cc | (6 << 24) | (PieceTaken << 16) | (SqFrom + 9);
			pma++;


		   *pma = cc | (7 << 24) | (PieceTaken << 16) | (SqFrom + 9);
			pma++;


		   *pma = cc | (8 << 24) | (PieceTaken << 16) | (SqFrom + 9);
			pma++;
}
		   }
	   }
	if ( tabla[SqFrom + 11] > WK)
	   {
	   PieceTaken = tabla[SqFrom + 11];

	   if(SqFrom < 80)
	   {

		   *pma = cc | (PieceTaken << 16) | (SqFrom + 11);
			pma++;
	   }
	   else
		   {
		   // transform

		   *pma = cc | (5 << 24) | (PieceTaken << 16) | (SqFrom + 11);
			pma++;

if(indExchange == 0)
{
		   *pma = cc | (6 << 24) | (PieceTaken << 16) | (SqFrom + 11);
			pma++;

		   *pma = cc | (7 << 24) | (PieceTaken << 16) | (SqFrom + 11);
			pma++;

		   *pma = cc | (8 << 24) | (PieceTaken << 16) | (SqFrom + 11);
			pma++;
}
		   }
	   }

	if ( (gameDepth > 0 ) && (tabla[SqFrom - 1 ] == BP) &&
	  (((Stack2[gameDepth - 1].move > 8) & 255) == (SqFrom - 1 + 20)  &&  (Stack2[gameDepth - 1].move & 255) == (SqFrom - 1) ) )
	   {

		   *pma = cc | (3 << 24) |  (SqFrom + 9);
			pma++;

	   }
	if ( (gameDepth > 0 ) && (tabla[SqFrom  + 1] == BP) &&
      (((Stack2[gameDepth - 1].move >> 8) & 255) == (SqFrom + 1 + 20)  &&  (Stack2[gameDepth - 1].move & 255) == (SqFrom + 1) ) )
	   {

		   *pma = cc | (3 << 24) |  (SqFrom + 11);
			pma++;
	   }

} // if WP
else
	{

	switch (PieceMoved) {

    case WR:
    
	for(p3 = 0; p3 < 4; p3++)
	{
	SqTo = SqFrom;
	increment = dr[p3];
	for(;;)
	 {
	 SqTo = SqTo + increment;
	 PieceTaken = tabla[SqTo];

	 if(whiteTake[PieceTaken + 2] == 0) break;

	 if(indExchange > 0 && PieceTaken == 0) continue; // exchange -> only attacks are generated

	 if(PieceTaken == 0)
	 {

		   *pma = cc +  SqTo;
			pma++;

		continue;
	 }
	 else
	 {
		*pma = cc |  (PieceTaken << 16) | SqTo;
		pma++;
		break;
	 }
	 } // end for on a direction

	} // end for directions for a piece
    
	break;

    case WN:
    
	for(p3 = 0; p3 < 8; p3++)
	{
	 SqTo = SqFrom + dn[p3];
	 PieceTaken = tabla[SqTo];

	 if(whiteTake[PieceTaken + 2] == 0) continue;

	 if(indExchange > 0 && PieceTaken == 0) continue;

	 if(PieceTaken == 0 )
	 {

		*pma = cc +   SqTo;
		pma++;
		continue;
	 }
	 else
	 {
		*pma = cc |  (PieceTaken << 16) | SqTo;
		pma++;

		continue;
	 }

	} // end for direction for a piesa
    
	break;

    case WB:
	for(p3 = 0; p3 < 4; p3++)
	{
	SqTo = SqFrom;
	increment = db[p3];
	for(;;)
	 {
	 SqTo = SqTo + increment;
	 PieceTaken = tabla[SqTo];

	 if(whiteTake[PieceTaken + 2] == 0) break;

	 if(indExchange > 0 && PieceTaken == 0) continue; // exchange -> only attacks are generated

	 if(PieceTaken == 0)
	 {

		*pma = cc +   SqTo;
		pma++;
		continue;
	 }
	 else
	 {
		*pma = cc |  (PieceTaken << 16) | SqTo;
		pma++;
		break;
	 }

	 } // end for pe o directie

	} // end for directii o piesa
      break;

    case WQ:
	for(p3 = 0; p3 < 8; p3++)
	{
	SqTo = SqFrom;
	increment = dq[p3];
	for(;;)
	 {
	 SqTo = SqTo + increment;
	 PieceTaken = tabla[SqTo];

	 if(whiteTake[PieceTaken + 2] == 0) break;

	 if(indExchange > 0 && PieceTaken == 0) continue; // exchange -> only attacks are generated

	 if(PieceTaken == 0)
	 {

		*pma = cc +  SqTo;
		pma++;
		continue;
	 }
	 else
	 {

		*pma = cc |  (PieceTaken << 16) | SqTo;
		pma++;
		break;
	 }
	 } // end for pe o directie

	} // end for directii o piesa
      break;

    case WK:
      // First test Rocades
		if(tabla[25] == WK)
		{
		  roc = Stack[gameDepth].RocadeIndicators;
		
		  if ((SqFrom == 25) && (tabla[SqFrom] == WK) && 
			  (tabla[SqFrom - 1] == 0 ) && (tabla[SqFrom - 2] == 0) &&
			(tabla[SqFrom - 3] == 0)  && ((roc & 32) != 0)  && ((roc & 2) != 0))
		  {
			  if((Stack2[gameDepth].IsCheckOwn == 0) &&  (TablaWhiteIsCheck(SqFrom - 1) == 0) && 
			(TablaWhiteIsCheck(SqFrom - 2) == 0))
				{

				   *pma = cc |  (1 << 24 ) |  23;
				pma++;
				}
		  }
		  if ((SqFrom == 25) && (tabla[SqFrom] == WK) && 
			(tabla[SqFrom + 1] == 0 ) && (tabla[SqFrom + 2] == 0) &&
			(tabla[SqFrom + 3] == WR) && ((roc & 16) != 0)  && ((roc & 2) != 0))
		  {
			  if((Stack2[gameDepth].IsCheckOwn == 0) &&
				(TablaWhiteIsCheck(SqFrom + 1) == 0) && (TablaWhiteIsCheck(SqFrom + 2) == 0 ))
				{

				   *pma = cc |  (1 << 24) |  27;
					pma++;

				}
		  }
		}

	for(p3 = 0; p3 < 8; p3++)
	  {
	  SqTo = SqFrom + dk[p3];
	  PieceTaken = tabla[SqTo];

	  if(whiteTake[PieceTaken + 2] == 0) continue;

	 if(indExchange > 0 && PieceTaken == 0) continue; // exchange -> only attacks are generated
		
	 if(WhiteTestBad(SqTo,WK,PieceTaken) >= 64)		continue;
				

	 if(PieceTaken == 0 )
	 {

		   *pma = cc |  SqTo;
			pma++;
		  continue;
	 }
	 else
	 {

		   *pma = cc |  (PieceTaken << 16) | SqTo;
			pma++;
			continue;
	 }

	  continue;


	} // end for directii o piesa rege alb
	} // end switch;
	} // end else
	return 0;
} // end moves generating for a White piece


//
// Move generation for a single Black piece
//
int inline GenMovesForABlackPiece(int indExchange,int PieceMoved,int SqFrom)
{
int p3;

int roc;

int cc;

int increment;
int PieceTaken;
int SqTo;


//MoveFromCheck = Stack2[gameDepth].IsCheckOwn;

cc = (PieceMoved << 20) | (SqFrom << 8);

if (PieceMoved == BP)
{


if(SqFrom > 80)
{
	
	if (( tabla[SqFrom - 10] == 0) && (tabla[SqFrom - 20] == 0) )
	   {
		   if(indExchange == 0)
		   {

			*pma = cc + (SqFrom - 20);
			pma++;
		   }
	   }
}
if ( tabla[SqFrom - 10] == 0)
	   {
	   if(SqFrom > 40)
		   {
			   if(indExchange == 0 )
			   {

				*pma = cc | (SqFrom - 10);
				pma++;
			   }
		   }
	   else
		   {
		   // transform

				*pma = cc | (9 << 24) | (SqFrom - 10);
				pma++;
if(indExchange == 0)
{
				*pma = cc | (10 << 24) | (SqFrom - 10);
				pma++;

				*pma = cc | (11 << 24) | (SqFrom - 10);
				pma++;

				*pma = cc | (12 << 24) | (SqFrom - 10);
				pma++;
}
		   }
	   }

	if ( (tabla[SqFrom - 9] > 0 ) && ( tabla[SqFrom - 9] < BP))
	    {
	   PieceTaken = tabla[SqFrom - 9];

	   if(SqFrom > 40)
	   {

				*pma = cc | (PieceTaken << 16) |  (SqFrom - 9);
				pma++;
	   }
	   else
		   {
		   // transform

				*pma = cc | (9 << 24) | (PieceTaken << 16) |  (SqFrom - 9);
				pma++;

if(indExchange == 0)
{
				*pma = cc | (10 << 24) | (PieceTaken << 16) |  (SqFrom - 9);
				pma++;

				*pma = cc | (11 << 24) | (PieceTaken << 16) |  (SqFrom - 9);
				pma++;


				*pma = cc | (12 << 24) | (PieceTaken << 16) |  (SqFrom - 9);
				pma++;
}
		   }
	    }

	if ( (tabla[SqFrom - 11] > 0 ) && ( tabla[SqFrom - 11] < BP))
	    {
	   PieceTaken = tabla[SqFrom - 11];

	   if(SqFrom > 40)
	   {

				*pma = cc |  (PieceTaken << 16) |  (SqFrom - 11);
				pma++;
	   }
	   else
		   {
		   // transform

				*pma = cc |  (9 << 24) | (PieceTaken << 16) |  (SqFrom - 11);
				pma++;

if(indExchange == 0)
{
				*pma = cc |  (10 << 24) | (PieceTaken << 16) |  (SqFrom - 11);
				pma++;


				*pma = cc |  (11 << 24) | (PieceTaken << 16) |  (SqFrom - 11);
				pma++;


				*pma = cc |  (12 << 24) | (PieceTaken << 16) |  (SqFrom - 11);
				pma++;
}
		   }
	    }

	if ( (gameDepth > 0 ) && (tabla[SqFrom - 1] == WP) &&
	     ((Stack2[gameDepth - 1].move >> 8) & 255) == (SqFrom - 1 - 20) && (Stack2[gameDepth - 1].move & 255) == (SqFrom - 1)  )
	    {

				*pma = cc |  (4 << 24) |  (SqFrom - 11);
				pma++;
	    }
		if ( (gameDepth > 0 ) && (tabla[SqFrom + 1] == WP) &&
 	     ((Stack2[gameDepth - 1].move >> 8) & 255) == (SqFrom + 1 - 20) && (Stack2[gameDepth - 1].move & 255) == (SqFrom + 1))
	    {

				*pma = cc |  (4 << 24) |   (SqFrom - 9);
				pma++;
	    }

} // end BP
else
{ 
	switch (PieceMoved) {

    case BR:
    
	for(p3 = 0; p3 < 4; p3++)
	{
	SqTo = SqFrom;
	increment = dr[p3];
	for(;;)
	 {
	 SqTo = SqTo + increment;
	 PieceTaken = tabla[SqTo]; // alt patrat pe directie

	 if(blackTake[PieceTaken + 2] == 0) break;

	 if(indExchange > 0 && PieceTaken == 0) continue; // exchange -> only attacks are generated

	 if(PieceTaken == 0)
	 {
		*pma = cc |  SqTo;
		pma++;
		continue;
	 }
	 else
	 {

			*pma = cc |  (PieceTaken << 16) |  SqTo;
			pma++;

		break;
	 }

	 } // end for pe o directie

	} // end for directii o piesa
      break;

    case BN:
	for(p3 = 0; p3 < 8; p3++)
	{
	SqTo = SqFrom + dn[p3]; // directii cal
	PieceTaken = tabla[SqTo];


	 if(blackTake[PieceTaken + 2] == 0) continue;

	 if(indExchange > 0 && PieceTaken == 0) continue; // exchange -> only attacks are generated

	 if(PieceTaken == 0)
	 {
		*pma = cc | SqTo;
		pma++;
		continue;
	 }
	 else
	 {
			*pma = cc |  (PieceTaken << 16) |  SqTo;
			pma++;

		continue;
	 }

	} // end for directions of knight
      break;

    case BB:

	for(p3 = 0;p3 < 4; p3++)
	{

	SqTo = SqFrom;
	increment = db[p3];

	for(;;)
	 {
	 SqTo = SqTo + increment;
	 PieceTaken = tabla[SqTo];

	 if(blackTake[PieceTaken + 2] == 0) break;

	 if(indExchange > 0 && PieceTaken == 0) continue; // exchange -> only attacks are generated

	 if(PieceTaken == 0)
	 {
		*pma = cc |   SqTo;
		pma++;
		continue;
	 }
	 else
	 {

			*pma = cc |  (PieceTaken << 16) |  SqTo;
			pma++;

		break;
	 }
	 } // end for a direction

	} // end for directions for a piece
      break;

    case BQ:
    
	for(p3 = 0; p3 < 8; p3++)
	{
	SqTo = SqFrom; // alta directie

	increment = dq[p3];

	for(;;)
	 {
	 SqTo = SqTo + increment;
	 PieceTaken = tabla[SqTo]; // pozitii pe o directie

	 if(blackTake[PieceTaken + 2] == 0) break;

	 if(indExchange > 0 && PieceTaken == 0) continue; // exchange -> only attacks are generated

	 if(PieceTaken == 0)
	 {
		*pma= cc |   SqTo;
		pma++;
		continue;
	 }
	 else
	 {

			*pma = cc |  (PieceTaken << 16) |  SqTo;
			pma++;

		break;
	 }
	 } // end for pe o directie

	} // end for directii o piesa
      break;

    case BK:
      // First test Rocades
	if(tabla[95] == BK)
	{
		roc = Stack[gameDepth].RocadeIndicators;

		if ((SqFrom == 95) && (tabla[SqFrom] == BK) && 
		  (tabla[SqFrom - 1] == 0 ) && (tabla[SqFrom - 2] == 0) &&
		  (tabla[SqFrom - 3] == 0) && ( (roc & 8) != 0)   && ((roc & 1) != 0))
		{
			if(	(Stack2[gameDepth].IsCheckOwn == 0) &&  (TablaBlackIsCheck(SqFrom - 1) == 0 ) && 
			(TablaBlackIsCheck(SqFrom - 2) == 0))
			{
				*pma = cc |  (2 << 24) |  93;
				pma++;
			}
		}
		if ((SqFrom == 95) && (tabla[SqFrom] == BK) && 
		  (tabla[SqFrom + 1] == 0 ) && (tabla[SqFrom + 2] == 0) &&
		  (tabla[SqFrom + 3] == BR) && ((roc & 4) != 0) && ( (roc & 1) != 0))
		{
		  if(	(Stack2[gameDepth].IsCheckOwn == 0) &&
		   (TablaBlackIsCheck(SqFrom + 1) == 0)  && (TablaBlackIsCheck(SqFrom + 2) == 0 ) )
			{
				*pma = cc | (2 << 24) |  97;
				pma++;
			}
		}
	}
for(p3 = 0;p3 < 8; p3++)
	{
	increment = dk[p3];

	SqTo = SqFrom + increment; // pozitii succesive pe directiile rege
	PieceTaken = tabla[SqTo];

	if(blackTake[PieceTaken + 2] == 0) continue;

	 if(indExchange > 0 && PieceTaken == 0) continue; // exchange -> only attacks are generated
		
	if(BlackTestBad(SqTo,BK,PieceTaken) >= 64)		continue;

	if(PieceTaken == 0)
	{
		*pma = cc |   SqTo;
		pma++;
		continue;
	}
	else
	{
		*pma = cc |  (PieceTaken << 16) |  SqTo;
		pma++;
		continue;
	}


	continue;

	} // end for directii rege
   } // End Swith piese 
   } // end else

  return 0;

} // end rutina GenMovesForABlackPiece()

int inline WhiteTestBad(int SqTo,int PieceMoved,int PieceTaken)
{
//	if(tabla_black_power_attacks[SqTo] > 0) return 64;
//	else return 0;
	
//return 0;
  if(BlackTakeLikePawn[tabla[SqTo + 9] + 1] != 0 || BlackTakeLikePawn[tabla[SqTo + 11] + 1] != 0 || 
	  BlackTakeLikeRook[tabla[SqTo + 10] + 1] != 0 || tabla[SqTo + 10] == 0 &&
	   BlackTakeLikeRook2[tabla[SqTo + 20] + 1] != 0 || tabla[SqTo + 19] == BN || tabla[SqTo + 21] == BN ||
		tabla[SqTo + 8] == BN || tabla[SqTo + 12] == BN)
  {  
	  if((-ValPiesa[PieceTaken] - 50) >= ValPiesa[PieceMoved]) 
		return 64;
	  else
		return 128;
  }
  return 0;
}

int inline BlackTestBad(int SqTo,int PieceMoved,int PieceTaken)
{
//	if(tabla_white_power_attacks[SqTo] > 0) return 64;
//	else return 0;	
//return 0;
  if(WhiteTakeLikePawn[tabla[SqTo - 9] + 1] != 0 || WhiteTakeLikePawn[tabla[SqTo - 11] + 1] != 0 || 
	  WhiteTakeLikeRook[tabla[SqTo - 10] + 1] != 0 || tabla[SqTo - 10] == 0 &&
	   WhiteTakeLikeRook2[tabla[SqTo - 20] + 1] != 0 || tabla[SqTo - 19] == WN || tabla[SqTo - 21] == WN ||
		tabla[SqTo - 8] == WN || tabla[SqTo - 12] == WN) 
  {
  if((ValPiesa[PieceTaken] - 50) >= (-ValPiesa[PieceMoved])) 
	  return 64;
  else
	  return 128;
  }
  return 0;
}

