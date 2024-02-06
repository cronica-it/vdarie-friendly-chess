/*
 * Copyright (c) 2024 Viorel Darie. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

//
// Inspired from Slow Chess move generation functions
// by Jon Kreuzer
//
//
// MOVELIST FUNCTIONS
//
// ----------------
// Clear the Movelist
// ----------------
void inline Clear_Movelist ()
{
  pma0 = (int*)&Stack2[gameDepth].Attacks[0];
  //		pm0  = (int *)&Stack2[gameDepth].NoAttacks[0];

  pma = pma0;

  Stack2[gameDepth].nbAttacks = 0;
  Stack2[gameDepth].movesOnLevel = 0;
}

int inline Close_Movelist ()
{
  Stack2[gameDepth].nbAttacks = pma - pma0;
  Stack2[gameDepth].movesOnLevel = Stack2[gameDepth].nbAttacks;

  Stack2[gameDepth].rankOnLevel = 0;
  Stack2[gameDepth].rankAttacks = 0;
  return 0;
}
#if 0
// -----------------
// Add a Move to Movelist (used for castling moves)
// -----------------
void inline AddCastleMove (const int src, const int dst, const int type,
                           const int piece)
{
  int indi = 1;
  if piece
    == BK indi = 2;

  Stack[gameDepth].NoAttacks[Stack[gameDepth].nbNoAttacks++].move
      = piece * 1000000 + src * 100 + dst;
  Stack[gameDepth].NoAttacks[Stack[gameDepth].nbNoAttacks++].Indicators = indi;
}

// -----------------
//  Find Moves for a promoted pawn
// -----------------
void
FindMovesWhitePromotedPawn (Movestruct* WMoves, SBoard& Board,
                            unsigned char* Boardptr, int data)
{
  if (*Boardptr == WQUEEN)
    FindMovesWhiteQueen (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == WROOK)
    FindMovesWhiteRook (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == WBISHOP)
    FindMovesWhiteBishop (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == WKNIGHT)
    FindMovesWhiteKnight (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == WPAWN)
    FindMovesWhitePawn (*WMoves, Board.Sq, Boardptr - Board.Sq, data,
                        Board.EnPassant);
}

void
FindMovesBlackPromotedPawn (Movestruct* WMoves, SBoard& Board,
                            unsigned char* Boardptr, int data)
{
  if (*Boardptr == BQUEEN)
    FindMovesBlackQueen (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == BROOK)
    FindMovesBlackRook (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == BBISHOP)
    FindMovesBlackBishop (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == BKNIGHT)
    FindMovesBlackKnight (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == BPAWN)
    FindMovesBlackPawn (*WMoves, Board.Sq, Boardptr - Board.Sq, data,
                        Board.EnPassant);
}

void
FindAMovesWhitePromotedPawn (Movestruct* WMoves, SBoard& Board,
                             unsigned char* Boardptr, int data)
{
  if (*Boardptr == WQUEEN)
    FindAMovesWhiteQueen (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == WROOK)
    FindAMovesWhiteRook (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == WBISHOP)
    FindAMovesWhiteBishop (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == WKNIGHT)
    FindMovesWhiteKnight (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == WPAWN)
    FindMovesWhitePawn (*WMoves, Board.Sq, Boardptr - Board.Sq, data,
                        Board.EnPassant);
}

void
FindAMovesBlackPromotedPawn (Movestruct* WMoves, SBoard& Board,
                             unsigned char* Boardptr, int data)
{
  if (*Boardptr == BQUEEN)
    FindAMovesBlackQueen (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == BROOK)
    FindAMovesBlackRook (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == BBISHOP)
    FindAMovesBlackBishop (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == BKNIGHT)
    FindAMovesBlackKnight (*WMoves, Board.Sq, Boardptr - Board.Sq, data);
  else if (*Boardptr == BPAWN)
    FindAMovesBlackPawn (*WMoves, Board.Sq, Boardptr - Board.Sq, data,
                         Board.EnPassant);
}
#endif
// -----------------------------------
// Find all the moves on a Board for WHITE and store in Attacks or NoAttacks at
// level gameDepth
// -----------------------------------
void
FindMovesWhite (int indExchange)
{
  int i, j, np;
  Clear_Movelist ();

  //		indExchange = 0;

  for (i = 1; i <= 6; i++)
    {
      np = Stack[gameDepth].pieceCounts[i];
      if (np == 0)
        continue;

      for (j = 0; j < np; j++)
        {
          GenMovesForAWhitePiece (indExchange, i,
                                  Stack[gameDepth].pieceAPosition[i][j]);
        }
    }

  Close_Movelist ();
  return;
}
/*
// Check Test First
void FindMovesWhiteCh(SBoard &Board, Movestruct &Moves)
{
        Moves.ischeck = IsCheck (WHITE, Board);
        FindMovesWhite ( Board, Moves );
}
*/
// Rook and Queen Moves only
void
FindHeavyMovesWhite (int color)
{
  Clear_Movelist ();

  int i, j, np;

  for (i = 2; i <= 5; i++)
    {
      if (i == 3 || i == 4)
        continue;

      np = Stack[gameDepth].pieceCounts[i];
      if (np == 0)
        continue;

      for (j = 0; j < np; j++)
        {
          GenMovesForAWhitePiece (0, i, Stack[gameDepth].pieceAPosition[i][j]);
        }
    }
  Close_Movelist ();
}
// -------------------------------------------------
// Find all the moves on a Board for BLACK and store in BMoves
// -------------------------------------------------
void
FindMovesBlack (int indExchange)
{
  Clear_Movelist ();

  int i, j, np;

  //		 indExchange = 0;

  for (i = 7; i <= 12; i++)
    {
      np = Stack[gameDepth].pieceCounts[i];
      if (np == 0)
        continue;

      for (j = 0; j < np; j++)
        {
          GenMovesForABlackPiece (indExchange, i,
                                  Stack[gameDepth].pieceAPosition[i][j]);
        }
    }
  Close_Movelist ();
  return;
}
/*
// Check Test First
void FindMovesBlackCh(SBoard &Board, Movestruct &Moves)
{
        Moves.ischeck = IsCheck (BLACK, Board);
        FindMovesBlack ( Board, Moves );
}
*/
// Rook and Queen Moves only
void
FindHeavyMovesBlack (int color)
{
  int i, j, np;
  Clear_Movelist ();

  for (i = 8; i <= 11; i++)
    {
      if (i == 9 || i == 10)
        continue;
      np = Stack[gameDepth].pieceCounts[i];
      if (np == 0)
        continue;

      for (j = 0; j < np; j++)
        {
          GenMovesForABlackPiece (0, i, Stack[gameDepth].pieceAPosition[i][j]);
        }
    }
  Close_Movelist ();
}
// -----------------------------------
// Find all ATTACKING moves on a Board for WHITE
// -----------------------------------
void
FindAttackMovesWhite ()
{
  int i, j, np;

  Clear_Movelist ();

  //		indExchange = 22;

  for (i = 1; i <= 6; i++)
    {
      np = Stack[gameDepth].pieceCounts[i];
      if (np == 0)
        continue;

      for (j = 0; j < np; j++)
        {
          GenMovesForAWhitePiece (22, i,
                                  Stack[gameDepth].pieceAPosition[i][j]);
        }
    }
  Close_Movelist ();
  //		indExchange = 0;
  return;
}
// -----------------------------------
// Find all the moves on a Board for WHITE and store in Attacks or NoAttacks at
// level gameDepth
// -----------------------------------
void
FindAttackMovesBlack ()
{
  int i, j, np;

  Clear_Movelist ();

  //	 indExchange = 22;

  for (i = 7; i <= 12; i++)
    {
      np = Stack[gameDepth].pieceCounts[i];
      if (np == 0)
        continue;

      for (j = 0; j < np; j++)
        {
          GenMovesForABlackPiece (22, i,
                                  Stack[gameDepth].pieceAPosition[i][j]);
        }
    }
  Close_Movelist ();
  //	indExchange = 0;
  return;
}

