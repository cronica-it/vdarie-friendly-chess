// This compute chassboard position code and access database for end game position
//
// Principles:
//    - rocades are supposed not allowed both sides
//    - pieces codes has rules:
//        - white king, one or two white pieces, black king, one or two pieces
//        - this time total pieces may be 4 or 3
//        - horizontal simetrization is made that white king must be in left side on chessboard, that
//            means columns A,B,C and D
//        - pieces of each side are ordered, that means King, Queen, Rook, Bishop, Knihgt, Pawn
//    - end game repository says only how many moves white mate blacks in given position
//            white is to move
//    - all black mates result by transposition to analog white position to acces
//         repository, dooing simetrization of pieces types and 
//         vertical simetrization (pawn directions)
//         
//    - when white is to move, he compute his mates by applying normal algorithm of 
//        minimax, accessing repository when white is to move
//    - the end game repository may be useful for playing games with some few pieces on
//        chess board, using minimax algorithm, promoting pieces exchanges to arrive 
//        to situations with 4 pieces, where repository data exists
//    - GOOD LUKE



struct EndGameCode {
	int whiteKing;
	int blackKing;
	int piece1;
	int position1;
	int piece2;
	int position2;
	int code;
};

int ta64[120] = 
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 1, 2, 3, 4, 5, 6, 7, 0,
		  0, 8, 9,10,11,12,13,14,15, 0,
		  0,16,17,18,19,20,21,22,23, 0,
		  0,24,25,26,27,28,29,30,31, 0,
		  0,32,33,34,35,36,37,38,39, 0,
		  0,40,41,42,43,44,45,46,47, 0,
		  0,48,49,50,51,52,53,54,55, 0,
		  0,56,57,58,59,60,61,62,63, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

struct EndGameCode EndCode;

char masiv[600*1000000];

int q;
 
int pas;

int indRead = 0; // firts access read Q,R and P ends

int GenEndStatistics[50][80];

#define SZ 64

int comb = 0;

FILE *f;
int ReadRepositoryEnd(int steps)
{
 int nread; 
 
	 for(i=0; i<600*1000000;i++)
		masiv[i] = 0;

	f = fopen("Repository.end","rb");
	
		fseek(f, (long)0,SEEK_SET);

		nread = fread((char *) &masiv[0],(long)steps*64*64, 64*SZ,f);

	fclose(f);
	return 1;
}

//
// EXTENDED Consult End Game Repository
//
// When less than 10 pieces, try some different pattern of Engames from Repository
// if find one acceptable, return a value from this pattern + 5
//
// if SURE pattern, return a SURE value (if not null)
//
int ExtendedConsultEndGame()
{

struct Pieces {
		int piece;
		int position;
		int flag;
		int state;
	};

struct Pieces wPieces[50], bPieces[50];
int nwPieces, nbPieces,;


int qmin, aa;
int xx;

    aa = 10000;
	qmin = 10000; // for minim
	
    // case very simple 3 or 4 pieces -> SURE tresold consulting
	if(Stack[gameDepth].nPieces <= 4) 
		return ConsultEndGame(); // aici chiar este 4 piese
	
  return 0; // provizoriu
	
	if(who == 0)
	{
		// copy lwhite and lblack as they are
		for(i=0,nwPieces=0;i<nwhite;i=i+3,nwPieces++)
		{
				wPieces[nwPieces].piece = lwhite[i];
				wPieces[nwPieces].position = lwhite[i+1];
				wPieces[nwPieces].flag = 0;
				wPieces[nwPieces].state = 0;
		}
		for(i=0,nbPieces=0;i<nblack;i=i+3,nbPieces++)
		{
				bPieces[nbPieces].piece = lblack[i];
				bPieces[nbPieces].position = lblack[i+1];
				bPieces[nbPieces].flag = 0;
				bPieces[nbPieces].state = 0;
		}
	}
	else
	{
	   // copy lblack and lwhite by transposition whites-blacks
		// copy lwhite and lblack as they are
		for(i=0,nwPieces=0;i<nblack;i=i+3,nwPieces++)
		{
				wPieces[nwPieces].piece = lblack[i] - 6;
				wPieces[nwPieces].position = 119 - lblack[i+1];
				wPieces[nwPieces].flag = 0;
				wPieces[nwPieces].state = 0;
		}
		for(i=0,nbPieces=0;i<nwhite;i=i+3,nbPieces++)
		{
				bPieces[nbPieces].piece = lwhite[i + 6];
				bPieces[nbPieces].position = 119 - lwhite[i+1];
				bPieces[nbPieces].flag = 0;
				bPieces[nbPieces].state = 0;
		}

	}
	// mark blocked or denied pawns

	// trebuie o tabla aici !!!!!!!!!!!!

	for(i=0;i<nwPieces;i++)
	{
		for(xx=wPieces[i].position;xx<100;xx=xx+10)
		{
			if((wPieces[i].piece == WP) && 
				(tabla[xx + 10] == WP) || (tabla[xx + 10] == BP) || (tabla[xx+ 10] > WP) ||
				(tabla[xx + 10] == BR) || (tabla[xx + 10] == BN) || (tabla[xx + 10] == BB) ||
				(tabla[xx + 9] == BP) || (tabla[xx + 11] == BP))
					wPieces[i].state = 1; // blocked
		}
	}
    
	for(i=0;i<nbPieces;i++)
	{
		for(xx=bPieces[i].position;xx>30;xx=xx-10)
		{
			if((bPieces[i].piece == BP) &&
				(tabla[xx - 10] == BP) || (tabla[xx - 10] == WP) || (tabla[xx - 10] > BP) ||
				(tabla[xx - 10] == WR) || (tabla[xx - 10] == WN) || (tabla[xx - 10] == WB) ||
				(tabla[xx - 9] == WP) || (tabla[xx - 11] == WP))
					bPieces[i].state = 1; // blocked
		}
	}
    
	if(nblack <= 6) // black king single
	{
		// analyze pieces after king and 2 pieces more important
		    if(lwhite[3] == WQ)
			{
			// try with quin and the next pieces
				qmin = 10000;
				for(xx=6;xx<=nwhite;xx = xx + 3)
				{
					piece1 = lwhite[3];
					piece2 = lwhite[xx];
					aa = AccessMat4(0,lwhite[3+1],lwhite[0+1],lblack[0+1],lwhite[xx + 1]);

					if(aa < qmin)
							qmin = aa;
				}
				
				if(qmin > 0 && qmin < 100) return qmin + 5;
			}
		if(qmin == 10000) return 0;
	}
 return 0;
}


