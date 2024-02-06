/*
 * Copyright (c) 2024 Viorel Darie. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

#include <windows.h>
#include <cstdlib>
// -------- Defines for board and various other things (don't change these)
// --------

#define BOTH 3
#define NONE 4
#define START 5
#define NO_BEST 65535

#define EMPTY 0

#define INVALID 8

#define TIMEOUT 31000
// #define MATE 9999

#define CAPTURED 1
#define PD 0

#define WHITEWINS 1
#define BLACKWINS 2
#define DRAW 3

#define KSCASTLE 15
#define QSCASTLE 16

#define CAPTURE 30990
#define PAWNCAP 30991
#define KINGSAFETY 30992
#define RECAP 30993
#define MATETHREAT 30994
#define ENPASSANT 30995
#define NO_EXT 30996 // > Means the search won't stop
#define QUEENRECAP 31000
#define KINGPAWNS 30999
#define QUEENTHREAT 31000
#define MATETHREAT2 31001
#define PAWNPUSH 31002 // >= Pawnpush triggers check/pawnpush extension
#define PAWNPUSH2 31003
#define KINGSAFETY2 31004
#define KINGPAWNS2 31005
// -----------------------------
// Init Square Value Array
// -----------------------------
// int SquareVals [ 128 * 16 ];
short BPawnValue[128];
short BPawnValueQS[128];
short BPawnValueOK[128];
short BPawnValueQSOK[128];
short BPawnValueEnd[128];
short BPawnIsolated[128];
// short BKnightPost[ 128 ];
// unsigned int KnightAttacks [ 256 ];
// unsigned int KingAttacks [ 256 ];
// unsigned int TouchChecks [ 256 ];

void
InitSquareArray ()
{
  int i, x, y;

  i = BP;
  for (x = 0; x < 8; x++)
    for (y = 0; y < 8; y++)
      {
        BPawnValue[SQ (x, y)] = -WPawnPos[SQ (x, 7 - y)];
        BPawnValueEnd[SQ (x, y)] = -WPawnValueEnd[SQ (x, 7 - y)];
        BPawnValueQS[SQ (x, y)] = -WPawnValueQS[SQ (x, 7 - y)];
        BPawnValueQSOK[SQ (x, y)] = -WPawnValueQSOK[SQ (x, 7 - y)];
        BPawnValueOK[SQ (x, y)] = -WPawnValueOK[SQ (x, 7 - y)];
        BPawnIsolated[SQ (x, y)] = WPawnIsolated[SQ (x, 7 - y)];
      }
}

#define PLY 32
const int HALFPLY = PLY / 2;
const int QUARTPLY = HALFPLY / 2;
const int THREEQUARTPLY = QUARTPLY * 3;

void inline Swap (int& x1, int& x2)
{
  int temp = x1;
  x1 = x2;
  x2 = temp;
}

void inline Swap (unsigned char& x1, unsigned char& x2)
{
  unsigned char temp = x1;
  x1 = x2;
  x2 = temp;
}

const unsigned char BITMASK[9] = { 0, 1, 2, 4, 8, 16, 32, 64, 128 };

const int F_MARGIN = 100; // Main Search
const int FUTILE_MARGIN = 130; // QSearch
const int LAZY_MARGIN = 515;
const int LAZY_MARGIN2 = 180;

struct PawnHashEntry
{
  unsigned int checksum;
  short eval, flags;
  short Mat[4];
  short MatEnd;
  unsigned char PawnFileL, PawnFileR;
  unsigned char OpenFileW, OpenFileB;
  unsigned char WeakFileB, WeakFileW;
  unsigned char WPassSqs[4], BPassSqs[4];
  short PassedCon, extra;
};

int
Liquidate ();
int
EvaluateBoard (int alpha, int beta);
void
Clear_PawnHash (void);
unsigned int
Pawn_HashBoard ();
void
PawnRookEval (int& PawnEval, int& WRookEval, int& BRookEval);

const int KINGBYPAWN[9] = { 0, 0, 0, 0, 0, 45, 75, 100, 0 };
const int KINGPAWNDIST[16] = { 0, 0, 2, 12, 20, 28, 36, 40, 44, 44 };
const int BLOCKEDPASSPAWN[9] = { 0, 0, 0, 0, 8, 15, 20, 35, 0 };
const int WBLOCKER[13] = { 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0 };
const int BBLOCKER[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1 };
#define ROOK_BEHIND_PP 21 // behind a passed pawn
const int ROOK_PP[30] = {
  0, 0, ROOK_BEHIND_PP,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, -ROOK_BEHIND_PP, 0, 0, 0, 0, 0, 0, 0
};
const int ROOK_PP1W[30] = { 0, 0, 14, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0,  0, 0, 0, 0, -6, 0, 0, 0, 0, 0, 0, 0 };
const int ROOK_PP1B[30] = { 0, 0, 6, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, -14, 0, 0, 0, 0, 0, 0, 0 };

// If one side is up in material, it gets a bonus, using the number of total
// pieces on the board as the index into this array.
const int LIQUIDATION[36]
    = { 36, 36, 36, 36, 34, 32, 24, 16, 12, 8, 4, 2, 0, 0, 0, 0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0 };
#define MORE_PIECES 70

int inline MatEval (int nWP, int nBP, const int& MatVal)
{
  int eval = 0;

  if (Stack[gameDepth].pieceCounts[WQ] > 0)
    nWP++; // Queen counts as 2 for this code
  if (Stack[gameDepth].pieceCounts[BQ] > 0)
    nBP++;
  // More Pieces/ Number of pieces left
  if (nWP > nBP)
    eval += MORE_PIECES;
  if (nBP > nWP)
    eval -= MORE_PIECES;

  if (MatVal > 178
      && Stack[gameDepth].pieceCounts[WP] > Stack[gameDepth].pieceCounts[BP])
    eval += LIQUIDATION[nBP + nWP];
  if (MatVal < -178
      && Stack[gameDepth].pieceCounts[BP] > Stack[gameDepth].pieceCounts[WP])
    eval -= LIQUIDATION[nBP + nWP];

  return eval;
}

int inline ChessDistW (const int& nSq1, const int& nSq2)
{
  int Dist1 = SQRANK[nSq1] - SQRANK[nSq2];
  int Dist2 = abs (SQFILE[nSq1] - SQFILE[nSq2]);
  return (Dist1 > Dist2) ? Dist1 : Dist2;
}

int inline ChessDistB (const int& nSq1, const int& nSq2)
{
  int Dist1 = SQRANK[nSq2] - SQRANK[nSq1];
  int Dist2 = abs (SQFILE[nSq1] - SQFILE[nSq2]);
  return (Dist1 > Dist2) ? Dist1 : Dist2;
}

// Endgame Evaluation
int
OnePieceEndgame (int& eval, const int nWKingSq, const int nBKingSq,
                 const PawnHashEntry& SPawns, const int WMat, const int BMat);
int
KingAndPawns (int& eval, const int nWKingSq, const int nBKingSq,
              const PawnHashEntry& SPawns);

void
InitEndgameTables ();
int
ProbePawnBitbases (int& eval, const int nWKingSq, const int nBKingSq);
int
ProbeBitbases (int& eval, const int nWKingSq, const int nBKingSq);

// ==========================
// Pawn Hashing
// ==========================
// DATA
unsigned int WPHashSq[128];
unsigned int BPHashSq[128];
#define PAWNHASHTABLESIZE ((2 << 15) - 1)

PawnHashEntry* PawnTable = (PawnHashEntry*)malloc (sizeof (PawnHashEntry)
                                                   * (PAWNHASHTABLESIZE + 1));

// --------------------------------------------------
void
Clear_PawnHash ()
{
  ZeroMemory (PawnTable, sizeof (PawnHashEntry) * PAWNHASHTABLESIZE);
}

// --------------------------------------------------
//  Pawn LookUp
// --------------------------------------------------
int inline PawnHash_Lookup (PawnHashEntry& TEntry,
                            const unsigned int nCheckSum)
{
  register int index = nCheckSum & PAWNHASHTABLESIZE;

  if (PawnTable[index].checksum == nCheckSum)
    {
      TEntry = PawnTable[index];
      return TRUE;
    }
  return FALSE;
}

// --------------------------------------------------
//  Pawn HashBoard
// --------------------------------------------------
unsigned int
Pawn_HashBoard ()
{
  unsigned int sum = 0;
  int i;

  for (i = 0; i < Stack[gameDepth].pieceCounts[WP]; i++)
    {
      if (Stack[gameDepth].pieceAPosition[WP][i] > 0)
        sum += WPHashSq[(int)Stack[gameDepth].pieceAPosition[WP][i]];
    }
  for (i = 0; i < Stack[gameDepth].pieceCounts[BP]; i++)
    {
      if (Stack[gameDepth].pieceAPosition[BP][i] > 0)
        sum += WPHashSq[(int)Stack[gameDepth].pieceAPosition[BP][i]];
    }

  return sum;
}

// --------------------------------------------------
//
//  Evaluate the pawn structure on the Board
//
// --------------------------------------------------
#define ISLAND_V 5
#define WEAK_OPENFILE 6
#define BACKWARD1_V 10
const int DOUBLED_V[9]
    = { 0, 0, 12, 26, 45, 65, 85, 105, 105 }; // number of pawns on a file
const int PASSED_RANK[9] = { 0, 0, 30, 32, 50, 70, 90, 150, 0 };
const int PASSEDBE_RANK[9] = { 0, 0, 16, 22, 25, 34, 60, 85, 0 };
const int CONNECTED_RANK[9] = { 0, 0, 0, 2, 5, 18, 32, 52, 0 };

int inline FreeSqCoveredByPawnB (const int nSq)
{
  if ((tabla[nSq]) == BP || tabla[nSq + 9] == BP || tabla[nSq + 11] == BP)
    return 1;
  return 0;
}

int inline FreeSqCoveredByPawnW (const int nSq)
{
  if ((tabla[nSq]) == WP || tabla[nSq - 11] == WP || tabla[nSq - 9] == WP)
    return 1;
  return 0;
}
//

//
// So it can display this term in the board flags display
//
int
Liquidate ()
{
  return MatEval (Stack[gameDepth].nWP, Stack[gameDepth].nBP,
                  Stack[gameDepth].ponder);
}

//
// ROOKS
//
const int RPAWNSUPPORT[12] = { 0, 20, 15, 9, 0, 0, 9, 15, 20 };
const int R_Tropism[16] = { 11, 6, 2, 1, 0, 0, 0, 0 }; // Half open file
const int R_TropismOpen[16] = { 22, 12, 6, 3, 0, 0, 0, 0 }; // Open File
#define ROOK_HALF 10
#define ROOK_FULL 21 // Rook on open file
#define KINGTRAPSROOK 29
#define ROOKS_7 10
#define ROOKS_8 18
#define ROOKS_7K8 \
  52 // ENDR7_K8 is in addition, as well as the piece square tables
#define QR7_K8 70
#define ENDR7_K8 34
const int BRMOVE[16] = { 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const int WRMOVE[16] = { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0 };
const int R_MovesValOpen[16]
    = { -16, -6, 0, 2, 4, 6, 8, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
const int R_MovesValClosed[16]
    = { -16, -14, -2, 0, 2, 4, 6, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
// Converts (x,y) (range: 0 to 7) coordinates to a single index into the board
int inline SQ (int x, int y)
{
  return 21 + x + y * 10;
}

//
// BISHOPS
//
const int BISHOP_PAIR = 50; // era 20
const int BISHOP_PAIRENDGAME = 60; // era 35
const int BISHOP_BEHINDPAWN = -6;
const int BISHOP_PROTECTED = 16; // era 5
const int BISHOP_BLOCKSPAWN = -8;
const int BISHOP_TRAPPED = -150;

const int B_Tropism[16] = { 9, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const int B_Tropism2[16]
    = { 7, 10, 10, 8, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const int B_MovesVal[16]
    = { -12, -6, -2, 0, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

//
// KNIGHTS
//
const int N_Tropism[16]
    = { 12, 11, 11, 14, 12, 10, 7, 6, 4, 2, 0, 0, 0, 0, 0, 0 };
#define KNIGHT_UNSAFE -22;
#define KNIGHT_NOFORWARD -8;

#define KING_PAWNCOVERFRONT 14
#define KING_PAWNCOVERFRONT2 10
#define KING_PAWNCOVER 11
#define KING_PAWNCOVER2 8
#define MAX_PAWNCOVER 18

//
// Don't return good evals for a side if it can't win (or is very unlikely to
// win.) Called when no pawns are on the board for the side in the funtion
// name, and the eval favors that side
//
int inline WhiteCantWin (int& eval)
{
  if (Stack[gameDepth].whitePower < 400)
    {
      eval = -Stack[gameDepth].pieceCounts[BP];
      return TRUE;
    }
  if (Stack[gameDepth].whitePower == 2 * ValPiesa[WN])
    {
      eval = (eval >> 3);
      return TRUE;
    }

  if (Stack[gameDepth].nWP == (Stack[gameDepth].nBP + 1)
      && Stack[gameDepth].nWMinors == (Stack[gameDepth].nBMinors + 1)
      && (Stack[gameDepth].whitePower + Stack[gameDepth].pieceCounts[WP] * 100)
             < 400)
    {
      if (Stack[gameDepth].pieceCounts[WP] > 2)
        eval = (eval >> 1);
      else
        {
          if (Stack[gameDepth].whitePower == ValPiesa[WR] + ValPiesa[WB])
            eval = (eval >> 2);
          else
            eval = (eval >> 3);
        }
      return TRUE;
    }
  return FALSE;
}

int inline BlackCantWin (int& eval)
{
  if (Stack[gameDepth].blackPower < 400)
    {
      eval = Stack[gameDepth].pieceCounts[WP];
      return TRUE;
    }
  if (Stack[gameDepth].blackPower == 2 * ValPiesa[WN])
    {
      eval = (eval >> 3);
      return TRUE;
    }

  if (Stack[gameDepth].nBP == (Stack[gameDepth].nWP + 1)
      && Stack[gameDepth].nBMinors == (Stack[gameDepth].nWMinors + 1)
      && (Stack[gameDepth].blackPower - Stack[gameDepth].pieceCounts[BP] * 100)
             < 400)
    {
      if (Stack[gameDepth].pieceCounts[BP] > 2)
        eval = (eval >> 1);
      else
        {
          if (Stack[gameDepth].blackPower == ValPiesa[WR] + ValPiesa[WB])
            eval = (eval >> 2);
          else
            eval = (eval >> 3);
        }
      return TRUE;
    }
  return FALSE;
}

const int Q_Tropism[16]
    = { 22, 26, 23, 21, 17, 14, 12, 9, 7, 5, 4, 2, 1, 0, 0, 0 };
#define QUEENATTACKER 24
#define OUTSIDE_PAWN_END 32
const int BISH_ROOK = ValPiesa[WR] + ValPiesa[WB];

int WTropism, BTropism;
int WPawnCover, BPawnCover;
// ==========================
//
//  BOARD EVALUATION
//
// (This isn't the full Eval Function, some parts are elsewhere and some is
// updated incrementally ) 1 Pawn = 100
// ==========================
int
EvaluateBoard (int alpha, int beta)
{
  int eval = 0, nSq;
  int eval_save;
  int nWKingSq = Stack[gameDepth].whiteKingSquare;
  int nBKingSq = Stack[gameDepth].blackKingSquare;

  int WMat, BMat;

  int PieceMoved = Stack2[gameDepth - 1].PieceMoved;

  int wpieces = Stack[gameDepth].nWP;
  int bpieces = Stack[gameDepth].nBP;
  // piece count  ( incrementally updated by domove )
  eval += Stack[gameDepth].ponder + Stack[gameDepth].positionVal;

  // Suplimentary ponders to force pawn move and K R at corner avoiding

  //
  // Evaluare premii de stadii joc
  //
  int wpower = Stack[gameDepth].whitePower;
  int bpower = Stack[gameDepth].blackPower;

  int di = 8
           - DistKing[Stack[gameDepth].whiteKingSquare]
                     [Stack[gameDepth].blackKingSquare];

  int bonif = 0;
  if (Stack[gameDepth].nPieces <= 10)
    {
      if (bpower == 0)
        {
          if (wpower >= 960)
            bonif += MAT - (gameDepth - treeDepth) - 4000 + 40 * di;
          else if (wpower >= 640)
            bonif = 1000 + 40 * di; // + MateWithKnightBishop(); // mate with
                                    // bishop and knight
          else if (wpower >= 500)
            bonif = 1000 + 20 * di;
          else if (Stack[gameDepth].pieceCounts[WP] > 0 && wpower >= 400)
            bonif = MAT - (gameDepth - treeDepth) - 4000
                    + Stack[gameDepth].pieceAPosition[WP][0]; // pawn endgame
        }
      else if (bpower > 0 && wpower > 0 && wpower > bpower)
        {
          if (bpower < wpower / 2 && wpower >= 960)
            bonif += 5000 + 30 * di;
          else if (bpower < wpower * 0.6 && wpower >= 510)
            bonif += 2000 + 10 * di;
          else if (bpower < wpower * 0.9 && wpower >= 510)
            bonif += 50 + 5 * di;
          else if (bpower < wpower * 0.95 && wpower >= 510)
            bonif += 20 + 5 * di;
        }
      else if (wpower == 0)
        {
          if (bpower >= 960)
            bonif += -MAT + (gameDepth - treeDepth) + 4000 - 40 * di;
          else if (bpower >= 640)
            bonif = -1000 - 40 * di; // + MateWithKnightBishop(); // mate with
                                     // bishop and knight
          else if (bpower >= 500)
            bonif = -1000 - 20 * di;
          else if (Stack[gameDepth].pieceCounts[BP] > 0 && bpower >= 400)
            bonif = -MAT + (gameDepth - treeDepth) + 4000
                    - Stack[gameDepth].pieceAPosition[BP][0]; // pawn endgame
        }
      else if (bpower > 0 && wpower > 0 && bpower > wpower)
        {
          if (wpower < bpower / 2 && bpower >= 960)
            bonif -= 5000 + 30 * di;
          else if (wpower < bpower * 0.6 && bpower >= 510)
            bonif -= 2000 + 10 * di;
          else if (wpower < bpower * 0.9 && bpower >= 510)
            bonif -= 50 + 5 * di;
          else if (wpower < bpower * 0.95 && bpower >= 510)
            bonif -= 20 + 5 * di;
        }

      eval += bonif;

    } // less pieces
  WMat = Stack[gameDepth].whitePower - Stack[gameDepth].pieceCounts[WP] * 100;
  BMat = Stack[gameDepth].blackPower - Stack[gameDepth].pieceCounts[BP] * 100;

  // Early exit on the eval for fail high or low
  // I adjust to be safer because for fail lows I get a value,
  // then don't call eval from the quiescence if the capture isn't good enough
  if ((WMat > 900 || BMat > 900))
    {
      if ((eval + LAZY_MARGIN) < alpha)
        return eval + 200;
      if ((eval - LAZY_MARGIN) > beta)
        return eval - 200;
    }

  // penalties for queens early moves
  if (treeDepth < 20 && (wpower > 2000 || bpower > 2000))
    {
      if ((int)tabla[24] != WQ)
        eval -= 50;
      if ((int)tabla[94] != BQ)
        eval += 50;

      // penalties for early king moves
      if (tabla[25] != WK && tabla[27] != WK && tabla[22] != WK
          && tabla[21] != WK)
        eval -= 70;
      if (tabla[95] != BK && tabla[97] != BK && tabla[92] != BK
          && tabla[91] != BK)
        eval += 70;

      // penalty for king moves without rocades
      if (tabla[28] == WR && (tabla[26] == WK || tabla[27] == WK))
        eval -= 70;
      if (tabla[98] == BR && (tabla[96] == BK || tabla[97] == BK))
        eval += 70;

      if (tabla[34] == WK)
        eval -= 120; // strange king tendince to move to D2 early

      if (tabla[51] == WQ)
        eval -= 80; // strange queen tendince to move to A4 early
    }
  // pieces mobility
  // bonif for king trophism
  if (bpower < 200)
    eval = eval + di * 10; // di is 8 - distante beetwen kings
  if (wpower < 200)
    eval = eval - di * 10;

  if (gameDepth > 2)
    {
      if (PieceMoved < 7)
        eval += Stack2[gameDepth - 1].movesOnLevel
                - Stack2[gameDepth - 2].movesOnLevel;
      else
        eval -= Stack2[gameDepth - 1].movesOnLevel
                - Stack2[gameDepth - 2].movesOnLevel;
    }

  // bishop pair in midle and end game
  int bi = 25;
  int nPieces = Stack[gameDepth].nPieces;
  if (nPieces <= 16)
    bi = 50;
  if (Stack[gameDepth].pieceCounts[WB] == 2)
    eval += bi;
  if (Stack[gameDepth].pieceCounts[BB] == 2)
    eval -= bi;

  // tropism by queen, rook, knight
  int q1 = (int)Stack[gameDepth].pieceAPosition[WQ][0];
  int q2 = (int)Stack[gameDepth].pieceAPosition[BQ][0];

  int king1 = (int)Stack[gameDepth].pieceAPosition[WK][0];
  int king2 = (int)Stack[gameDepth].pieceAPosition[BK][0];

  int pi1;
  int pi2;

  if (q1 > 0)
    eval += (7 - DistKing[q1][king2]) * 5; // queen tropism to advers king
  if (q2 > 0)
    eval -= (7 - DistKing[q2][king1]) * 5;

  // rooks double on column, rook supporting pawns
  int r1 = (int)Stack[gameDepth].pieceAPosition[WR][0];
  int r2 = (int)Stack[gameDepth].pieceAPosition[WR][1];

  if (r1 > 0 && ((r1 % 10) == (king2 % 10)))
    eval += 5;
  if (r2 > 0 && ((r2 % 10) == (king2 % 10)))
    eval += 5;

  if (r1 > 0)
    eval += (7 - DistKing[r1][king2]) * 1; // rook tropism to advers king
  if (r2 > 0)
    eval += (7 - DistKing[r2][king2]) * 1;

  pi1 = Stack[gameDepth].pawnWOnFiles[r1 % 10];
  pi2 = Stack[gameDepth].pawnBOnFiles[r1 % 10];

  int bonus = 0;
  if (r1 != 0 && ((r1 % 10) == (r2 % 10)) && (pi1 + pi2) <= 1 && (r1 % 10) > 2
      && (r1 % 10) < 7)
    {
      bonus = 20 - (pi1 + pi2) * 10; // bonus for rooks on the same column
    }
  else if (r1 > 0 && nPieces < 16 && (pi1 + pi2) == 1)
    bonus = 10; // bonus for rooks protecting/ attacking pawns
  eval += bonus;

  pi1 = Stack[gameDepth].pawnWOnFiles[r2 % 10];
  pi2 = Stack[gameDepth].pawnBOnFiles[r2 % 10];

  bonus = 0;
  if (r2 > 0 && nPieces < 16 && (pi1 + pi2) == 1)
    bonus += 10; // bonus for rooks protecting/ attacking pawns
  eval += bonus;

  bonus = 0;
  r1 = (int)Stack[gameDepth].pieceAPosition[BR][0];
  r2 = (int)Stack[gameDepth].pieceAPosition[BR][1];

  if (r1 > 0 && ((r1 % 10) == (king1 % 10)))
    eval -= 5; // tropism rook to advers king
  if (r2 > 0 && ((r2 % 10) == (king1 % 10)))
    eval -= 5;

  if (r1 > 0)
    eval -= (7 - DistKing[r1][king1]) * 1; // rook tropism to advers king
  if (r2 > 0)
    eval -= (7 - DistKing[r2][king1]) * 1;

  bonus = 0;
  if (r1 != 0 && ((r1 % 10) == (r2 % 10)) && (pi1 + pi2) <= 1 && (r1 % 10) > 2
      && (r1 % 10) < 7)
    {
      bonus = 20 - (pi1 + pi2) * 10; // bonus for rooks on the same column
    }
  else if (r1 > 0 && nPieces < 16 && (pi1 + pi2) == 1)
    bonus = 10; // bonus for rooks protecting/ attacking pawns

  if (r2 > 0 && nPieces < 16 && (pi1 + pi2) == 1)
    bonus += 10; // bonus for rooks protecting/ attacking pawns

  eval -= bonus;

  int ee = 0; // king safety
  if (Stack[gameDepth].whitePower > 2000 && king2 > 80)
    {
      if (tabla[king2 - 9] > 6)
        ee -= 10;
      if (tabla[king2 - 10] > 6)
        ee -= 20;
      if (tabla[king2 - 10] == 0 && tabla[king2 - 20] > 6)
        ee -= 8;
      if (tabla[king2 - 11] > 6)
        ee -= 15;
      if (king2 == 96 && tabla[king1 + 98] == BR)
        ee += 20;
      if (king2 == 97 && tabla[king1 + 98] == BR)
        ee += 20;
      ee += tabla_white_power_attacks[king2 - 9]
            + tabla_white_power_attacks[king2 - 10]
            + tabla_white_power_attacks[king2 - 20]
            + tabla_white_power_attacks[king2 - 11];
      // ee = ee + ee;
    }
  if (Stack[gameDepth].blackPower > 2000 && king1 < 39)
    {
      if (tabla[king1 + 9] > 0 && tabla[king1 + 9] < 7)
        ee += 15;
      if (tabla[king1 + 10] > 0 && tabla[king1 + 10] < 7)
        ee += 20;
      if (tabla[king1 + 10] == 0 && tabla[king1 + 20] > 0
          && tabla[king1 + 20] < 7)
        ee += 8;
      if (tabla[king1 + 11] > 0 && tabla[king1 + 11] < 7)
        ee += 10;
      if (king1 == 26 && tabla[king1 + 28] == WR)
        ee -= 20;
      if (king1 == 27 && tabla[king1 + 28] == WR)
        ee -= 20;
      ee -= tabla_black_power_attacks[king1 + 9]
            + tabla_black_power_attacks[king1 + 10]
            + tabla_black_power_attacks[king1 + 20]
            + tabla_black_power_attacks[king1 + 11];
      // ee = ee + ee;
    }

  // pawn structures
  int ee2;

  int promo;
  int promoWPawn[12] = { 0, 0, 0, 15, 30, 50, 70, 90, 100, 0, 0 };
  int promoBPawn[12] = { 0, 0, -90, -70, -50, -30, -15, 0, 0, 0, 0 };

  // int ppx =  (wpieces - bpieces) * 2;
  for (int k = 0; k < Stack[gameDepth].pieceCounts[WP]; k++)
    {
      ee2 = 0;

      int p1 = Stack[gameDepth].pieceAPosition[WP][k];

      promo = promoWPawn[SQRANK[p1]] / 2 + Options.level;

      if (tabla[p1 + 9] == WP)
        ee2 += 10;
      if (tabla[p1 + 11] == WP)
        ee2 += 10;
      if (tabla[p1 + 1] == WP)
        ee2 += 5;
      if (tabla[p1 - 1] == WP)
        ee2 += 5;
      if (tabla[p1 + 10] == WP)
        promo = promo / 2; // doubled white
      if (tabla[p1 + 10] == BP)
        promo = promo / 2; // bloked frontal pawn
      // if(tabla[p1+9] == BP) ee3 -= 10; // bloked lateral pawn
      // if(tabla[p1+11] == BP) ee3 -= 10; // bloked lateral pawn
      if (ee2 > 0 && p1 > 50)
        ee2 += 20;
      if (ee2 == 0)
        ee2 -= 10; // isolated white pawn
      // if(ee3 == 0) ee3 += 10; // not attaked white pawn
      if (nPieces <= 16)
        ee2 += (promo) / 4;
      if (nPieces <= 20)
        ee2 += (promo) / 8;
      if (nPieces <= 24)
        ee2 += (promo) / 16;
      ee += ee2;
    }

  for (int k = 0; k < Stack[gameDepth].pieceCounts[BP]; k++)
    {
      ee2 = 0;

      int p1 = Stack[gameDepth].pieceAPosition[BP][k];

      promo = promoBPawn[SQRANK[p1]] / 2 - Options.level;

      if (tabla[p1 + 1] == BP)
        ee2 -= 5;
      if (tabla[p1 - 1] == BP)
        ee2 -= 5;
      if (tabla[p1 - 9] == BP)
        ee2 -= 10;
      if (tabla[p1 - 11] == BP)
        ee2 -= 10;
      if (tabla[p1 - 10] == BP)
        promo = promo / 2; // doubled white
      if (tabla[p1 - 10] == WP)
        promo = promo / 2; // bloked frontal pawn

      if (ee2 < 0 && p1 < 50)
        ee2 -= 20; // advanced supported pawn
      if (ee2 == 0)
        ee2 += 10; // isolated black pawn

      if (nPieces <= 16)
        ee2 += (promo) / 4;
      if (nPieces <= 20)
        ee2 += (promo) / 8;
      if (nPieces <= 24)
        ee2 += (promo) / 16;
      ee += ee2;
    }
  eval += ee;

  // White Knights
  // Knigths moves evaluation
  int tab1[13] = { 0, 1, 5, 3, 3, 5, 0, 2, 8, 4, 4, 8, 20 };
  int tab2[13] = { 0, 2, 8, 4, 4, 8, 20, 1, 5, 3, 3, 5, 0 };

  for (int k = 0; k < Stack[gameDepth].pieceCounts[WN]; k++)
    {
      k1 = Stack[gameDepth].pieceAPosition[WN][k];
      if (tabla[k1 - 21] >= 0)
        eval += tab1[tabla[k1 - 21]];
      if (tabla[k1 - 19] >= 0)
        eval += tab1[tabla[k1 - 19]];
      if (tabla[k1 - 12] >= 0)
        eval += tab1[tabla[k1 - 12]];
      if (tabla[k1 - 8] >= 0)
        eval += tab1[tabla[k1 - 8]];

      if (tabla[k1 + 8] >= 0)
        eval += tab1[tabla[k1 + 8]];
      if (tabla[k1 + 12] >= 0)
        eval += tab1[tabla[k1 + 12]];
      if (tabla[k1 + 19] >= 0)
        eval += tab1[tabla[k1 + 19]];
      if (tabla[k1 + 21] >= 0)
        eval += tab1[tabla[k1 + 21]];

      eval += (7 - DistKing[k1][king2]) * 2;
    } // if

  // Black Knights

  for (int k = 0; k < Stack[gameDepth].pieceCounts[BN]; k++)
    {
      k1 = Stack[gameDepth].pieceAPosition[BN][k];

      if (tabla[k1 - 21] >= 0)
        eval -= tab2[tabla[k1 - 21]];
      if (tabla[k1 - 19] >= 0)
        eval -= tab2[tabla[k1 - 19]];
      if (tabla[k1 - 12] >= 0)
        eval -= tab2[tabla[k1 - 12]];
      if (tabla[k1 - 8] >= 0)
        eval -= tab2[tabla[k1 - 8]];

      if (tabla[k1 + 8] >= 0)
        eval -= tab2[tabla[k1 + 8]];
      if (tabla[k1 + 12] >= 0)
        eval -= tab2[tabla[k1 + 12]];
      if (tabla[k1 + 19] >= 0)
        eval -= tab2[tabla[k1 + 19]];
      if (tabla[k1 + 21] >= 0)
        eval -= tab2[tabla[k1 + 21]];

      eval -= (7 - DistKing[k1][king1]) * 2;
    } // if
  if (Options.level == 0)
    return rand ();

  return eval + score_must + cmr_penalty;
}