// ----------------
// Returns 1 if a the piece on square is attacked by the opposite color
// ----------------
int inline IsAttacked (int square, int color)
{

  if (color == WHITE)
    {
      if (DistKing[square][Stack[gameDepth].blackKingSquare] <= 1)
        return 1;
      return TablaWhiteIsCheck (square);
    }
  else
    {

      if (DistKing[square][Stack[gameDepth].whiteKingSquare] <= 1)
        return 1;
      return TablaBlackIsCheck (square);
    }
}

// -----------------
// Add a Move to Movelist
// -----------------
void inline AddMove (int dst, int Move)
{
  int SqFrom = (Move >> 8) & 255;
  int SqTo = Move & 255;
  int PieceMoved = tabla[SqFrom];
  int PieceTaken = tabla[SqTo];
  if (PieceMoved == 0)
    {
      who = who;
    }
  *pma = (PieceMoved << 20) + (PieceTaken << 16) + Move;
  pma++;
}

void inline AddAtkMove (int dst, int Move)
{
  int SqFrom = (Move >> 8) & 255;
  int SqTo = Move & 255;
  int PieceMoved = tabla[SqFrom];
  if (PieceMoved == 0)
    {
      who = who;
    }

  int PieceTaken = tabla[SqTo];
  //	pma->Indicators = 0;
  //	pma++;
  *pma = (PieceMoved << 20) + (PieceTaken << 16) + Move;
  pma++;
}
/*
// -----------------
// Add All Moves possible for nDirec
// -----------------
void inline SlideDirecW (  int Sq, int x, int dir, int Move)
{
        int nSlide = x + dir;
        while (tabla[nSlide ] == 0)
                {
                        AddMove ( nSlide, Move );
                        nSlide+= dir;
                }
        if (tabla[ nSlide ] > WK) AddAtkMove (nSlide, Move );
}

void inline SlideDirecB ( int Sq, int x, int nDirec, int Move)
{
        int nSlide = x + nDirec;
        while (tabla[ nSlide ] == 0)
                {AddMove (nSlide, Move ); nSlide+= nDirec;}
        if (tabla[ nSlide ] < BP) AddAtkMove (nSlide, Move );
}
// -----------------
// Add Attack Moves for nDirec
// -----------------
void inline SlideDirecAtkW (int Sq, int x, int nDirec, int Move)
{
        int nSlide = x + nDirec;
        while (tabla[ nSlide ] == 0) nSlide+= nDirec;
        if (tabla[ nSlide ] > WK) AddAtkMove (nSlide, Move );
}

void inline SlideDirecAtkB (int Sq, int x, int nDirec, int Move)
{
        int nSlide = x + nDirec;
        while (tabla [ nSlide ] == 0) nSlide+= nDirec;
        if (tabla [ nSlide ] < BP) AddAtkMove (nSlide, Move );
}
//------------------
// Add Move for nSq if possible
//------------------
void inline AddSquareB (  int nSq, int Sq,  int Move)
{
        if ( tabla[ nSq ] == 0) AddMove    (nSq, Move );
        else if ( tabla[ nSq ] < BP) AddAtkMove (nSq, Move );
}

void inline AddSquareW ( int nSq, int Sq,   int Move)
{
        if ( tabla[ nSq ] == 0) AddMove    ( nSq, Move );
        else if ( tabla[ nSq ] > WK) AddAtkMove (nSq, Move );
}

// ----------------
// Find ROOK Moves
// ----------------
void inline FindMovesBlackRook( int Sq, int x, int nMove)
{
        for (int i = 0; i < 4; i++) SlideDirecB ( Sq, x, dr[i], nMove);
}

void inline FindMovesWhiteRook(int Sq, int x, int nMove)
{
        for (int i = 0; i < 4; i++) SlideDirecW (Sq, x, dr[i], nMove);
}

// ----------------
// Find BISHOP Moves
// ----------------
void inline FindMovesBlackBishop(int Sq, int x, int nMove)
{
        for (int i = 0; i < 4; i++) SlideDirecB (Sq, x, db[i], nMove);
}

void inline FindMovesWhiteBishop(int Sq, int x, int nMove)
{
        for (int i = 0; i < 4; i++) SlideDirecW ( Sq, x, db[i], nMove);
}

// ----------------
// Find QUEEN Moves
// ----------------
void inline FindMovesBlackQueen(int Sq, int x, int nMove)
{
        for (int i = 0; i < 8; i++) SlideDirecB (Sq, x, dq[i], nMove);
}

void inline FindMovesWhiteQueen(int Sq, int x, int nMove)
{
        for (int i = 0; i < 8; i++) SlideDirecW (Sq, x, dq[i], nMove);
}

// ----------------
// Find KING Moves
// ----------------
void inline FindMovesBlackKing(int Sq, int x, int nMove)
{
        for (int i = 0; i < 8; i++) AddSquareB ( x + dk[i],Sq, nMove);
}

void inline FindMovesWhiteKing (int Sq, int x, int nMove)
{
        for (int i = 0; i < 8; i++) AddSquareW ( x + dk[i],Sq, nMove);
}

// ----------------
// Find KNIGHT Moves
// ----------------
void inline FindMovesBlackKnight( int Sq, int x, int nMove)
{
        for (int i = 0; i < 8; i++) AddSquareB ( x + dn[i],Sq, nMove);
}

void inline FindMovesWhiteKnight (int Sq, int x, int nMove)
{
        for (int i = 0; i < 8; i++) AddSquareW ( x + dn[i],Sq, nMove);
}
*/
// ----------------
// Find LEGAL KING Moves
// ----------------
void inline AddIfLegalB (int Src, int Dst, int piece, int color, int Move)
{
  int pie;

  pie = tabla[Src];
  if (tabla[Dst] == 0)
    {
      tabla[Src] = 0;
      if (IsAttacked (Dst, color) == 0)
        {
          tabla[Src] = pie;
          AddMove (Dst, Move);
        }
      else
        {
          tabla[Src] = pie;
        }
    }
  else if (tabla[Dst] < BP && tabla[Dst] > 0)
    {
      tabla[Src] = 0;
      if (IsAttacked (Dst, color) == 0)
        {
          tabla[Src] = pie;
          AddAtkMove (Dst, Move);
        }
      else
        {
          tabla[Src] = pie;
        }
    }
}