//
// Consult End Game Repository
//
// When less than 4 pieces, there is consulted End Game repository to find how many
// plays are to be made till the end of the game
//
int ConsultEndGame()
{
 
	if(Stack[gameDepth].nPieces <= 3)
	{

      if(who == 0) // white position to play
	  {
		if(Stack[gameDepth].nPieces == 3) // 3 pieces
		{

			if(lwhite[3] == 0) return 0; // no mate info

			piece1 = lwhite[3];
			piece2 = 0;
			return AccessMat4(0,lwhite[3+1],lwhite[0+1],lblack[0+1],0);
		}
		else if(Stack[gameDepth].nPieces == 4) 
		{

			if((lblack[3] > 0) && (lblack[6] > 0)) return 0; // no mate info (only black pieces)

			if(lblack[3] == 0) // 2 white pieces, 0 black
			{
				piece1 = lwhite[3];
				piece2 = lwhite[6];
				return AccessMat4(0,lwhite[3+1],lwhite[0+1],lblack[0+1],lwhite[6 + 1]);
			}
			else // 1 white piece 1 black piece
			{
				piece1 = lwhite[3];
				piece2 = lblack[3];
				return AccessMat4(0,lwhite[3+1],lwhite[0+1],lblack[0+1],lblack[3+1]);
			}
		}
	  }
	  else // black position to play
	  {
		if(Stack[gameDepth].nPieces == 3) // 3 pieces
		{

			if(lblack[3] == 0) return 0; // no mate info

			piece1 = lblack[3] - 6;
			piece2 = 0;
			return AccessMat4(0,Simetric[lblack[3+1]],Simetric[lblack[0+1]],Simetric[lwhite[0+1]],0);
		}
		else if(Stack[gameDepth].nPieces == 4) 
		{

			if((lwhite[3] > 0) && (lwhite[6] > 0)) return 0; // no mate info (only white pieces)

			if(lwhite[3] == 0) // 2 black pieces, 0 white
			{
				piece1 = lblack[3] - 6;
				piece2 = lblack[6] - 6;
				return AccessMat4(0,Simetric[lblack[3+1]],Simetric[lblack[0+1]],Simetric[lwhite[0+1]],Simetric[lwhite[3+1]]);
			}
			else // 1 black piece 1 white piece
			{

				piece1 = lblack[3] - 6;
				piece2 = lwhite[3] + 6;
				return AccessMat4(0,Simetric[lblack[3+1]],Simetric[lblack[0+1]],Simetric[lwhite[0+1]],Simetric[lwhite[3+1]]);
			}
		} // 4 pieces
	  } // black
	} // if <= 4
	return 0;
}
 


