// ==========================
//
//  Transposition Table 
//
// ==========================
//
// Enpassant and castling flags are added in later, since this made an update easier.
// There's nothing wrong with this, except the added chance of confusion.
//
// DATA
// HashEntry defined in search.h

#define g_nHashSize  2 * 1024 * 1024 - 1

struct HashPair
	{
	HashEntry Near;
	HashEntry Far;
//	HashEntry Final;
	};


unsigned int HashFunction [128*16];
unsigned int HashFunction2 [128*16];
struct HashPair TranspositionTable[ g_nHashSize + 100];
//HashPair *TranspositionTable = NULL;

// --------------------------------------------------
// Allocate memory, sizeof (hashentry) * 2 ^ nBits
// --------------------------------------------------
/*
void SetHashSizeBits ( int nBits )
{
 nBits--; // (Since it's a 2 level table, divide by 2)
 g_nHashSize = (2 << (nBits-1) ) - 1;
 if (TranspositionTable != NULL ) free ( TranspositionTable );
 TranspositionTable = (HashPair*) malloc ( (g_nHashSize + 5) * sizeof (HashPair));
 if (TranspositionTable == NULL )
	{
	 if (nBits > 15) SetHashSizeBits ( 15 );
	 return;
	}
 Clear_Hash ( );
}
*/
//
// Get as close to nMegs hash as possible without going over
//
/*
void SetHashMB ( unsigned int nMegs )
{
	unsigned int nBits = 0;
	if (nMegs > 2)
		{
		for (int i = 17; i < 25; i++)
			if  ( ( (2 <<(i-1)) * sizeof (HashEntry) )  <= nMegs*1048577) nBits = i;
		if (nBits >= 17 && nBits < 25) SetHashSizeBits ( nBits );
		}
}
*/
// --------------------------------------------------
// Clear Hash Table by zeroing it out.
// --------------------------------------------------
void Clear_Hash ( )
{
ZeroMemory (TranspositionTable, sizeof (HashPair) * g_nHashSize);
Clear_PawnHash();
}

// --------------------------------------------------
//  Compute the Random Numbers of each piece on each square for the Hash Keys & Checksums
// --------------------------------------------------
void Create_HashFunction ( )
    {
	srand (33731);
	int i, x;

	for (i = 0; i <128; i++)
		{
		// xor together some 16-bit rands
		BPHashSq[i] = rand() ^ (rand()<<15) ^ (rand()<<25);
		WPHashSq[i] = rand() ^ (rand()<<15) ^ (rand()<<25);

        for (x = 0; x < 16; x++)
            {
			HashFunction  [i * 16 + x] = rand() ^ (rand()<<15) ^ (rand()<<25);
			HashFunction2 [i * 16 + x] = rand() ^ (rand()<<15) ^ (rand()<<25);
            }
		}

	srand (clock()); 
    }       

// --------------------------------------------------
//  Compute the Hash Value for an entire Board
// --------------------------------------------------
unsigned int Hash_Board ()
        {
		unsigned int sum = 0;
        int i;

		for (i = 20; i <= 100; i++)
			sum ^= HashFunction [ i * 16 + tabla [i] ];

        if (who == BLACK) sum ^= HashFunction [8 ];

        return sum;
        }

unsigned int  Hash_Board2 ()
        {
		unsigned int sum = 0;
        int i;

		for (i = 20; i <= 100; i++)
			sum ^= HashFunction2 [ i * 16 + tabla [i] ];

        if (who == BLACK) sum ^= HashFunction2 [8 ];

        return sum;
        }

// -------------------------
//  Incrementally update the Hash Value for the board from the last move
// -------------------------
long inline HashUpdate ()
    {
	 int Sq, oldpiece, newpiece;
        
		 // destination square
		 Sq = Stack2[gameDepth-1].SqTo;
		 newpiece = Stack2[gameDepth-1].PieceMoved; //tabla [ Sq ];
		 Sq <<= 4;
		 oldpiece = Stack2[gameDepth-1].PieceTaken; 
         Stack[gameDepth].HashKey  ^= HashFunction  [ Sq + newpiece ] ^ HashFunction  [ Sq + oldpiece ];
         Stack[gameDepth].HashCheck ^= HashFunction2 [ Sq + newpiece ] ^ HashFunction2 [ Sq + oldpiece ];
 
		 // source square
         Sq = Stack2[gameDepth-1].SqFrom;
		 newpiece = 0;
		 Sq <<= 4;
		 oldpiece = Stack2[gameDepth-1].PieceMoved; 
         Stack[gameDepth].HashKey  ^= HashFunction  [ Sq + newpiece ] ^ HashFunction  [ Sq + oldpiece ];
         Stack[gameDepth].HashCheck ^= HashFunction2 [ Sq + newpiece ] ^ HashFunction2 [ Sq + oldpiece ];

		 // extra castling squares?

		 if ( Stack2[gameDepth-1].Indicators == 1 || Stack2[gameDepth-1].Indicators == 2 )
                { Sq = (Stack2[gameDepth-1].SqTo + 1); newpiece = WR; oldpiece  = 0;
				  Sq <<= 4;
                  Stack[gameDepth].HashKey  ^= HashFunction  [ Sq + newpiece ] ^ HashFunction  [ Sq + oldpiece ];
                  Stack[gameDepth].HashCheck ^= HashFunction2 [ Sq + newpiece ] ^ HashFunction2 [ Sq + oldpiece ];

                }
        
		// Switch Side To Move
		Stack[gameDepth].HashKey ^= HashFunction   [8];
		Stack[gameDepth].HashCheck ^= HashFunction2 [8];

		return Stack[gameDepth].HashKey;
    }