void inline AddIfLegalW (int Src, int Dst, int piece, int color, int Move)
{
  int pie;

  pie = tabla[Src];

  if (tabla[Dst] == 0)
    {
      tabla[Src] = 0;
      if (IsAttacked (Dst, color) == 0)
        {
          tabla[Src] = pie;
          AddMove (Dst, Move);
        }
      else
        {
          tabla[Src] = pie;
        }
    }
  else if (tabla[Dst] > WK)
    {
      tabla[Src] = 0;
      if (IsAttacked (Dst, color) == 0)
        {
          tabla[Src] = pie;
          AddAtkMove (Dst, Move);
        }
      else
        {
          tabla[Src] = pie;
        }
    }
}

void inline FindLegalMovesBlackKing (int x)
{

  int i;

  //	x = Stack[gameDepth].blackKingSquare;

  for (i = 0; i < 8; i++)
    {
      AddIfLegalB (x, x + dk[i], BK, BLACK, (x << 8) | x + dk[i]);
    }
}

void inline FindLegalMovesWhiteKing (int x)
{
  int i;

  for (i = 0; i < 8; i++)
    {
      AddIfLegalW (x, x + dk[i], WK, WHITE, (x << 8) | x + dk[i]);
    }
}
/*
// ----------------
// Find PAWN Moves
// ----------------
void inline FindMovesWhitePawn(int Sq, int x, int Move, int nSqEnPassant)
{
 if (tabla [ x-10 ] == 0)
        {
//	 if (SQRANK[x] == 7) AddAtkMove(x - 10, nMove );
//	 else  {AddMove (x - 10, Move );

         if (x > 81 && tabla [x - 20] == 0) AddMove (x - 20, Move );
        }
// if (tabla [ x-11 ] >8 || (x-11) == nSqEnPassant)  AddAtkMove (x-11, Move );
// if (tabla [ x-9 ]  >8 || (x-9)  == nSqEnPassant)  AddAtkMove (x-9, Move );
}

void inline FindMovesBlackPawn(int Sq, int x, int Move, int nSqEnPassant)
{
 if (tabla [ x+10 ] == 0)
        {
//	 if (SQRANK[x] == 2) AddAtkMove (x+10, Move );
//	 else	{AddMove (x+10, Move );
                         if (x < 42 && tabla [x+20] == 0) AddMove (x+20, Move
);
        }
 if ((tabla [ x+11 ] < 8 && tabla  [ x+11 ] > 0) || (x+11) == nSqEnPassant)
AddAtkMove (x+11, Move ); if ((tabla  [ x+9 ] < 8  && tabla [ x+9 ] > 0)  ||
(x+9)  == nSqEnPassant)  AddAtkMove (x+9,  Move );
}

// ----------------
// Find KNIGHT Attack Moves
// ----------------
void inline FindAMovesBlackKnight(int Sq, int x,int Move)
{
        for (int i = 0; i < 8; i++)
                if ( tabla[ x + dn[i] ] < BP && tabla[ x + dn[i]  ] > 0)
AddAtkMove (x + dn[i], Move );
}

void inline FindAMovesWhiteKnight(int Sq, int x, int Move)
{
        for (int i = 0; i < 8; i++)
                if ( tabla[ x + dn[i] ] > WK) AddAtkMove (x + dn[i], Move );
}

// ----------------
// Find KING Attack Moves
// ----------------
void inline FindAMovesBlackKing (int Sq, int x, int Move)
{
        for (int i = 0; i < 8; i++)
                if ( tabla[ x + dk[i] ] < BP && tabla[ x + dk[i]  ] > 0)
AddAtkMove (x + dk[i], Move );
}

void inline FindAMovesWhiteKing (int Sq, int x, int Move)
{
        for (int i = 0; i < 8; i++)
                if ( tabla[ x + dk[i] ] > WK) AddAtkMove (x + dk[i], Move );
}


// ----------------
// Find PAWN Attack Moves ( & Promotions )
// ----------------
void inline FindAMovesWhitePawn(int Sq, int x, int Move, int nSqEnPassant)
{
 if (tabla [ x-11 ] > WK || (x-11) == nSqEnPassant)  AddAtkMove (x-11, Move );
 if (tabla [ x-9 ]  > WK || (x-9)  == nSqEnPassant)  AddAtkMove (x-9, Move );
// if ( SQRANK [ x ] == 7 && tabla[ x - 10] == 0) AddAtkMove (x-10, Move);
}

void inline FindAMovesBlackPawn(int Sq, int x, int Move, int nSqEnPassant)
{
 if ( (tabla [ x+11 ] < BP && tabla [ x+11 ] > 0) || (x+11) == nSqEnPassant)
AddAtkMove (x+11, Move ); if ( (tabla [ x+9 ]  < BP && tabla [ x+9  ] > 0) ||
(x+9)  == nSqEnPassant)  AddAtkMove (x+9, Move );
// if ( SQRANK [ x ] == 2 && tabla [ x + 10] == 0) AddAtkMove (x+10, Move);
}

// ----------------
// Find ROOK Attack Moves
// ----------------
void inline FindAMovesBlackRook(int Sq, int x, int nMove)
{
        for (int i = 0; i < 4; i++) SlideDirecAtkB (Sq, x, dr[i], nMove);
}

void inline FindAMovesWhiteRook(int Sq, int x, int nMove)
{
        for (int i = 0; i < 4; i++) SlideDirecAtkW (Sq, x, dr[i], nMove);
}

// ----------------
// Find BISHOP Attack Moves
// ----------------
void inline FindAMovesBlackBishop(int Sq, int x, int nMove)
{
        for (int i = 0; i < 4; i++) SlideDirecAtkB (Sq, x, db[i], nMove);
}

void inline FindAMovesWhiteBishop(int Sq, int x, int nMove)
{
        for (int i = 0; i < 4; i++) SlideDirecAtkW (Sq, x, db[i], nMove);
}

// ----------------
// Find QUEEN Attack Moves
// ----------------
void inline FindAMovesBlackQueen(int Sq, int x, int nMove)
{
        for (int i = 0; i < 8; i++) SlideDirecAtkB (Sq, x, dq[i], nMove);
}

void inline FindAMovesWhiteQueen(int Sq, int x, int nMove)
{
        for (int i = 0; i < 8; i++) SlideDirecAtkW (Sq, x, dq[i], nMove);
}
*/
// -------------------------
//
// Generate Legal Responses to a Check
//
// -------------------------
void inline AddMoveW (int src, int dst, int bAttack, int indi)
{
  register int nPiece;
  int nPiece2;
  nPiece = tabla[src];
  nPiece2 = tabla[dst];
  if (bAttack == 2 || nPiece2 == BK
      || !DiscoveredCheck (WHITE, dst | (src << 8) | (nPiece2 << 16)
                                      | (nPiece << 20) | (indi << 24)))
    {

      *pma
          = dst | (src << 8) | (nPiece2 << 16) | (nPiece << 20) | (indi << 24);
      *pma++;
    }
}