//
// Accessing read/write in End Game Repository
//

int AccessMat4(int mode, int zpoz1,int zpoz2,int zpoz3,int zpoz4)
{
// zpos1 - position of piece 1
// zpos4 - position of piece 2 if exists
// zpos2 - position of white king
// zpoz2 - position of bhite king
// piece1   - type of piece 1
// piece2   - type of piece 2 if not null

int offset;
int info;

int val;


    // form position code to access info
    
	EndCode.whiteKing = zpoz2;   
	EndCode.blackKing = zpoz3;   
	EndCode.piece1    = 0;
	EndCode.position1 = 0;
	EndCode.piece2    = 0;
	EndCode.position2 = 0;

	EndCode.piece1    = piece1;
	EndCode.position1 = zpoz1;

    if (zpoz4 > 0)
	{
		EndCode.piece2    = piece2;
		EndCode.position2 = zpoz4;
	}
	else
	{
		EndCode.piece2    = 0;
		EndCode.position2 = 0;
	}
	// because of pawn repository generation, may result that piece1 is lower than piece2 as 
	// value -> then intercenge place between piece1 and piece2
	if((EndCode.piece2 < BP) && (EndCode.piece2 > 0) && 
		(ValPiesa[EndCode.piece1] < ValPiesa[EndCode.piece2]))
	{
		// switch piece1 with piece2
		int xx = EndCode.piece1;
		EndCode.piece1 = EndCode.piece2;
		EndCode.piece2 = xx;

		xx = EndCode.position1;
		EndCode.position1 = EndCode.position2;
		EndCode.position2 = xx;
        
	}

		// Compute offset address to find if there is a mate
	
		// 1. compute relative adress for a given piece structure
		EndCode.whiteKing = ta64[EndCode.whiteKing];
		EndCode.blackKing = ta64[EndCode.blackKing];
		EndCode.position1 = ta64[EndCode.position1];
		if(zpoz4 > 0)
			EndCode.position2 = ta64[EndCode.position2];		
 
		EndCode.code =  EndCode.whiteKing * 64 * 64  +
						EndCode.blackKing * 64  +
						EndCode.position1  ; // Atention 64 not SZ here
		if(zpoz4 > 0)
						EndCode.code = EndCode.code * 64 + EndCode.position2;

	    // 2. compute code for piece structure	4 not 99	
		for(i=0;planEnds[i].indexMasiv<99;i++)
		{
			if( (planEnds[i].pieseComposition[0] == WK) &&
				(planEnds[i].pieseComposition[1] == BK) && 
				(planEnds[i].pieseComposition[2] == EndCode.piece1) && 
				(planEnds[i].pieseComposition[3] == EndCode.piece2)) break;

		}
		if(planEnds[i].indexMasiv == 99) // pieces combination not found
		{
			q = 0;
			return -1;
		}
		// 3. Disc address =
		offset  = i * SZ * 64 * 64 * 64;

		info = 0;

		if(mode == 0) // read info
		{
					q = (int)masiv[offset + EndCode.code];
		}
		else // write info
		{
			if(masiv[offset + EndCode.code] == 0)
			{
					masiv[offset + EndCode.code] = q;
			}
		}
    // found mate info
		val = q;

		return val;
}

