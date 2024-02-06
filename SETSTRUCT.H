
//
// SETSTRUCT INITIALIZATIONS
//

int posType[13] = {0, 1,4,2,3,5,6, 7,10,8,9,11,12};

//
// Set Piece Chain
//
int SetStruct()
{

if(gameDepth == treeDepth)
  CompStructBase();
return 0;
}

//
// MakePIeceList - for some routines like MateRepository()
//

int MakePieceList()
{
int i;

int t;



int SqTo;
int SqFrom;
int PieceTaken;
int PieceMoved;

	SqFrom = (Stack2[gameDepth-1].move >> 8) & 255;
	SqTo   = (Stack2[gameDepth-1].move & 255);
	PieceMoved = (Stack2[gameDepth-1].move >> 20) & 15;
	PieceTaken = (Stack2[gameDepth-1].move >> 16) & 15;

  Stack[gameDepth].nWP = 0;

  Stack[gameDepth].nBP = 0;

if(gameDepth == treeDepth)
  CompStructBase();

  Stack[gameDepth].nWP = 0;

  Stack[gameDepth].nBP = 0;


for(i=0;i<13;i++)
	piecesCount[i][0] = 0;




for(i=21;i<99;i++)
    {
	t = tabla[i];
    if ( t <= 0) continue;

	int t1 = posType[t]; // internal types indexes translation
	piecesCount[t1][0]++;
	piecesCount[t1][piecesCount[t1][0]] = i;

	piecesCount[t1][19] = t;


    Stack[gameDepth].nWP += xnWP[t]; // t is piece code
    Stack[gameDepth].nBP += xnBP[t];



} // for i= pieses on board

nwhite = 0;
nblack = 0;


for(i=6;i>0;i--)
{
	if(piecesCount[i][0] == 0) continue;

	for(j=1;j<=piecesCount[i][0];j++)
	{
			lwhite[nwhite] = piecesCount[i][19]; //  type of piece
			lwhite[nwhite + 1] = piecesCount[i][j];
			nwhite = nwhite + 3;
	}
}
for(i=12;i>=7;i--)
{
	if(piecesCount[i][0] == 0) continue;

	for(j=1;j<=piecesCount[i][0];j++)
	{
			lblack[nblack] = piecesCount[i][19]; //  type of piece
			lblack[nblack + 1] = piecesCount[i][j];
			nblack = nblack + 3;
	}
}

lwhite[nwhite] = 0;
lblack[nblack] = 0;   

return 0;

}
//
// Compute Struct info  at BASE of the Base