void inline TestAddDirecW (int sq, int nDirec, int piece1, int piece2,
                           int bAttack)
{
  int tempSq = sq + nDirec;
  while (tabla[tempSq] == 0)
    tempSq += nDirec;
  if (tabla[tempSq] == piece1 || tabla[tempSq] == piece2)
    {
      AddMoveW (tempSq, sq, bAttack, 0);
    }
}
//
// Find the moves
//
void inline FindWhiteMovesToSq (int sq, int bAtk)
{
  int indi;

  if (bAtk > 0)
    {

      if (tabla[sq - 11] == WP && tabla[sq] >= BP)
        {
          if (sq > 90)
            {
              AddMoveW (sq - 11, sq, 1, 5);
            }
          else
            {
              AddMoveW (sq - 11, sq, 1, 0);
            }
        }
      if (tabla[sq - 9] == WP && tabla[sq] >= BP)
        {
          if (sq > 90)
            {
              AddMoveW (sq - 9, sq, 1, 5);
            }
          else
            {
              AddMoveW (sq - 9, sq, 1, 0);
            }
        }

      if (Stack2[gameDepth].enPassant == (sq) && tabla[sq + 10] == 0
          && tabla[(int)Stack2[gameDepth].enPassant] == BP)
        {
          indi = 3;
          if (tabla[sq - 1] == WP)
            {
              AddMoveW (sq - 1, sq + 10, 1, indi);
            }
          if (tabla[sq + 1] == WP)
            {
              AddMoveW (sq + 1, sq + 10, 1, indi);
            }
        }
    }
  else
    {
      if (tabla[sq - 10] == WP)
        {
          if (sq > 90)
            {
              AddMoveW (sq - 10, sq, 1, 5);
              AddMoveW (sq - 10, sq, 1, 6);
              AddMoveW (sq - 10, sq, 1, 7);
              AddMoveW (sq - 10, sq, 1, 8);
            }
          else
            {
              AddMoveW (sq - 10, sq, 1, 0);
            }
        }
      if (SQRANK[sq] == 4 && tabla[sq - 20] == WP && tabla[sq - 10] == 0)
        AddMoveW (sq - 20, sq, 0, 0);
    }

  int i;
  for (i = 0; i < 8; i++)
    if (tabla[sq + dn[i]] == WN)
      AddMoveW (sq + dn[i], sq, bAtk, 0);

  for (i = 0; i < 4; i++)
    TestAddDirecW (sq, db[i], WB, WQ, bAtk);

  for (i = 0; i < 4; i++)
    TestAddDirecW (sq, dr[i], WR, WQ, bAtk);
}
//-------------
// For BLACK
//-------------
void inline AddMoveB (int src, int dst, int bAttack, int indi)
{
  register int nPiece;
  int nPiece2;

  nPiece = tabla[src];

  if (nPiece == 0)
    {
      return;
    }
  nPiece2 = tabla[dst];

  if (bAttack == 2 || nPiece2 == WK
      || !DiscoveredCheck (BLACK, dst | (src << 8) | (nPiece2 << 16)
                                      | (nPiece << 20) | (indi << 24)))
    {

      *pma
          = dst | (src << 8) | (nPiece2 << 16) | (nPiece << 20) | (indi << 24);
      *pma++;
    }
}

void inline TestAddDirecB (int sq, int nDirec, int piece1, int piece2,
                           int bAttack)
{
  int tempSq = sq + nDirec;
  while (tabla[tempSq] == 0)
    tempSq += nDirec;
  if (tabla[tempSq] == piece1 || tabla[tempSq] == piece2)
    {
      AddMoveB (tempSq, sq, bAttack, 0);
    }
}