//
// Generate Endgame Repository
//
int GenerateEndGameRepository()
{
int i;
int MiMaQuik[2];
int k;


int wking,bking;
int pos1, pos2;


int xpos11,xpos12;
int xpos21,xpos22;

int activity;

int xcomb1;

int xp1[100] ,xp2[100] ,xp3[100],xp4[100];

int comb;

 
 return 0; // no more generation 
  
	for(i=0; i<600*1000000;i++)
		masiv[i] = 0;

	for(k=0;k<120;k++)
		{
		if(tabla0[k] <= 0)
			tabla[k] = tabla0[k];
		else
			tabla[k] = 0;
		}

	for( xcomb1=0;planEnds[xcomb1].indexMasiv<99;xcomb1++)
	{
		for(int pas1=0;pas1<80;pas1++)
		{
		  GenEndStatistics[xcomb1][pas1] = 0; // clear statistics
		}
	}

	// GENERATE REPOSITORY for ENDGAMES

	// It used a list "planEnds" to generate mates first with 3 pieces, and then with 4 pieces
	//

	
	for(comb=0;planEnds[comb].indexMasiv < 99;comb++)
	{
  
 if(planEnds[comb].indexMasiv == 7) break;

		// clear memory for a combination generation

		//
		// Load EndCode structure from planEnds structure
		piece1 = planEnds[comb].pieseComposition[2];  // 0 WK si 1 BK
		piece2 = planEnds[comb].pieseComposition[3];

		// each combination of pieces, starting with A1 square
		xpos11=21; // for de la xpos11 la xpos22
		xpos12=98;
		if((piece1 == WP) || (piece1 == BP)) 
		{
			xpos11=31;
			xpos12=88;
		}

		xpos21=21;
	 	xpos22=98;
		if(piece2 > 0)
		{

			if((piece2 == WP) || (piece2 == BP)) 
			{
				xpos21=31;  // for piesa2 de la xpos21 la xpos22
				xpos22=88;
			}
		}
		else
		{
			xpos21=0;
			xpos22=0;
		}

		xp1[comb] = 0;
		xp2[comb] = 0;
		xp3[comb] = 0;
		xp4[comb] = 0; 

		for(pas = 0;pas<80;pas++) // each iteration
		{
			activity = 0;
			for(wking=21;wking<=98;wking++)
			{
				if(tabla0[wking] < 0) continue;
				
				for(bking=21;bking<=98;bking++)
				{
					if(tabla0[bking] < 0) continue;
					//if(abs(wking / 10 - bking / 10) <= 1 && abs(wking % 10 - bking % 10) <= 1) continue;
					if( DistKing[wking][bking] == 1) continue; // no direct claches king1 and king2					
					if(wking == bking) continue;
					
					for(pos1=xpos11;pos1<=xpos12;pos1++)
					{
						if(tabla0[pos1]  < 0) continue;
						
						for(pos2=xpos21;pos2<=xpos22;pos2++)
						{

							if((pos2 > 0 ) && (tabla0[pos2] < 0)) continue;

							// pieces with the same color do not overlap
							if(pos1 == wking || pos1 == bking) continue;
							if(pos2 == wking || pos2 == bking) continue;
							
							if(pos1 == pos2 ) continue;

							tabla[wking] = WK;
							tabla[bking] = BK;
							tabla[pos1] = piece1;
							if(pos2 > 0) 
								tabla[pos2] = piece2;
							q = 0;
							if(piece2 == 0 )
								AccessMat4(0,pos1,wking,bking,0); // consultare (0,piesa1,wking,bking,0,&q
							else if(piece2 > 0 )
								AccessMat4(0,pos1,wking,bking,pos2); // consultare (0,piesa1,wking,bking,piesa2,&q
								
							if (  q == 0) 
							{

								MiMaQuik[0] = MAT;
								MiMaQuik[1] = GenQuik(pos1,wking,bking,pos2);
								if(pas > 0) 
									MiMaQuik[1] = MiMaQuik[1] - 2;

								if ( ( (int)MAT - MiMaQuik[1] ) == (2 * pas + 1))
								{
									q = (int)MAT - MiMaQuik[1];
									AccessMat4(1,pos1,wking,bking,pos2); // store q found
									
									xp1[comb] = pos1;
									xp2[comb] = wking;
									xp3[comb] = bking;
									xp4[comb] = pos2; 
									
									// increment statistics
									GenEndStatistics[comb][pas]++;
									activity++;
								}
							} // if
							tabla[pos1]  = 0; // make restore tabla for next combination
							tabla[wking] = 0;
							tabla[bking] = 0;
							if(pos2 > 0)
								tabla[pos2]  = 0;
							
						} // for piece2
					} // for piece1
				} // for bking
			} // for wking
		      if(activity == 0) break;
		} // for each "step"
		
 
	// write statistics into a file

	f = fopen("Statistics.end","wt+");

	for(int i=0;planEnds[i].indexMasiv<99;i++)
	{
		for(int pas1=0;pas1<80;pas1++)
		{
		  fprintf(f," %s  comb = %2d  pas = %2d  pozitii = %10d \n", 
			  planEnds[i].comentEnd, xcomb1, pas1, GenEndStatistics[i][pas1]);
			  
			
		  if(GenEndStatistics[i][pas1] == 0) break;

		}
		  fprintf(f," Combinatia cea mai lunga piesa 1 = %2d  rege W = %2d  rege B = %2d  pieca 2 = %2d \n", 
			  xp1[i],xp2[i], xp3[i], xp4[i]);
		
		fprintf(f,"  \n\n\n");
	}
	fclose(f);

	} // for each piece combination "comb"
	f = fopen("Repository.end","wb+");

	fseek(f, (long)0,SEEK_SET);
	fwrite((char * )&masiv[0],comb * 4096, 64 * SZ,f);

	fclose(f);
	return 1;
} // end Generate Repository