int CompStructBase()
{
int i;

int t,v;
int aa;
// int checkSum, checkSum2;

  Stack[gameDepth].nWP = 0;

  Stack[gameDepth].nBP = 0;


for(i=0;i<13;i++)
{
	Stack[gameDepth].pawnWOnFiles[i] = 0;
	Stack[gameDepth].pawnBOnFiles[i] = 0;
	Stack[gameDepth].pieceCounts[i] = 0;
	Stack[gameDepth].pieceAPosition[i][0] = 0;
	Stack[gameDepth].pieceAPosition[i][1] = 0;
	Stack[gameDepth].pieceAPosition[i][2] = 0;
	Stack[gameDepth].pieceAPosition[i][3] = 0;
	Stack[gameDepth].pieceAPosition[i][4] = 0;
	Stack[gameDepth].pieceAPosition[i][5] = 0;
	Stack[gameDepth].pieceAPosition[i][6] = 0;
	Stack[gameDepth].pieceAPosition[i][7] = 0;
}
for(i=21;i<99;i++)
 {
	t = tabla[i];
    if ( t <= 0) continue;

	Stack[gameDepth].pieceAPosition[t][Stack[gameDepth].pieceCounts[t]] = i;
	Stack[gameDepth].pieceCounts[t]++;
	Stack[gameDepth].pieceAPosition[t][Stack[gameDepth].pieceCounts[t]] = 0;
		
	Stack[gameDepth].nWP += xnWP[t]; // t is piece code
	Stack[gameDepth].nBP += xnBP[t];
	if(t == WP) Stack[gameDepth].pawnWOnFiles[(i - (i % 10)) / 10 - 1]++;
	if(t == BP) Stack[gameDepth].pawnBOnFiles[(i - (i % 10)) / 10 - 1]++;


 } // for i= pieses on board

  npiese = 0;
  materialPonder = 0;
  
//  checkSum = 0;
//  checkSum2= 0;

  Stack[gameDepth].whitePower = 0;
  Stack[gameDepth].blackPower = 0;
  Stack[gameDepth].nPieces = 0;

  totalMatePower = 0;

  for(i=0;i<13;i++)
	piecesCount[i][0] = 0;


    Stack[gameDepth].nWMinors = Stack[gameDepth].pieceCounts[WN] + Stack[gameDepth].pieceCounts[WB];
    Stack[gameDepth].nBMinors = Stack[gameDepth].pieceCounts[BN] + Stack[gameDepth].pieceCounts[BB];

	Stack[gameDepth].whiteKingSquare = Stack[gameDepth].pieceAPosition[WK][0];
	Stack[gameDepth].blackKingSquare = Stack[gameDepth].pieceAPosition[BK][0];


	for(i=21;i<99;i++)
    {
	    
	t = tabla[i];
    if ( t <= 0) continue;
    v = tso[t];

    aa = ValPiesa[t]; // Trait the weakness of chess level of player

    materialPonder = materialPonder + aa; //piece material value

	Stack[gameDepth].nPieces++;

	int t1 = posType[t]; // internal types indexes translation
	piecesCount[t1][0]++;
	piecesCount[t1][piecesCount[t1][0]] = i;

	piecesCount[t1][19] = t;


	totalMatePower = totalMatePower + matPower[t];

	if ( t < BP) Stack[gameDepth].whitePower += ValPiesa[t];
    else         Stack[gameDepth].blackPower -= ValPiesa[t];    


	if(t == WK) Stack[gameDepth].whiteKingSquare = i;
	if(t == BK) Stack[gameDepth].blackKingSquare = i;

//	checkSum += checkSum  + TabCheck[t][i];
//	checkSum2 += checkSum2 + TabCheck2[t][i] + 1;

	}

Stack[gameDepth].ponder = materialPonder;

//Stack[gameDepth].checkSumPosition  = checkSum;
//Stack[gameDepth].checkSumPosition2 = checkSum2;

return 0;

} // end Comp Struct Base


int inline RebuildPiecesPositions()
{
int t;
Stack[gameDepth].nWP = 0;

Stack[gameDepth].nBP = 0;

ZeroMemory ((char *)&Stack[gameDepth].pieceAPosition[0][0], sizeof (Stack[gameDepth].pieceAPosition ));
ZeroMemory ((char *)&Stack[gameDepth].pieceCounts[0], sizeof (Stack[gameDepth].pieceCounts ));
ZeroMemory ((char *)&Stack[gameDepth].pawnWOnFiles[0], sizeof (Stack[gameDepth].pawnWOnFiles ));
ZeroMemory ((char *)&Stack[gameDepth].pawnBOnFiles[0], sizeof (Stack[gameDepth].pawnBOnFiles[i] ));


Stack[gameDepth].positionVal = 0;
for(i=21;i<99;i++)
	{
		t = (int)tabla[i];
		if ( t <= 0) continue;

		Stack[gameDepth].pieceAPosition[t][Stack[gameDepth].pieceCounts[t]] = i;
		Stack[gameDepth].pieceCounts[t]++;
		Stack[gameDepth].pieceAPosition[t][Stack[gameDepth].pieceCounts[t]] = 0;

		Stack[gameDepth].nWP += xnWP[t]; // t is piece code
		Stack[gameDepth].nBP += xnBP[t];
		if(t == WP) Stack[gameDepth].pawnWOnFiles[(i - (i % 10)) / 10 - 1]++;
		if(t == BP) Stack[gameDepth].pawnBOnFiles[(i - (i % 10)) / 10 - 1]++;
		switch (t) {
			case WP:
				Stack[gameDepth].positionVal += WPawnPos[i];
				break;
			case WR:
				Stack[gameDepth].positionVal += WRookValue[i];
				break;
			case WN:
				Stack[gameDepth].positionVal += WKnightPost[i];
				break;
			case WB:
				Stack[gameDepth].positionVal += WBishopValue[i];
				break;
			case WQ:
				Stack[gameDepth].positionVal += WQueenValue[i];
				break;
			case WK:
				if((Stack[gameDepth].whitePower +  Stack[gameDepth].blackPower) > 2000)
					Stack[gameDepth].positionVal += WKingValue[i];
				else
					Stack[gameDepth].positionVal += WKingValueEnd[i];
				break;
			case BP:
				Stack[gameDepth].positionVal -= WPawnPos[Simetric[i]];
				break;
			case BR:
				Stack[gameDepth].positionVal -= WRookValue[Simetric[i]];
				break;
			case BN:
				Stack[gameDepth].positionVal -= WKnightPost[Simetric[i]];
				break;
			case BB:
				Stack[gameDepth].positionVal -= WBishopValue[Simetric[i]];
				break;
			case BQ:
				Stack[gameDepth].positionVal -= WQueenValue[Simetric[i]];
				break;
			case BK:
				if((Stack[gameDepth].whitePower +  Stack[gameDepth].blackPower) > 2000)
					Stack[gameDepth].positionVal -= WKingValue[Simetric[i]];
				else
					Stack[gameDepth].positionVal -= WKingValueEnd[i];
				break;
		}

	} // for i= pieses on board


	return 0;
}