void inline FindBlackMovesToSq (int sq, int bAtk)
{

  if (bAtk > 0)
    {

      if (tabla[sq + 11] == BP && tabla[sq] < BP && tabla[sq] > 0)
        {
          if (sq < 30)
            {
              AddMoveB (sq + 11, sq, 1, 9);
              //				AddMoveB ( sq+11,  sq, 1, 10 );
              //				AddMoveB ( sq+11,  sq, 1, 11 );
              //				AddMoveB ( sq+11,  sq, 1, 12 );
            }
          else
            {
              AddMoveB (sq + 11, sq, 1, 0);
            }
        }
      if (tabla[sq + 9] == BP /* && tabla[sq] < BP &&  tabla[sq] > 0 */)
        {
          if (sq < 30)
            {
              AddMoveB (sq + 9, sq, 1, 9);
              //				AddMoveB ( sq+9,  sq, 1, 10 );
              //				AddMoveB ( sq+9,  sq, 1, 11 );
              //				AddMoveB ( sq+9,  sq, 1, 12 );
            }
          else
            {
              AddMoveB (sq + 9, sq, 1, 0);
            }
        }

      if (Stack2[gameDepth].enPassant == (sq) && tabla[sq - 10] == 0
          && tabla[(int)Stack2[gameDepth].enPassant] == WP)
        {
          if (tabla[sq - 1] == BP)
            {
              AddMoveB (sq - 1, sq - 10, 1, 4);
            }
          if (tabla[sq + 1] == BP)
            {
              AddMoveB (sq + 1, sq - 10, 1, 4);
            }
        }
    }
  else
    {
      if (tabla[sq + 10] == BP)
        {
          if (sq < 30)
            {
              AddMoveB (sq + 10, sq, 1, 9);
              AddMoveB (sq + 10, sq, 0, 10);
              AddMoveB (sq + 10, sq, 0, 11);
              AddMoveB (sq + 10, sq, 0, 12);
            }
          else
            {
              AddMoveB (sq + 10, sq, 0, 0);
            }
        }
      if (SQRANK[sq] == 5 && tabla[sq + 20] == BP && tabla[sq + 10] == 0)
        AddMoveB (sq + 20, sq, 0, 0);
    }

  int i;
  for (i = 0; i < 8; i++)
    if (tabla[sq + dn[i]] == BN)
      AddMoveB (sq + dn[i], sq, bAtk, 0);

  for (i = 0; i < 4; i++)
    TestAddDirecB (sq, db[i], BB, BQ, bAtk);

  for (i = 0; i < 4; i++)
    TestAddDirecB (sq, dr[i], BR, BQ, bAtk);
}

//
// Generate Legal Moves to get out of Check
//
void
InCheckMoveGenW ()
{

  Clear_Movelist ();

  int nKingSq = Stack[gameDepth].whiteKingSquare;
  int nSq = Stack2[gameDepth].IsCheckOwn;
  if (nSq == 2) // Double Check, must move the king
    {
      FindLegalMovesWhiteKing (nKingSq);

      Close_Movelist ();

      return;
    }

  // Take Checking Piece
  FindWhiteMovesToSq (nSq, TRUE);
  // Move King
  FindLegalMovesWhiteKing (nKingSq);

  // Knight Check or Touch Check, can't block
  if (tabla[nSq] == BN || DistKing[nKingSq][nSq] <= 1)
    {
      Close_Movelist ();

      return;
    }

  // Blocking Moves
  int nDirec = DirKing[nSq][nKingSq];
  if (nDirec != 0)
    {
      nSq += nDirec;
      while (tabla[nSq] == 0)
        {
          FindWhiteMovesToSq (nSq, FALSE);
          nSq += nDirec;
        }
    }
  Close_Movelist ();
}

void
InCheckMoveGenB ()
{

  Clear_Movelist ();

  int nKingSq = Stack[gameDepth].blackKingSquare;
  int nSq = Stack2[gameDepth].IsCheckOwn;
  if (nSq == 2) // Double Check
    {
      FindLegalMovesBlackKing (nKingSq);

      Close_Movelist ();

      return;
    }

  // Take Checking Piece
  FindBlackMovesToSq (nSq, TRUE);
  // Move King
  FindLegalMovesBlackKing (nKingSq);
  // Knight Check or Touch Check, can't block
  if (tabla[nSq] == WN || DistKing[nKingSq][nSq] <= 1)
    {
      Close_Movelist ();

      return;
    }
  // Blocking Moves
  int nDirec = DirKing[nSq][nKingSq];
  if (nDirec != 0)
    {
      nSq += nDirec;
      while (tabla[nSq] == 0)
        {
          FindBlackMovesToSq (nSq, FALSE);
          nSq += nDirec;
        }
    }
  Close_Movelist ();
}

//
// Set the IsCheckOwn for moves
//
int
SetIsCheck (int color)
{
  int* pma10;
  int nmoves;
  int ret;
  ret = 0;
  pma10 = pma;

  if (IsCheck (color) > 0)
    {
      if (who == WHITE)
        FindBlackMovesToSq (Stack[gameDepth].whiteKingSquare, 2);
      else
        FindWhiteMovesToSq (Stack[gameDepth].blackKingSquare, 2);
      nmoves = (pma - pma10);
      if (nmoves > 1)
        ret = 2;
      else if (nmoves == 1)
        {
          ret = (*pma10 >> 8) & 255;
        }
      else
        ret = 0;
      return ret;
    }
  else
    return 0;
}

// ------------------------
// Queen Touch Mates functions : (dif, rege, regina)
// ------------------------
int
CheckSquareW (int MateSq, int nKingSq, int nQueenSq)
{
  int indRet;
  int piece;
  return 0;
  MateSq = MateSq + nKingSq;
  if (tabla[MateSq] > 0 && tabla[MateSq] < BP)
    return 0; // Friendly piece

  int nDirec = DirKing[MateSq][nQueenSq];
  if (nDirec == 0)
    return 0;
  if (CheckLine (MateSq, nQueenSq, nDirec, BQ, BQ) == 0)
    return 0;

  indRet = 1;
  piece = tabla[nQueenSq];
  tabla[nQueenSq] = 0;
  if (IsAttacked (MateSq, BLACK) == 0) // King can just take queen
    {
      tabla[nQueenSq] = piece;
      return 0;
    }

  if (TablaBlackIsCheck (tabla[MateSq]))
    indRet = 0; // Other Piece can take queen
  tabla[nQueenSq] = piece;

  if (indRet == 0)
    return 0;
  // if (DiscoveredCheck (WHITE, nKingSq, nQueenSq, MateSq, 0 ,0) ) return 0;
  // // Queen is Pinned

  return 1;
}

