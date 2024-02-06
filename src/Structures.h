/*
 * Copyright (c) 2024 Viorel Darie. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

#define WP (int)1
#define WR (int)2
#define WN (int)3
#define WB (int)4
#define WQ (int)5
#define WK (int)6
#define BP (int)7
#define BR (int)8
#define BN (int)9
#define BB (int)10
#define BQ (int)11
#define BK (int)12
#define MAXIDENT 10;
#define MINIMAX_RET (int)1
#define MATX (int)(-127)
#define MAT (int)20000

#define NEARXX (int)(-126)

#define PAT (int)0;
#define PATX (int)(-1);

#define ZERO (int)0;
// Structures

struct HashEntry
{
  unsigned int checksum;
  short eval;
  unsigned short bestmove;
  unsigned char age;
  signed char mate, depth, failtype;
};

struct StackElement
{
  int whiteKingSquare; // current position of white king position
  int blackKingSquare; // current position of black king position
  int RocadeIndicators; // Rocade flags

  int depthReduced; // must nor reduce depth in the same branch
  int ponder; // cumul of pieces ponder in stack memory
  int whitePower; // white pieces power sum
  int blackPower; // black pieces power sum
  char WCastle; //  flag if white rocade made
  char BCastle; //  flag if black rocade made
  char nPieces; // total number of pieces on board
  char mateDetected; // if == 1 mate is detected and valable till the end of
                     // the game
  char suplimDepth; // supplimentary depth caused by taken pieces

  char nWP; // count of white pieces without WK and WP
  char nBP; // count of black pieces without BK and BP
  char nBMinors; // number of black minors on table
  char nWMinors; //  -//-  white minors
  char powerWOnFiles[13]; // how many white pawns on each line
  char powerBOnFiles[13]; // how many black pawns on each line

  char pawnWOnFiles[13]; // how many white pawns on each line
  char pawnBOnFiles[13]; // how many black pawns on each line
  int pieceCounts[13]; // counts for each type of pieces
  int pieceAPosition[13][13]; // a square where there is a piece of that type
  int positionVal; // sum of position values of each piece on the board
  int PawnHash; // Current Pawn Hash initiated at each search and updated on
                // PlayMove
  int HashKey; // Hash key - utilizat la repetitii
  int HashCheck; // Hash Check
};

struct StackElement2
{
  char SqFrom;
  char SqTo;
  char sortedlevel;
  char PieceMoved;
  char PieceTaken;
  char Indicators;
  char IsCheck;
  char info[20];
  int bestmove; // best move with depth - 2
  char IsCheckOwn; // if != 0 the enemy king can be taken
  int BestWay[50]; // pointer to zone to store bestmoves line
  int BestMove; // pointer to zone to store bestmoves line
  int MaxOnLevel; // MiniMax on this level
  int nbAttacks; // number of attack moves (and rocades, transform etc)
  int Attacks[100]; // all moves al a level
  int ponders[100]; // list of ponders corecponding to Attacks list, indexed by
                    // rankOnLevel

  int flagsOfGame; // some flags of the game
  int reps; // repetitions;
  int BestLineDepth; // the depth of the generated best line vector
  int MaxDepth; // the max value for analized for move in tree
  int EffectiveAnalysed; // how many moves where analysed on level 0
  int rankOnLevel; // how moves were analyzed on this level
  int rankAttacks; // explored moves from attacks

  int movesOnLevel; // nbAttacks + nbNoAttacks
  int MoveValue;

  char enPassant; // 0 or pawn move 2 steps
  char eChecks; // how many moves are traited whem in check
  int recap; // ind of recapture
  int killers[2];
  int killers_count[2];
  int move;
  int sumranks;
  int moveBestLine;
};

struct Opt
{
  int nLevel;
  int level;
  int agresivity;
  int nTime;
  int mode;
  int thinking;
  int taken;
  int nModel;
  int sound;
  int blacks;
};

struct Hist
{
  int whiteHistory3[13 * 120]; // valori complete nivele mici
  int blackHistory3[13 * 120];

  int whiteMateHistory[13 * 120]; // mutari care duc la mat
  int blackMateHistory[13 * 120];

  int numarInRamura[13 * 120];
  int sumaPonderiInRamura[13 * 120];

  int whiteHistory[13 * 120]; // with dept * depth + 1
  int blackHistory[13 * 120];
};
struct OpeningText
{
  char cmove[70]; // in algebric notation
};

struct OpeningBin
{
  int moveNumber;
  int OpeningMove;
  char comment[60];
};