int inline RebuildPiecesPositions2()
{
int t;
int i;
Stack[gameDepth].nWP = 0;

Stack[gameDepth].nBP = 0;

for(i=0;i<13;i++)
{
	Stack[gameDepth].pawnWOnFiles[i] = 0;
	Stack[gameDepth].pawnBOnFiles[i] = 0;
}

Stack[gameDepth].positionVal = 0;
for(i=21;i<99;i++)
	{
		t = tabla[i];
		if ( t <= 0) continue;

		Stack[gameDepth].nWP += xnWP[t]; // t is piece code
		Stack[gameDepth].nBP += xnBP[t];
		if(t == WP) Stack[gameDepth].pawnWOnFiles[(i - (i % 10)) / 10 - 1]++;
		if(t == BP) Stack[gameDepth].pawnBOnFiles[(i - (i % 10)) / 10 - 1]++;
		switch (t) {
			case WP:
				Stack[gameDepth].positionVal += WPawnPos[i];
				break;
			case WR:
				Stack[gameDepth].positionVal += WRookValue[i];
				break;
			case WN:
				Stack[gameDepth].positionVal += WKnightPost[i];
				break;
			case WB:
				Stack[gameDepth].positionVal += WBishopValue[i];
				break;
			case WQ:
				Stack[gameDepth].positionVal += WQueenValue[i];
				break;
			case WK:
				if((Stack[gameDepth].whitePower +  Stack[gameDepth].blackPower) > 2000)
					Stack[gameDepth].positionVal += WKingValue[i];
				else
					Stack[gameDepth].positionVal += WKingValueEnd[i];
				break;
			case BP:
				Stack[gameDepth].positionVal -= WPawnPos[Simetric[i]];
				break;
			case BR:
				Stack[gameDepth].positionVal -= WRookValue[Simetric[i]];
				break;
			case BN:
				Stack[gameDepth].positionVal -= WKnightPost[Simetric[i]];
				break;
			case BB:
				Stack[gameDepth].positionVal -= WBishopValue[Simetric[i]];
				break;
			case BQ:
				Stack[gameDepth].positionVal -= WQueenValue[Simetric[i]];
				break;
			case BK:
				if((Stack[gameDepth].whitePower +  Stack[gameDepth].blackPower) > 2000)
					Stack[gameDepth].positionVal -= WKingValue[Simetric[i]];
				else
					Stack[gameDepth].positionVal -= WKingValueEnd[i];
				break;
		}

	} // for i= pieses on board


	return 0;
}

