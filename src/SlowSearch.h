/*
// ==========================
// GAME TREE SEARCH
// searches to depth
// returns evaluation of the best move found, (the move itself is returned in bestmove)
// BLACK = MIN, WHITE = MAX
// uses SearchBoard as input Board
//
// ==========================
short ABSearch(int color, int ahead, int depth,  short alpha, short beta,  int &bestmove, int bAfterNull)
{
if (stopthinking > 0 || g_ponder == 3 ) return TIMEOUT;
if (g_nMaxNodes && nodes > g_nMaxNodes && SearchDepth > 1 && !g_bAnalyze)  return TIMEOUT;

// Check if search time is used up every 50,000 nodes
if (nodes > nodes2)
	if (CheckTimeUp () ) return TIMEOUT; 

int u1, u2, nHashKeyAndFlags, MMove;
int Max, i, Ext, nMat;
int nextbest, nextDepth;
int nSearched, bPV = FALSE;
short value;

ahead++;
// ------- Try Null Move ---------
if (bAfterNull != TRUE && depth > 2 &&  MoveStack[ahead].ischeck == 0  && g_bDoNull == TRUE)
{ nMat = (color == BLACK) ? SearchBoard.BMat - SearchBoard.nBPawns * 100 : SearchBoard.WMat - SearchBoard.nWPawns * 100;
  if ( nMat > 499 ) 
	{
	// If the static eval isn't crummy, try a null move
	value = EvaluateBoard ( SearchBoard, alpha-210, beta+210 );
	if ( ( (value+200) >= beta && color == WHITE) 
	 || ( (value-200) <= alpha && color == BLACK))
		{
		if (depth > 6 && nMat > 900) nextDepth = depth - 4;
							else  	 nextDepth = depth - 3;
		if (nextDepth < 2) nextDepth = 0;
		MoveStack[ahead+1].ischeck = 0;
		nextbest = NO_BEST;

		int nMoves = 100;
		if (depth>3 && nMat < 1000)
			{if (color == BLACK) 
				{FindMovesBlack(SearchBoard, MoveStack[ahead]);
				 nMoves = (MoveStack[ahead].numAttackmoves + MoveStack[ahead].nummoves);
				}
			else
				{FindMovesWhite(SearchBoard, MoveStack[ahead]);
				nMoves = (MoveStack[ahead].numAttackmoves + MoveStack[ahead].nummoves);
				}
			}
		if (nMoves >= 10)
			{
			BoardSwitchColor ( );
			nHashKeyAndFlags = SearchBoard.HashKey ^ HashFunction[SearchBoard.EnPassant] ^ HashFunction[SearchBoard.WCastle + (SearchBoard.BCastle<<3)];
			value = NOVALUE;
			short nBounds = (color == WHITE) ? beta-1 : alpha;
			TT_Lookup ( SearchBoard.HashCheck, TranspositionTable [ nHashKeyAndFlags & g_nHashSize ], value, nextbest, nBounds, nBounds+1, ahead, nextDepth, bPV);
			if (value == NOVALUE)
				{// ABSearch will search at least 1 ply even if depth is 0 or less
				value =  ABSearch (color^3, ahead, nextDepth, nBounds, nBounds+1, nextbest, TRUE );						
				}
			TT_Store ( SearchBoard.HashCheck, TranspositionTable [ nHashKeyAndFlags & g_nHashSize ], value, nextbest, nBounds, nBounds+1, ahead, nextDepth);
			BoardSwitchColor ( );

			if (value == TIMEOUT) return TIMEOUT;
			if (value >= beta && color == WHITE ) return beta;
			if (value <= alpha && color == BLACK) return alpha; 
			}
		}
	}
}
// --------------------------
// Internal iterative deepening to find a guess for the best move
if (bestmove == NO_BEST && depth >= 3) 
	{
	value = ABSearch( color , ahead-1, depth-2, alpha, beta,  bestmove, FALSE );
	if ( value == TIMEOUT) return TIMEOUT;
	}

// Set Opposite Color var  
Max = (color == WHITE) ? -19999 : 19999;

// Find possible moves if not at root
if (ahead > 1)
	{
	value = GenerateAndSortMoves ( color, ahead, alpha, beta, bestmove  );
	if (value !=0) return value;
	}

// Draw by 50 move rule (here because we have to check for mates first)
if ((SearchBoard.GameTime - SearchBoard.LastReversible) >= 100 ) 
	{PV[ahead-1][1] = 252; PV[ahead-1][0] = 1;return DRAWSCORE;}

// ---
if (color == BLACK && (ahead < 2 || BoardStack[ahead-1].MatVal > beta + 350) )  MoveStack[ahead].eChecks--;
if (color == WHITE && (ahead < 2 || BoardStack[ahead-1].MatVal < alpha - 350))  MoveStack[ahead].eChecks--;

// Save Undo Board & Hash Values
Copy_Board2 (BoardStack[ahead], SearchBoard);
AddRepBoard (SearchBoard.HashKey ^ HashFunction[SearchBoard.EnPassant] ^ HashFunction[SearchBoard.WCastle + (SearchBoard.BCastle<<3)], SearchBoard);

int numMoves = MoveStack[ahead].nummoves;

	// Cycle through the moves and evaluate them
    for (i = 0; i < numMoves; i++)
                 {
				 // Sort by History if Bestmove, captures, and killer all failed
				 if ( depth>1 && ahead<11 && i == MoveStack[ahead].numAttackmoves ) SortByHistory (MoveStack[ahead], color);

				 // At root, update info then check for fail-low
				 if ( ahead == 1 && i == 1 && depth > 5)
					{
					 UpdateDisplayInfo ( i, MMove, Max, bestmove);
					 IsPanic ( CurrentEval );
					}					

				 MMove = MoveStack[ahead].Moves[i];

				 // Is this a legal move? If not skip it
				 if (MoveStack[ahead].ischeck==0)
					{
					 if (MPiece(MMove) == 15) // Moved King?
						{if ( IsAttacked ( SearchBoard, MDst(MMove), color) ) continue;}
					 else if (DiscoveredCheck (color, SearchBoard, MMove) == 1 ) continue;						
					}

				 // The move is legal, so do it
				 MoveStack[ahead+1].ischeck = CheckingMove (color, SearchBoard, MMove  );
                 Ext = Do_Move(SearchBoard, MMove , u1, u2, MPromo (MMove) );
				 MoveStack[ahead].recap = Ext;
                 nodes++;

				 // Set globals for display info, send PV to winboard if it changes
                 if (ahead == 1 )  UpdateDisplayInfo ( i, MMove, Max, bestmove);
				                    
				 nSearched = 1;

				 // Check the board for Mate Threats if it's not one piece or less endgame
				 if (MoveStack[ahead+1].ischeck == 0 && (SearchBoard.nWP > 1 || SearchBoard.nBP > 1))
					{
					if (color == WHITE )
						{
						if (TouchMatesB (SearchBoard, depth)  ) {value = -MATE+ahead+2; nSearched = 4; nodes++;}
						if (Ext <= NO_EXT)
							{if (MoveStack[ahead-1].recap == MDst(MMove) && BoardStack[ahead].BPiece[14] == MDst(MMove) ) Ext = QUEENRECAP;
							 if (depth > 1 && TouchMatesW (SearchBoard, depth) ) Ext = MATETHREAT;
							}
						 if (depth == 1 || bAfterNull == TRUE || (MoveStack[ahead].ischeck != 0 && depth == 0))
							if (QueenThreat ( SearchBoard.BPiece, MPiece(MMove), MDst (MMove), 0) == 1 ) Ext = QUEENTHREAT;

						}
					else
						{
						if (TouchMatesW (SearchBoard, depth) ) {value = MATE-ahead-2; nSearched = 4; nodes++; }
						if (Ext <= NO_EXT)
							{if (MoveStack[ahead-1].recap == MDst(MMove) && BoardStack[ahead].WPiece[14] == MDst(MMove) ) Ext = QUEENRECAP;
							 if (depth > 1 && TouchMatesB (SearchBoard, depth)  ) Ext = MATETHREAT;
							}
						if (depth == 1 || bAfterNull == TRUE || (MoveStack[ahead].ischeck != 0 && depth == 0))
							if (QueenThreat ( SearchBoard.WPiece, MPiece(MMove), MDst (MMove), 8) == 1) Ext = QUEENTHREAT;
						}
					}

				 if (nSearched == 1)
				 {
			     // Update HashValues
                 HashUpdate (u1, u2, SearchBoard );
				 nHashKeyAndFlags = SearchBoard.HashKey ^ HashFunction[SearchBoard.EnPassant] ^ HashFunction[SearchBoard.WCastle + (SearchBoard.BCastle<<3)];

                 // Draw by repetition
				 int Reps = DrawByRepetition (nHashKeyAndFlags, SearchBoard);
                 if ( (Reps > 1 || (ahead > 2 && Reps > 0) ) 
					  || (MoveStack[ahead+1].ischeck == 0 && NoMatingMaterial ( SearchBoard)) ) 
					{
					 value = DRAWSCORE; nSearched = 2;
					}
                 // If there is no depth left in the search, and the opponent isn't in check, evaluate the board position
				 else if ( ( depth <= 1 && MoveStack[ahead+1].ischeck== 0 && Ext<=NO_EXT && MoveStack[ahead].nummoves > MoveStack[ahead].eChecks) || (ahead > g_MaxDepth ) )
                       {
					 	nSearched = 0;
						if ( color==WHITE && alpha > (MATE-100) ) value = alpha;
						else if ( color==BLACK && beta < (-MATE+100) ) value = beta;
						else 		
							{
							if (color == WHITE) value = -QuiesceBoard( color^3, -beta, -alpha, ahead+1, 0);
								else value = QuiesceBoard( color^3, alpha, beta, ahead+1, 0);
							}
						}

                  //Look ahead to all possible moves if search hasn't stopped yet
                  else { 
                        value = NOVALUE;
                        nextbest = NO_BEST;

						DoExtensions ( depth, ahead, Ext, nextDepth, MDst (MMove) );
							
						TT_Lookup ( SearchBoard.HashCheck, TranspositionTable [ nHashKeyAndFlags & g_nHashSize ], value, nextbest, alpha, beta, ahead, nextDepth, bPV);

                        if (value == NOVALUE) // No value found in TT
                                {
								if (bPV == TRUE) // Ugly looking PV search code.
									{
									if (color == WHITE) 
										{
										value = ABSearch( color^3 , ahead, nextDepth, alpha, alpha+1,  nextbest, FALSE ); 
										if (value > alpha && value!=TIMEOUT )  // Fail High
											{
											if (ahead == 1) { CurrentBest = MMove; g_nFail = 1; g_nNoChange = 0; 
											AddPV ( MAXDEPTH, MMove, 1, MoveStack[ahead+1].ischeck, GameBoard );
											if (SearchDepth > 3) wbSendThinking (SearchDepth, Max, (100*(clock() - starttime))/( CLOCKS_PER_SEC ), nodes, TRUE, TRUE, FALSE);}
											value = ABSearch( color^3 , ahead, nextDepth, alpha, beta,  nextbest, FALSE ); 
											if (value > alpha || value == TIMEOUT) {bestmove = MMove;} else if (ahead==1) {CurrentBest = bestmove;}
											}
										}
									else 
										{
										value = ABSearch( color^3 , ahead, nextDepth, beta-1, beta,  nextbest, FALSE ); 
										if (value < beta && value!=TIMEOUT ) 
											{
											if (ahead == 1) { CurrentBest = MMove; g_nFail = 1; g_nNoChange = 0; 
															  AddPV ( MAXDEPTH, MMove, 1, MoveStack[ahead+1].ischeck, GameBoard );
															  if (SearchDepth > 3) wbSendThinking (SearchDepth, Max, (100*(clock() - starttime))/( CLOCKS_PER_SEC ), nodes, TRUE, TRUE, FALSE);}
											value = ABSearch( color^3 , ahead, nextDepth, alpha, beta,  nextbest, FALSE );
											if (value < beta || value == TIMEOUT) {bestmove = MMove;} else if (ahead==1) {CurrentBest = bestmove;}
											}
										}
									}
								else
									{
									value = ABSearch( color^3 , ahead, nextDepth, alpha, beta,  nextbest, FALSE ); 
									}
                                if (value == TIMEOUT) return TIMEOUT; // Time limit reached, bail out
				
								TT_Store ( SearchBoard.HashCheck, TranspositionTable [ nHashKeyAndFlags & g_nHashSize ], value, nextbest, alpha, beta, ahead, nextDepth);
								nSearched = 0;
                               }

						// reduce a 2nd repetition to 7/8ths
						if (Reps == 1 && value != DRAWSCORE && abs(value)<9000 ) value = (value>>1) + (value>>2) + (value>>3); 
                       }
				 } // end if nSearched

			 // Restore Board & Hash Values
             Undo_Move (SearchBoard, BoardStack[ahead], u1, u2);

              // Keep Track of Best Move and Alpha-Beta Prune
             if (color == WHITE &&  value > Max) 
					{
                     Max = value;
                     bestmove = MMove;
					 
					 if (value >= beta || value == (MATE-ahead-1) ) 
						{
						 AddHistory ( bestmove, depth, ahead, WHITE);
						 if (ahead == 1) {CurrentEval = value;}
						 AddPV ( ahead, MMove, nSearched, MoveStack[ahead+1].ischeck, SearchBoard );
                         return value;
						}
                     if (value > alpha ) 
						{
						 AddPV ( ahead, MMove, nSearched, MoveStack[ahead+1].ischeck, SearchBoard );
						 bPV = TRUE;
						 alpha = value;
						}
					}	
              else if (color == BLACK &&  value < Max) 
                    {
                     Max = value;
                     bestmove = MMove;
                                                                          
					 if (value <= alpha || value == (-MATE+ahead+1 )  )  
						{
						 AddHistory ( bestmove, depth, ahead, BLACK); 
						 if (ahead == 1) {CurrentEval = value;}
						 AddPV ( ahead, MMove, nSearched, MoveStack[ahead+1].ischeck, SearchBoard );
						 return value;
						}
                     if (value < beta ) 
						{
	 					 AddPV ( ahead, MMove, nSearched, MoveStack[ahead+1].ischeck, SearchBoard  );
						 bPV = TRUE;
						 beta = value;
						 }
				    }
                 }

// ---------- Done going through all the moves (No alpha/beta cuts) ------------
if (ahead==1) {CurrentEval = Max;}
else // StaleMate? (No moves found)
    {
	if ( abs(Max) == 19999 ) return DRAWSCORE;
    }

AddHistory ( bestmove,  depth, ahead, color); 

return Max;
}
*/