//
//
//
int
CheckSquareB (int MateSq, int nKingSq, int nQueenSq)
{
  int piece;
  return 0;
  MateSq = MateSq + nKingSq;
  if (tabla[MateSq] > WK)
    return 0; // Friendly piece blocking

  int nDirec = DirKing[MateSq][nQueenSq];
  // Is the queen lined up with the mating square? If so does it have a clear
  // path?
  if (nDirec == 0)
    return 0;
  if (CheckLine (MateSq, nQueenSq, nDirec, WQ, WQ) == 0)
    return 0;

  int indRet = 1;
  piece = tabla[nQueenSq];

  tabla[nQueenSq] = 0;
  if (IsAttacked (MateSq, WHITE) == 0) // King can just take queen
    {
      tabla[nQueenSq] = piece;
      return 0;
    }

  if (TablaWhiteIsCheck (tabla[MateSq]))
    indRet = 0; // Other Piece can take queen
  tabla[nQueenSq] = piece;

  if (indRet == 0)
    return 0;

  // if (DiscoveredCheck (BLACK,nKingSq, nQueenSq , MateSq, 0, 0) ) return 0;
  // // Queen is Pinned

  return 1;
}
//
//
//
int
TouchMatesW (int depth)
{

  int Ksq;
  int Qsq;
  int Qcode;
  int nKingSq;
  if (depth > 4)
    return FindMateThreat ((who + 1) & 1);

  nKingSq = Stack[gameDepth].whiteKingSquare;
  if (who == BLACK)
    {
      if (Stack[gameDepth].pieceCounts[BQ] == 0)
        return 0;
      Ksq = Stack[gameDepth].blackKingSquare;
      Qsq = Stack[gameDepth].pieceCounts[BQ];
      Qcode = BQ;
    }
  else
    {
      if (Stack[gameDepth].pieceCounts[WQ] == 0)
        return 0;
      Ksq = Stack[gameDepth].whiteKingSquare;
      Qsq = Stack[gameDepth].pieceCounts[WQ];
      Qcode = WQ;
    }

  int MateSq = EDGE[nKingSq];
  if (MateSq == 0)
    return 0;

  if (CheckSquareW (MateSq, Ksq, Qsq) == 1)
    return 1;

  if (MateSq == 9)
    {
      if (CheckSquareW (-1, Ksq, Qsq) == 1)
        return 1;
      if (CheckSquareW (+10, Ksq, Qsq) == 1)
        return 1;
      return 0;
    }
  if (MateSq == 11)
    {
      if (CheckSquareW (1, Ksq, Qsq) == 1)
        return 1;
      if (CheckSquareW (+10, Ksq, Qsq) == 1)
        return 1;
      return 0;
    }
  if (MateSq == 10)
    {
      if (tabla[Ksq + 9] > WK || tabla[Ksq + 20] == WP)
        {
          if (CheckSquareW (1, Ksq, Qsq) == 1)
            return 1;
        }

      if (tabla[Ksq - 1] > WK && (tabla[Ksq + 10] == 0 || tabla[Ksq + 9] > WK))
        return CheckSquareW (11, Ksq, Qsq);
    }

  return 0;
}

int
TouchMatesB (int depth)
{
  int Ksq;
  int Qsq;
  int Qcode;

  if (depth > 4)
    return FindMateThreat ((who + 1) & 1);

  if (who == BLACK)
    {
      if (Stack[gameDepth].pieceCounts[BQ] == 0)
        return 0;
      Ksq = Stack[gameDepth].blackKingSquare;
      Qsq = Stack[gameDepth].pieceAPosition[WQ][0];
      Qcode = BQ;
    }
  else
    {
      if (Stack[gameDepth].pieceCounts[WQ] == 0)
        return 0;
      Ksq = Stack[gameDepth].whiteKingSquare;
      Qsq = Stack[gameDepth].pieceAPosition[BQ][0];
      Qcode = WQ;
    }

  int MateSq = EDGE[Ksq];
  if (MateSq == 0)
    return 0;

  if (CheckSquareB (MateSq, Ksq, Qsq) == 1)
    return 1;

  if (MateSq == -9)
    {
      if (CheckSquareB (1, Ksq, Qsq) == 1)
        return 1;
      if (CheckSquareB (-10, Ksq, Qsq) == 1)
        return 1;
      return 0;
    }
  if (MateSq == -11)
    {
      if (CheckSquareB (-1, Ksq, Qsq) == 1)
        return 1;
      if (CheckSquareB (-10, Ksq, Qsq) == 1)
        return 1;
      return 0;
    }
  if (MateSq == -10)
    {
      if ((tabla[Ksq - 9] > 0 && tabla[Ksq - 9] < BP) || tabla[Ksq - 20] == BP)
        {
          if (CheckSquareB (-1, Ksq, Qsq) == 1)
            return 1;
        }

      if (tabla[Ksq - 1] > 0 && tabla[Ksq - 1] < BP
          && (tabla[Ksq - 10] == 0
              || (tabla[Ksq - 11] > 0 && tabla[Ksq - 11] < BP)))
        return CheckSquareB (-9, Ksq, Qsq);
    }

  return 0;
}

// ----------------------------------
//  Find a mate threat with any rook or queen move
// ----------------------------------
int
FindMateThreat (int Color)
{
  return 0;
  int anothermoveonlevel;

  Stack2[gameDepth].IsCheckOwn = 0;
  if (Color == WHITE)
    {
      FindHeavyMovesWhite (Color);
    }
  else
    {
      FindHeavyMovesBlack (Color);
    }

  for (Stack2[gameDepth].rankOnLevel = 0;
       Stack2[gameDepth].rankOnLevel < Stack2[gameDepth].movesOnLevel;)

    {
      anothermoveonlevel = TakeOneMove ();

      if (anothermoveonlevel == 0)
        break;

      if (DiscoveredCheck (Color, MoveToDo) == 1)
        continue;

      MoveToDoIsCheck = CheckingMove (Color, MoveToDo);

      PlayMove (MoveToDo);

      Stack2[gameDepth + 1].IsCheckOwn = MoveToDoIsCheck;
      if (Stack2[gameDepth].IsCheckOwn == 0)
        {
          MoveBack ();
          continue;
        }

      if (Color == WHITE)
        InCheckMoveGenB ();
      else
        InCheckMoveGenW ();

      MoveBack ();
      if (Stack2[gameDepth + 1].movesOnLevel == 0)
        return MoveToDo & 0xFFFF;
    }

  return 0;
}
// ----------------
// Returns 1 if color is currently in Check
// ----------------
int
IsCheck (int color)
{

  if (DistKing[Stack[gameDepth].whiteKingSquare]
              [Stack[gameDepth].blackKingSquare]
      <= 1)
    {
      if (color == WHITE)
        return Stack[gameDepth].blackKingSquare;
      else
        return Stack[gameDepth].whiteKingSquare;
    }

  if (color == BLACK)
    return TablaBlackIsCheck (Stack[gameDepth].blackKingSquare);
  else
    return TablaWhiteIsCheck (Stack[gameDepth].whiteKingSquare);

  return 0;
}