//
// For a Null Move
//
// Updates the Board to-play color, and the hashvalues
//
void inline BoardSwitchColor ( )
{
	who = (who + 1) & 1;
    Stack[gameDepth].HashKey   ^= HashFunction  [8];
    Stack[gameDepth].HashCheck ^= HashFunction2 [8];
	Stack2[gameDepth].enPassant ^= 128;
}

// --------------------------------------------------
//  Look up an entry and retrieve its info for the search
// --------------------------------------------------
//
// Return a Value if possible
//
void inline TT_CutOff ( HashEntry &TEntry, int &value, int &nextbest, const int &alpha, const int &beta, const int &ahead, const int &depth, const int &bPV)
{
	if ( ahead > 1 && (TEntry.depth >= depth || TEntry.mate!=0) ) // Don't grab values at root
		{
		// CheckMate value returned? then adjust the eval since the score depends on ply
		if (TEntry.mate !=0)
			{
			if (TEntry.eval < 0 ) 
				{TEntry.eval = -MAT + TEntry.mate + ahead; 
				 if (TEntry.depth < depth) 
					{if (TEntry.eval <= alpha && TEntry.failtype !=2) value = TEntry.eval;
					 return;
					}
				}
			if (TEntry.eval > 0 ) 
				{TEntry.eval =  MAT - TEntry.mate - ahead; 
				 if (TEntry.depth < depth) 
					{if (TEntry.eval >= beta && TEntry.failtype !=1) value = TEntry.eval;
					 return;
					}
				}
			}
		if (TEntry.failtype == 0) value = TEntry.eval;
        else if (TEntry.failtype == 2 && TEntry.eval >= beta) value = TEntry.eval;
        else if (TEntry.failtype == 1 && TEntry.eval <= alpha) value = TEntry.eval;
		}
}

// Return a Best Move if it looks good. 
void inline TT_BestMove ( HashEntry &TEntry, int &value, int &nextbest, const int &alpha, const int &beta, const int &ahead, const int &depth, const int &bPV)
{
	// Is the best move from a search to a decent depth
	if ( (TEntry.depth >= (depth-2) || TEntry.mate!=0) ) 
		{
		// Don't take best move from a "fail low" since no best move exists
		// Unless the boundaries are similar, then take the best move so we don't do internal iterative deeping
		if (TEntry.failtype == 1 && who == WHITE && alpha < (TEntry.eval-80)) return;
		if (TEntry.failtype == 2 && who == BLACK && beta > (TEntry.eval+80)) return;		
		// Don't take best move from a "fail high" if it was for a window with a significantly lesser bound (so it uses internal iterative deepening instead)
		// This takes care of some degenerate cases where there's a mate in 1 (or more) but a non-mating best move is searched deeply first instead.
		if (TEntry.failtype == 0 || (alpha != (beta-1) && bPV == FALSE)
			|| ( TEntry.failtype == 2 && (who == BLACK || (TEntry.eval+150) >= beta))  
			|| ( TEntry.failtype == 1 && (who == WHITE || (TEntry.eval-150) <= alpha)) )	nextbest = TEntry.bestmove;
		}
}

int bHashing = 1;
char ccc;
void inline TT_Lookup_Final ( const unsigned int &nHashCheck, HashPair &TEntry, int &value, int &nextbest, const int &alpha, const int &beta, const int &ahead, const int &depth, const int &bPV)
{
	// hashing off
	if (bHashing == FALSE) return; 
	ccc = nHashCheck;

	if (TEntry.Far.checksum  == ccc) 
		{
		TT_CutOff ( TEntry.Far, value, nextbest, alpha, beta, ahead, depth, bPV);
		TT_BestMove ( TEntry.Far, value, nextbest, alpha, beta, ahead, depth, bPV);
		}
}


void inline TT_Lookup ( const unsigned int &nHashCheck, HashPair &TEntry, int &value, int &nextbest, const int &alpha, const int &beta, const int &ahead, const int &depth, const int &bPV)
{
	// hashing off
	if (bHashing == FALSE) return; 
	ccc = nHashCheck;
	if (TEntry.Near.checksum == ccc) 
		{
		TT_CutOff ( TEntry.Near, value, nextbest, alpha, beta, ahead, depth, bPV);
		if (value!= NOVALUE ) 
			{TEntry.Near.age = g_ucAge;  // set age to current since this entry was useful
			 return;}
		if (TEntry.Far.checksum  == ccc) 
			TT_CutOff ( TEntry.Far, value, nextbest, alpha, beta, ahead, depth, bPV);
		if (value!= NOVALUE ) return; // Check if there's a cutoff in the far table for this position
		TT_BestMove ( TEntry.Near, value, nextbest, alpha, beta, ahead, depth, bPV);
		return;
		}
	if (TEntry.Far.checksum  == ccc) 
		{
		TT_CutOff ( TEntry.Far, value, nextbest, alpha, beta, ahead, depth, bPV);
		TT_BestMove ( TEntry.Far, value, nextbest, alpha, beta, ahead, depth, bPV);
		}
}
// --------------------------------------------------
//  Store info from the search in a Hash Entry
// --------------------------------------------------
void inline TT_Write ( HashEntry &TEntry, const unsigned int &nHashCheck, const int &value, const int &nextbest, const int &alpha, const int &beta, const int &ahead, const int &depth)
{
	TEntry.checksum = nHashCheck;
	TEntry.eval = value;
	TEntry.bestmove = nextbest;
	TEntry.age = g_ucAge;
    TEntry.depth = depth;

	if (value <= alpha) TEntry.failtype = 1;
	else if (value >= beta)  TEntry.failtype = 2;
	else TEntry.failtype = 0;

	if ( abs(value) > 9900)
		{
		TEntry.mate = MAT - abs(value) - ahead;
		}
	else TEntry.mate = 0;
}