//
// CHECK RELATED FUNCTIONS
//
// ----------------------------------------------------------
// Check for a discovered attack on Sq from nDirection
// ----------------------------------------------------------
int inline CheckDirec (int Sq0, int nDirec, int piece1, int piece2)
{
  int Sq;
  Sq = Sq0 + nDirec;
  while (tabla[Sq] == 0)
    Sq += nDirec;
  if (tabla[Sq] == piece1 || tabla[Sq] == piece2)
    return Sq;
  return 0;
}

int inline CheckLine (int Sq0, int dstSq, int nDirec, int pie1, int pie2)
{
  int Sq;

  Sq = Sq0 + nDirec;
  while (tabla[Sq] == 0)
    Sq += nDirec;
  if (Sq == dstSq && (tabla[Sq] == pie1 || tabla[Sq] == pie2))
    return dstSq;
  return 0;
}

// ----------------------------------------------------------
//
// Test for a Discovered check
//
// ----------------------------------------------------------

int
DiscoveredCheck (int color, int Move)
{
  int bRetVal = FALSE;
  int enPassant = Stack2[gameDepth].enPassant;

  int srcSq = (Move >> 8) & 255;
  int dstSq = (Move & 255);

  int cPiece;
  // TestWrong();
  // Check Testing
  if (color == BLACK)
    {
      int KingSq = Stack[gameDepth].blackKingSquare;
      int nDirec = DirKing[KingSq][srcSq];
      if (nDirec != 0 && nDirec != DirKing[KingSq][dstSq])
        {
          // TestWrong();
          cPiece = tabla[srcSq];
          enPassant = 0;
          if ((dstSq + 10) == Stack2[gameDepth].enPassant && tabla[srcSq] == BP
              && tabla[dstSq] == 0
              && tabla[(int)Stack2[gameDepth].enPassant] == WP)
            {
              // en passant vacate 2 squares
              enPassant = Stack2[gameDepth].enPassant;
              tabla[enPassant] = 0;
            }
          tabla[srcSq] = 0;

          if (nDirec == -1 || nDirec == 1 || nDirec == -10 || nDirec == 10)
            {
              if (CheckDirec (KingSq, nDirec, WR, WQ))
                bRetVal = TRUE;
            }
          else
            {
              if (CheckDirec (KingSq, nDirec, WB, WQ))
                bRetVal = TRUE;
            }

          tabla[srcSq] = cPiece;
          if (enPassant != 0)
            tabla[enPassant] = WP;
          // TestWrong();
        }
    }
  else
    {
      int KingSq = Stack[gameDepth].whiteKingSquare;
      int nDirec = DirKing[KingSq][srcSq];
      if (nDirec != 0 && nDirec != DirKing[KingSq][dstSq])
        {
          // TestWrong();
          cPiece = tabla[srcSq];
          enPassant = 0;
          if ((dstSq - 10) == Stack2[gameDepth].enPassant && tabla[srcSq] == WP
              && tabla[dstSq] == 0
              && tabla[(int)Stack2[gameDepth].enPassant] == BP)
            {
              // en passant vacate 2 squares
              enPassant = Stack2[gameDepth].enPassant;
              tabla[enPassant] = 0;
            }
          tabla[srcSq] = 0;

          if (nDirec == -1 || nDirec == 1 || nDirec == -10 || nDirec == 10)
            {
              if (CheckDirec (KingSq, nDirec, BR, BQ))
                bRetVal = TRUE;
            }
          else
            {
              if (CheckDirec (KingSq, nDirec, BB, BQ))
                bRetVal = TRUE;
            }

          tabla[srcSq] = cPiece;
          if (enPassant != 0)
            tabla[enPassant] = BP;
          // TestWrong();
        }
    }

  return bRetVal;
}

//
// Helpers for Checking Move.
//
int inline DiscoverCheckW (int Sq, int KingSq, int nDirec)
{
  int nDirecD = DirKing[KingSq][Sq];
  if (nDirecD != 0 && nDirecD != nDirec)
    {
      if (nDirecD == -1 || nDirecD == 1 || nDirecD == -10 || nDirecD == 10)
        return CheckDirec (KingSq, nDirecD, WR, WQ);
      else
        return CheckDirec (KingSq, nDirecD, WB, WQ);
    }
  return 0;
}

int inline DiscoverCheckB (int Sq, int KingSq, int nDirec)
{
  int nDirecD = DirKing[KingSq][Sq];
  if (nDirecD != 0 && nDirecD != nDirec)
    {
      if (nDirecD == -1 || nDirecD == 1 || nDirecD == -10 || nDirecD == 10)
        return CheckDirec (KingSq, nDirecD, BR, BQ);
      else
        return CheckDirec (KingSq, nDirecD, BB, BQ);
    }
  return 0;
}

int Promo[13] = { 0, 0, 0, 0, 0, WQ, WR, WB, WN, BQ, BR, BB, BN };

// ----------------------------------------------------------
//
// Test to see if Move checks the enemy King
// returns the checking Square, (or returns the number 2 if there are 2
// checking squares)
//
// ----------------------------------------------------------

int
CheckingMove (int color, int Move)
{
  int pie;

  // return 0;
  int dstSq = (Move & 255);

  int srcSq = (Move >> 8) & 255;
  int srcSq1 = srcSq;
  int dstSq1 = dstSq;
  int nCheck = 0, nDCheck = 0;
  if (tabla[srcSq] == 0)
    {
      who = who;
    }

  if (tabla[srcSq] == BK)
    {
      // rocades of black king
      if (srcSq == 95 && dstSq == 97)
        {
          dstSq1 = 96;
          srcSq1 = 98;
        }
      if (srcSq == 95 && dstSq == 93)
        {
          dstSq1 = 94;
          srcSq1 = 91;
        }

      tabla[srcSq] = 0;
      tabla[srcSq1] = 0;
      pie = tabla[dstSq];
      tabla[dstSq1] = BR;
      tabla[dstSq] = BK;

      int KingSq = Stack[gameDepth].whiteKingSquare;

      int nDirec = DirKing[KingSq][dstSq1];

      if (nDirec == 1 || nDirec == -1 || nDirec == -10 || nDirec == 10)
        nCheck = CheckLine (KingSq, dstSq1, nDirec, WQ, WR);

      tabla[srcSq1] = BR;
      tabla[srcSq] = BK;
      tabla[dstSq1] = 0;
      tabla[dstSq] = pie;

      return nCheck;
    }
  if (tabla[srcSq] == WK)
    {
      // white rocades
      if (srcSq == 25 && dstSq == 27)
        {
          dstSq1 = 26;
          srcSq1 = 28;
        }
      if (srcSq == 25 && dstSq == 23)
        {
          dstSq1 = 24;
          srcSq1 = 21;
        }

      tabla[srcSq] = 0;
      tabla[srcSq1] = 0;
      pie = tabla[dstSq];
      tabla[dstSq1] = WR;
      tabla[dstSq] = WK;

      int KingSq = Stack[gameDepth].blackKingSquare;

      int nDirec = DirKing[KingSq][dstSq1];

      if (nDirec == 1 || nDirec == -1 || nDirec == -10 || nDirec == 10)
        nCheck = CheckLine (KingSq, dstSq1, nDirec, BQ, BR);

      tabla[srcSq1] = WR;
      tabla[srcSq] = WK;
      tabla[dstSq1] = 0;
      tabla[dstSq] = pie;

      return nCheck;
    }
  // ---------------

  int piece = tabla[srcSq];
  int oldpiece = tabla[dstSq];

  tabla[srcSq] = 0;
  tabla[dstSq] = piece;

  // WHITE moves
  if (color == WHITE)
    {
      int KingSq = Stack[gameDepth].blackKingSquare;

      int nDirec = DirKing[KingSq][dstSq];

      int indi = (MoveToDo >> 24) & 15;

      if (piece == WP && SQRANK[dstSq] == 8)
        {
          tabla[dstSq] = Promo[indi];
        }
      // tabla[srcSq] is empty now!!
      switch (tabla[dstSq])
        {
        case WP:
          if ((dstSq == KingSq - 9) || (dstSq == KingSq - 11))
            nCheck = dstSq;
          // En Passant captures vacate 2 squares, so we have to have to check
          // the captured piece square for discovered too
          else if ((dstSq - 10) == Stack2[gameDepth].enPassant
                   && oldpiece == 0)
            {
              tabla[dstSq - 10] = 0;

              nCheck = DiscoverCheckW (dstSq - 10, KingSq,
                                       DirKing[KingSq][dstSq - 10]);

              tabla[dstSq - 10] = BP;
            }
          break;
        case WB:
          if (nDirec == -9 || nDirec == 9 || nDirec == -11 || nDirec == 11)
            nCheck = CheckLine (KingSq, dstSq, nDirec, WQ, WB);
          break;
        case WN:
          if (KnightKing[dstSq][KingSq] & 1)
            nCheck = dstSq;
          break;
        case WR:
          if (nDirec == -1 || nDirec == 1 || nDirec == -10 || nDirec == 10)
            nCheck = CheckLine (KingSq, dstSq, nDirec, WQ, WR);
          break;
        case WQ:
          if (nDirec)
            nCheck = CheckLine (KingSq, dstSq, nDirec, WQ, WQ);
          break;
        }

      // Discovered Checks
      if (piece != WQ)
        nDCheck = DiscoverCheckW (srcSq, KingSq, nDirec);
    }
  // Same as above, except for BLACK
  else
    {

      int KingSq = Stack[gameDepth].whiteKingSquare;
      int nDirec = DirKing[KingSq][dstSq];
      int indi = (MoveToDo >> 24) & 15;
      ;
      if (piece == BP && SQRANK[dstSq] == 1)
        {
          tabla[dstSq] = Promo[indi];
        }
      // tabla[srcSq] is empty now!!

      switch (tabla[dstSq])
        {
        case BP:
          if (dstSq == KingSq + 9 || dstSq == KingSq + 11)
            nCheck = dstSq;
          // En Passant captures vacate 2 squares
          else if ((dstSq + 10) == Stack2[gameDepth].enPassant
                   && oldpiece == 0)

            {
              tabla[dstSq + 10] = 0;
              nCheck = DiscoverCheckB (dstSq + 10, KingSq,
                                       DirKing[KingSq][dstSq + 10]);
              tabla[dstSq + 10] = WP;
            }
          break;
        case BB:
          if (nDirec == -9 || nDirec == 9 || nDirec == -11 || nDirec == 11)
            nCheck = CheckLine (KingSq, dstSq, nDirec, BQ, BB);
          break;
        case BN:
          if (KnightKing[dstSq][KingSq] & 1)
            nCheck = dstSq;
          break;
        case BR:
          if (nDirec == -1 || nDirec == 1 || nDirec == -10 || nDirec == 10)
            nCheck = CheckLine (KingSq, dstSq, nDirec, BQ, BR);
          break;
        case BQ:
          if (nDirec)
            nCheck = CheckLine (KingSq, dstSq, nDirec, BQ, BQ);
          break;
        }

      // Discovered Checks
      if (piece != BQ)
        nDCheck = DiscoverCheckB (srcSq, KingSq, nDirec);
    }

  tabla[dstSq] = oldpiece;
  tabla[srcSq] = piece;

  if (nCheck == 0)
    return nDCheck;
  else if (nDCheck)
    return 2; // 2 Checking Pieces
  return nCheck;
}

// -------------------------------------
// Queen is beetween the own king and a enemy rook or bishop
// -------------------------------------
int
QueenAttacked ()
{
  // I may have several Queens on the table: take one of them
  int Ksq, Qsq;
  int Qcode;
  int dir;
  int TempSq;

  if (who == BLACK)
    {
      if (Stack[gameDepth].pieceCounts[BQ] == 0)
        return 0;
      Ksq = Stack[gameDepth].blackKingSquare;
      Qsq = Stack[gameDepth].pieceAPosition[BQ][0];
      Qcode = BQ;
    }
  else
    {
      if (Stack[gameDepth].pieceCounts[WQ] == 0)
        return 0;
      Ksq = Stack[gameDepth].whiteKingSquare;
      Qsq = Stack[gameDepth].pieceAPosition[WQ][0];
      Qcode = WQ;
    }

  if ((DirKing[Ksq][Qsq]) != 0)
    {
      // queen and king on the same diagonal or file or line

      dir = DirKing[Ksq][Qsq];
      TempSq = Ksq + dir;
      while (tabla[TempSq] == 0)
        TempSq += dir;
      if (tabla[TempSq] == Qcode)
        {
          TempSq += dir;
          while (tabla[TempSq] == 0)
            TempSq += dir;
          if (dir == 9 || dir == 11 || dir == -9 || dir == -11)
            {
              if (tabla[TempSq] == BB && Qcode == WQ)
                return 1;
              if (tabla[TempSq] == WB && Qcode == BQ)
                return 1;
            }
          else if (tabla[TempSq] == BR && Qcode == WQ)
            return 1;
          if (tabla[TempSq] == WR && Qcode == BQ)
            return 1;
        }
    }
  return 0;
}
