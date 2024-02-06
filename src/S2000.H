/*
 * Copyright (c) 2024 Viorel Darie. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

//  s2000 definiri de date

#define WHITE 0
#define BLACK 1
#define MAXDEPTH 32
#define NOTIME -999999

#define MAX_MOVES 700 // half moves
#define MAX_LEN 50000

#define NO_BEST 65535
#define NOVALUE 65535
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
#define FIRSTBRANCH 31006;
#define MAXIDENT 10;
#define EXTRANK 2

int max_checks_depth = 0;
int max_attacks = 0;

int
MateWithKnightBishop ();

int tabla_white_attackers[120];
int tabla_black_attackers[120];

int tabla_white_power_attacks[120];
int tabla_black_power_attacks[120];

int agresivity = 0;

int indFileOpen = 0;

int forcedreturns = 0;

int MiniMax = 0;

int MaxAhead = 28;

int repeatvalue = 0;

int g_LastBest = 0;

int g_lasteval = 0;
// int upgrade = WQ ;// pawn promotion
int
MoveToStringPNG (int, char[]);
char ListPieces[] = "0PTCNDRPTCNDR";
int nBestmoveFrom = NO_BEST;
int nBestmoveTo = 0;
// int zizi[10000];
int g_ucAge;
int* pm;

int tt_gasit, tt_negasit;
int* pma;
int *pm0, *pma0;

int WhiteTakeLikePawn[14] = { 0, 0, 10, 0, 0, 5, 2, 1, 0, 0, 0, 0, 0, 0 };
int BlackTakeLikePawn[14] = { 0, 0, 0, 0, 0, 0, 0, 0, -10, 0, 0, -5, -2, -1 };
int WhiteTakeLikeRook[14] = { 0, 0, 0, 5, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0 };
int BlackTakeLikeRook[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, -10, 0, 0, -2, -1 };
int WhiteTakeLikeRook2[14] = { 0, 0, 0, 5, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 };
int BlackTakeLikeRook2[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, -10, 0, 0, -2, 0 };

int ValPiesaReal[]
    = { 0, 100, 480, 310, 330, 970, 0, -100, -480, -310, -330, -970, 0 };
// int ValPiesaReal[] = {0,100,510,315,332,960,0,-100,-510,-315,-332,-960,0};

int ValPiesa[]
    = { 0, 100, 480, 310, 330, 970, 0, -100, -480, -310, -330, -970, 0 };
int ValPiesaMat[] = { 0, 10, 5, 1, 2, 12, 0, -10, -5, -1, -2, -12, 0 };
int ValTakenPri[] = { 0, 0, 5, 3, 3, 10, 0, 0, 5, 3, 3, 10, 0 };

int ValPenaltyNearB[13] = { 0, 120, 0, 0, 80, 120, 120, 0, 0, 0, 0, 0, 0 };
int ValPenaltyNearW[13] = { 0, 0, 0, 0, 0, 0, 0, -120, 0, 0, -80, -120, -120 };

const int KNIGHTSV = 2 * ValPiesa[WN];

extern unsigned int WPHashSq[128];
extern unsigned int BPHashSq[128];

int RocadaFlagsMayChange[120];

int Moves[200];
int MovesA[200];
int nMoves;
int nMovesA;
int lastOptimalMove;

// int PieceMoved;
// int PieceTaken;
// int SqFrom;
// int SqTo;

int valueRetrived;

int pruneType;

int LineWhite[8];
int ColumnWhite[8];
int LineBlack[8];
int ColumnBlack[8];

int TabCheck[13][120];
int TabCheck2[13][120];

int TabPositionValues[13][128];

int TabRookKing[13] = { 0, 0, 2, 0, 0, 0, 6, 0, 8, 0, 0, 0, 12 };

int SuplimentaryDepth[13] = { 0, 0, 2, 1, 1, 2, 0, 0, 2, 1, 1, 2, 0 };
int flagCenter[13]
    = { 0, 1, 0, 1, 1, 0, 0, -1, 0, -1, -1, 0, 0 }; // allow moving to center

int Alpha, Beta; // AlphaBeta limits

int buttonPressed = 0;

int rep[25000]; // statistics for checksum use

struct Hist History;

// excenge
int pawnhash;
int nopawnhash;

// int wHits[100];
// int bHits[100];

// int nWHits;
// int nBHits;

// int nbWHits,nbBHits;
int xa[120];
int taend[120];
int sq;
int thinking;

int inline AlphaBetaAlgorithm (int who, int ahead, int depth, int alpha,
                               int beta, int& bestmove, int indNull);
int
MatingEval (const int& nWKingSq, const int& nBKingSq, int eval);

int inline WhiteTestBad (int SqTo, int PieceMoved, int PieceTaken);
int inline BlackTestBad (int SqTo, int PieceMoved, int PieceTaken);

int
TakeOneMove2 (int, int, int, int);
void
SaveGame (HWND hwnd);
int
testxx ();
int inline UpdateNormal (int SqFrom, int SqTo, int PieceMoved, int PieceTaken);
int
SetTabPositionValues ();
int
SetPiecesPositions (char Indicators, int Move);
int
RebuildPiecesPositions ();

int inline RebuildPiecesPositions2 ();

int inline AddPiece (int SqTo, int OldPiece, int NewPiece);

int
SetTabCheck ();
void
SetHashMB (unsigned int nMegs);
void
Clear_Hash (void);
void
Create_HashFunction ();
unsigned int
Hash_Board2 ();
unsigned int
Hash_Board ();
unsigned int
Pawn_HashBoard (); // In Evaluate.cpp
void
GetPVString (char* buffer, int format);
int
CheckGameOver (int bPopUps, int& GameResult);
int
IsMate (char Color);
int
FindMateThreat (char Color, int ahead);
void
SetHashSizeBits (int nBits);
void
InitSearchGlobals ();
void
SetThinkTime (float nCompTm, int nMoves, int nSecs);
// void UpdateDisplayInfo ( int i, int MMove, int Max, int bestmove );
void
ExtendThinkTime ();
int
FindMateThreat (int Color);
int inline CheckLine (int Sq0, int dstSq, int nDirec, int pie1, int pie2);
int inline DiscoveredCheck (int, int);
int inline DiscoverCheckW (int Sq, int KingSq, int nDirec);
int inline CheckingMove (int color, int Move);
int inline IsCheck (int who);
int inline CheckDirec (int Sq0, int nDirec, int piece1, int piece2);
int inline SQ (int x, int y);
void inline FindLegalMovesBlackKing (int x);
void inline FindLegalMovesWhiteKing (int x);
void inline FindHeavyMovesWhite (int color);
void inline FindHeavyMovesBlack (int color);
void
Clear_PawnHash ();
unsigned int
Pawn_HashBoard ();
void
InitGenTree ();
int
TouchMatesB (int depth);
int
TouchMatesW (int depth);

// int inline QueenThreat (const int Piece, const int nDst, const int nSide );

void inline AddMoveW (int src, int nDstSq, int bAttack, int indi);
void inline AddMoveB (int src, int nDstSq, int bAttack, int indi);

void inline InCheckMoveGenW ();

void inline InCheckMoveGenB ();

void
ReadGamePGN ();
void
ReadGamePGN ();

int
ClearHistory ();
int inline SetHistory (const int Move, const int& depth, const int& ahead,
                       const char& who, const int& value, int flag);

int
SetStructTree ();
int
StructPosition ();

int inline GenMovesForAWhitePiece (int indExchange, int PieceMoved,
                                   int SqFrom);
int inline GenMovesForABlackPiece (int indExchange, int PieceMoved,
                                   int SqFrom);
// int GenExchangeMovesForAWhitePiece(int PieceMoved,int SqFrom);
// int GenExchangeMovesForABlackPiece(int PieceMoved,int SqFrom);

int inline TestRepeat ();

int inline IsAttacked (int square, int color);

void inline ComputeExtensions (int& depth, int& ahead, int& Ext,
                               int& nextDepth, int& nDst);

int
MakePieceList ();

int inline AddWhiteAttacks (int sq);
int inline AddBlackAttacks (int sq);

int
SaveToRepository ();
int
RestoreFromRepository ();
int
SaveMoveToRepository (int nextdepth, int value, int cksum);

char moveMessage[60];
char openingMessage[60];

int coliziunick = 0;

int tablaRoot[120];

int nrDeposit; // nb of best lines saved in deposit
int maxIdepth;
int statRang[100];

int ta[120]; // working table

int ranksort[200];

int rankattacks[200];

int ponders[100];

int nran1;
int nran2;
int ran1[100];
int ran2[100];

int gTotal = 0;

int savedLevel;
int savedKmax;
int randMove;
int repeatPosition;
int repeatPosition2;

int CurrentBest;
int CurrentEval;

int sortbestmove;

int recap[800];
int suplimDepth[800];
int eChecks[800];
int wasRecap[800];

int lastTreeDepth;

long tmov, talfa, tbest;

time_t rawtime0;
time_t rawtime1;
char stime[20];

int MaxDepth;

int tt_apel;
int tt_reusit;

int MaxEndPieces = 10;
int PenaltyTake[13] = {
  0, 0, 20, 10, 10, 30, 40, 0, -20, -10, -10, -30, -40
}; // take pieces first with low value pieces

// setul Viorel
int dk[9]
    = { 1, -1, 10, -10, 9, -9, 11, -11, 0 }; // kings relative deplacements
int dr[5] = { 1, -1, 10, -10, 0 }; // rooks relative increments
int db[5] = { 9, -9, 11, -11, 0 }; // bishop relative increments
int dn[9]
    = { -21, -19, -12, -8, 8, 12, 19, 21, 0 }; // knights relative deplacements

int dk2[9] = { -11, -10, -9, -1, 1, 9, 10, 11, 0 };
int dq[9] = { 1, -1, 9, -9, -10, 10, -11, 11, 0 };

int whiteTake[15] = { 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 };
int blackTake[15] = { 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 };

// Algorith == 1
int saveTreeDepth;

int CurrentMove;

int saveKsup;
int limDepth;
int endOfBranch;
int iDepth;

//
int noAlphaBeta; // flag to say if alpha beta to work or not; work when == 1
char moveout[10];
int ksup, kmax;

long generatedTotal;
long generatedExchange;

long nodes;
long nodes1;
long nodes2;

long nodesbest;
long nodeskill;

long razoring;
long nodes3 = 0;
long nodes4 = 0;
long nodes5 = 0;
long width = 0;
long nodes10 = 0;

long generatedTotalTotal;
long generatedExchangeTotal;
long consultingEnd;

long generatedIsCheck;
long generated10Pri;
long generatedTaken;

long savedRepository = 0;

int alfafull = 0;
int alfanarrow = 0;
int apelExchange = 0;
int apelNull = 0;

long identicalcks = 0;

long generatedMoves[500];
int piece1;
int piece2;
long checks[500];
long mates[500];
long takes[500];

long quiets[500];

long nrAlphaBeta[500];
long nrAlphaBetaMiniTree[500];
long expandedNodes[500];
int noTime;
int endOfGame;

int horizontStrategyCumul;
int horizontNMoves;

int aaMat;

int foundMoveRepository = 0;
int foundMoveRepository2 = 0;
int savedMoveRepository = 0;
int StoreMoveRepository = 0;

int agresivityMode;

int
ClearPlace ();

int
ClearRepository ();
int
ClearMoveRepository ();

int
CheckEvade (int color, int& alpha, int& beta, int& ahead, int& depth);

int
SaveToRepository ();
int
RestoreBestWay ();

// int SaveMoveToRepository();
int inline RetriveMove (int alpha, int beta, int ahead, int nextDepth,
                        int foundMove);

int
CompStructBase ();
int
SetIsCheck (int color);
int
GemePlaying ();
int
GetWhiteMove ();
int
GetBlackMove ();
int
GetMoveString (int& Move);
int
TestButtons ();
int BestVal;

int
ExtendedConsultEndGame ();

int
MoveIsCheck ();
void
FindMovesBlack (int indExchange);
void
FindMovesWhite (int indExchange);
void inline AddAtkMove (int dst, int MoveFrom, int MoveTo);
int
AddUnderPromos ();
int
SortAttacks ();
// int SortMoveList (int bestmove, int killermove);

int
Exchange (int who1, int alpha, int beta, int ahead, int depth, int mod);
int inline GenerateAndSortMoves (const int color, const int ahead,
                                 const int alpha, const int beta);

int
MateRepository ();
int
GetOptimalSchema ();

int
PreprocessOpeningFile ();
int
TakeWord (char[], char[], int);
int
TakeOpeningMove ();

int
SaveGame ();

int
MoveBackTa ();
int
PlayMoveTa ();

int
GenLevelMoves (int indExchange);
int
GetOptimalMove ();
// int CopyMove(struct MoveLong *, struct MoveLong *);

int
QueenAttacked ();

int
MoveBack ();

int RookNotMoved1;
int RookNotMoved2;
int RookNotMoved3;
int RookNotMoved4;
int KingNotMoved1;
int KingNotMoved2;

int score_must;
int cmr_penalty;
int
InitChessBoard ();

struct StackElement Stack[500]; // Board Stack for all moves from game begining
struct StackElement2
    Stack2[500]; // Info Stack for all moves from game begining

int x_centru; // centrul de greutate al distributiei pieselor
int y_centru; // centrul de greutate al distributiei pieselor

int gameDepth; // the depth from begining of game
int treeDepth; // the depth from the current tree

int OptimalMove;
int OptimalMoveMoveValue;
int OptimalMoveIsCheck;
int MoveToDo;
int MoveToDoIsCheck;

int MoveToDoTa;

struct planEndsMates
{
  int indexMasiv;
  char comentEnd[20];
  int pieseComposition[4];
  char indexAdvers;
}
	planEnds[] = { { 1,"WK WR BK --", WK, BK, WR, 0, 0},\
          { 2,"WK WQ BK --", WK, BK, WQ, 0, 0},\
	      { 3,"WK WP BK --", WK, BK, WP, 0, 0},\
          { 4,"WK WQ BK BN", WK, BK, WQ,BN, 0},\
          { 5,"WK WQ BK BR", WK, BK, WQ,BR, 0},\
          { 6,"WK WQ BK BB", WK, BK, WQ,BB, 0},\

	      { 7,"WK WQ BK BQ", WK, BK, WQ,BQ, 0},\
          { 8,"WK WR BK BQ", WK, BK, WR,BQ, 0},\
          { 9,"WK WR BK BR", WK, BK, WR,BR, 0},\
          {10,"WK WR BK BB", WK, BK, WR,BB, 0},\
	      {11,"WK WR BK BN", WK, BK, WR,BN, 0},\

          {12,"WK WQ WQ BK", WK, BK, WQ,WQ, 0},\
          {13,"WK WQ WR BK", WK, BK, WQ,WR, 0},\
          {14,"WK WQ WB BK", WK, BK, WQ,WB, 0},\
	      {15,"WK WQ WN BK", WK, BK, WQ,WN, 0},\

          {16,"WK WR WR BK", WK, BK, WR,WR, 0},\
          {17,"WK WR WB BK", WK, BK, WR,WB, 0},\
	      {18,"WK WR WN BK", WK, BK, WR,WN, 0},\

	      {19,"WK WQ BK BP", WK, BK, WQ,BP, 0},\
	      {20,"WK WR BK BP", WK, BK, WR,BP, 0},\

          {21,"WK WP BK BQ", WK, BK, WP,BQ, 0},\
          {22,"WK WP BK BR", WK, BK, WP,BR, 0},\
          {23,"WK WP BK BB", WK, BK, WP,BB, 0},\
	      {24,"WK WP BK BN", WK, BK, WP,BN, 0},\
	      {25,"WK WP BK BP", WK, BK, WP,BP, 0},\

          {26,"WK WQ WP BK", WK, BK, WQ,WP, 0},\
          {27,"WK WR WP BK", WK, BK, WR,WP, 0},\

          {28,"WK WP WB BK", WK, BK, WP,WB, 0},\
	      {29,"WK WP WN BK", WK, BK, WP,WN, 0},\
	      {30,"WK WP WP BK", WK, BK, WP,WP, 0},\

		  {99," ",0,0,0,0,0} };

int
SetStruct ();
int
SetKingKing ();
int
SetDirKing2 ();

int
SetQueenKing ();
int
SetRookKing ();
int
SetBishopKing ();
int
SetKnightKing ();

int
AccessMat4 (int mode, int zpoz1, int zpoz2, int zpoz3, int zpoz4);
int
ConsultEndGame ();

long tlev;

int bestMiniMax;
int nrMiniMaxBest;
int nrMovesNoTakeSave;

int BestLine[100];
int BestLine_pv[100];
int level;
int deltaxx;
int algorithm;

int limitSuplimDepth; // suplimentary depth caused by pieces excenge

int pond;
int npiese;

int lwhite[130];
int lblack[130];
int nwhite, nblack;

int limHigt;

int piesa, piesa2, loc1, loc2;
int i, j, k;

int who, tip, k1, k2, flags;

int tabla[120];

unsigned int tabla_rand[600];

int KingCenter[128] = {
  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 0,  2,  5,  5,  5,  5,  2,  0,  PD, PD, 2,  13, 16,
  16, 16, 16, 13, 0,  PD, PD, 2,  16, 33, 33, 33, 33, 16, 2,  PD, PD,
  2,  16, 33, 43, 43, 33, 16, 2,  PD, PD, 2,  16, 33, 43, 43, 33, 16,
  2,  PD, PD, 2,  16, 33, 33, 33, 33, 16, 2,  PD, PD, 2,  13, 16, 16,
  16, 16, 13, 2,  PD, PD, 0,  2,  5,  5,  5,  5,  2,  0,  PD,
};

int tso[13] = { 0, 1, 5, 3, 4, 6, 8, -8, -4, -7, -6, -3, -2 };
int npieces;
int ltip[64], lpoz[64], lval[64];

int
GenTree ();

int tabla0[120]
    = { -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, 2,  3,  4,  5,  6,  4,  3,  2,  -1, -1, 1,  1,  1,  1,  1,
        1,  1,  1,  -1, -1, 0,  0,  0,  0,  0,  0,  0,  0,  -1, -1, 0,  0,  0,
        0,  0,  0,  0,  0,  -1, -1, 0,  0,  0,  0,  0,  0,  0,  0,  -1, -1, 0,
        0,  0,  0,  0,  0,  0,  0,  -1, -1, 7,  7,  7,  7,  7,  7,  7,  7,  -1,
        -1, 8,  9,  10, 11, 12, 10, 9,  8,  -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2 };

int lgBestWay;
int lgMax;
int lgMaxKmax;

int
PutMoveString (int sq1, int sq2, int p1, int p2, int indicators);

int WhiteToMove = 1;
int BlackToMove = 0;

int WhiteIsMan = 1;
int BlackIsMan = 0;

int
GetWhiteMove ();
int
GetBlackMove ();

int
MoveBack ();

int
GamePlaying ();

int
GetOptimalMove ();

int
AllowedMove (int, int& NewMove);

int MoveNumber = 1;

int sq1;
int sq2;
int p1;
int p2;

int piecesCount[40][40];

int
SaveBestWay (int Move);

int
TablaWhiteIsCheck (int loc);
int
TablaBlackIsCheck (int loc);
int
CopyMove (struct MoveLong*, struct MoveLong*);

int
CompMatBB (int, int, char*, char*, char*);
int
GetMatBB (int, int, int, int, int*);

int
GetMatBB (int, int, int, int, int*);
int
GetMat4 (int, int, int, int, int*);
int
SetMat4 (int, int, int, int, int);

int
GenQuik (int, int, int, int);
int
TestMat (int, int);
int
GenTree ();
int
CompMat ();
int
CompMatPawn ();
int
ClearPlace ();
int
IniPond (void);
int
SetStruct (void);
int
GenLevelMoves ();
int
Replay (void);
int
Play (void);
int
TakeOneMove (int alpha, int beta, int ahead, int depth, int foundValidMove);

int
SetPiecesChaine (void);
int
SetStruct ();
int
SetPiecesChain ();
int
SetKingKing ();
int
SetQueenKing ();
int
SetRookKing ();
int
SetBishopKing ();
int
SetKnightKing ();

int
SetMatP (int, int, int, int);
int
GetMatP (int, int, int, int*);
int
SetMatQ (int, int, int, int);
int
GetMatQ (int, int, int, int*);
int
SetMatR (int, int, int, int);
int
GetMatR (int, int, int, int*);

int
CompMatP ();
int
CompMatQ ();
int
CompMatR ();

int
SaveGame ();

// SETSTRUCT

int PownPoz[] = { 0, 0, 10, 20, 30, 60, 100, 100 };

int
GenWhiteHits ();
int
GenBlackHits ();

int
GenWhiteAreal ();
int
GenBlackAreal ();

int materialPonder;
int matPower[13] = {
  0, 500, 700, 0, 1, 1000, 0, 500, 700, 0, 1, 1000, 0
}; // ponders with pieces capables to mate

int valAttack[13] = {
  0, -2, -10, -6, -6, -20, -20, 2, 10, 6, 6, 20, 20
}; // bonuses for attacks
int valProtect[13]
    = { 0, 5, 3, 2, 3, 1, 0, -5, -3, -2, -3, -1, 0 }; // bonuces for defence

int totalMatePower;

char DirKing[120][120];
char KingKing[120][120];
char QueenKing[120][120];
char DistKing[120][120];

char RookKing[120][120];
char BishopKing[120][120];
char KnightKing[120][120];

// struct Move wAttacks[100];
//  struct Move bAttacks[100];
// int nrWAttacks;
//  int nrBAttacks;

// TESTSAH

int
WhiteIsCheck (int loc);
int
BlackIsCheck (int loc);

// GENTREE VARIABLES

int vv;

int
InitGame ();

int
EvaluateBoard (int alpha, int beta);
int
HeadLevelLogics ();
int
TreeMiniMax ();
int
SomeTests ();
int
SaveToRepository ();

int
TreeMiniMax ();
int
AlphaBetaTest ();

int
GenTree ();

int
RestoreBestWay ();
int StoreRepository;

int totalKiller = 0;
int foundRepository = 0;
int foundRepository2 = 0;

int
MoveBack ();
int
TakeOneMove ();

int anotherMoveOnLevel;
int listNotGeneratedOnLevel;

int nbOpeningMoves;
struct OpeningBin OpeningBinary[10000];
struct StackElement* pstack;
struct StackElement* pstackxx;

struct StackElement2* pstack2;
struct StackElement2* pstackxx2;

//
/*
// knight
-50,-40,-30,-30,-30,-30,-40,-50,
-40,-20,  0,  0,  0,  0,-20,-40,
-30,  0, 10, 15, 15, 10,  0,-30,
-30,  5, 15, 20, 20, 15,  5,-30,
-30,  0, 15, 20, 20, 15,  0,-30,
-30,  5, 10, 15, 15, 10,  5,-30,
-40,-20,  0,  5,  5,  0,-20,-40,
-50,-40,-30,-30,-30,-30,-40,-50,
*/
int WKnightPost[120] = {
  PD, PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,
  PD, PD,  PD,  PD,  PD,  PD,  -50, -40, -30, -30, -30, -30, -40, -50, PD,
  PD, -40, -20, 0,   5,   5,   0,   -20, -40, PD,  PD,  -30, 5,   10,  15,
  15, 10,  5,   -30, PD,  PD,  -30, 0,   15,  20,  20,  15,  0,   -30, PD,
  PD, -30, 5,   15,  20,  20,  15,  5,   -30, PD,  PD,  -30, 0,   10,  15,
  15, 10,  0,   -30, PD,  PD,  -40, -20, 0,   5,   5,   0,   -20, -40, PD,
  PD, -50, -40, -30, -30, -30, -30, -40, -50, PD,
};
/*
int WKnightPost[ 120 ] =
{
        PD, PD, PD, PD, PD, PD, PD, PD, PD,  PD,
        PD, PD, PD, PD, PD, PD, PD, PD, PD,  PD,
        PD,  0,  0,   0,   0,   0,   0,  0,  0,  PD,
        PD,   5,  6,  9,   9,   9,   9,  6,  5,  PD,
        PD,   6,  8, 10,  12,  12,  10,  8,  6,  PD,
        PD,   3,  6,  7,   8,   8,   7,  6,  3,  PD,
        PD,   2,  3,  4,   4,   4,   4,  3,  2,  PD,
        PD,   0,  2,  2,   2,   2,   2,  2,  2,  PD,
        PD,   0,  0,  0,   0,   0,   0,  0,  0,  PD,
        PD,   0,  0,  0,   0,   0,   0,  0,  0,  PD
};
*/
int BKnightPost[120] = {
  PD, PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,
  PD, PD,  PD,  PD,  PD,  PD,  -50, -40, -30, -30, -30, -30, -40, -50, PD,
  PD, -40, -20, 0,   5,   5,   0,   -20, -40, PD,  PD,  -30, 5,   10,  15,
  15, 10,  5,   -30, PD,  PD,  -30, 0,   15,  20,  20,  15,  0,   -30, PD,
  PD, -30, 5,   15,  20,  20,  15,  5,   -30, PD,  PD,  -30, 0,   10,  15,
  15, 10,  0,   -30, PD,  PD,  -40, -20, 0,   5,   5,   0,   -20, -40, PD,
  PD, -50, -40, -30, -30, -30, -30, -40, -50, PD,
};
/*
int BKnightPost[ 120 ] =
{
        PD, PD, PD, PD, PD, PD, PD, PD, PD,  PD,
        PD, PD, PD, PD, PD, PD, PD, PD, PD,  PD,
        PD,  0,  0,   0,   0,   0,   0,  0,  0,  PD,
        PD,   0,  0,  0,   0,   0,   0,  0,  0,  PD,
        PD,   0,  2,  2,   2,   2,   2,  2,  2,  PD,
        PD,   2,  3,  4,   4,   4,   4,  3,  2,  PD,
        PD,   3,  6,  7,   8,   8,   7,  6,  3,  PD,
        PD,   6,  8, 10,  12,  12,  10,  8,  6,  PD,
        PD,   5,  6,  9,   9,   9,   9,  6,  5,  PD,
        PD,   0,  0,  0,   0,   0,   0,  0,  0,  PD,
};
*/

int WBishopValue[128] = {
  PD, PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,
  PD, PD,  PD,  PD,  PD,  PD,  -20, -10, -10, -10, -10, -10, -10, -20, PD,
  PD, -10, 5,   0,   0,   0,   0,   5,   -10, PD,  PD,  -10, 10,  10,  10,
  10, 10,  10,  -10, PD,  PD,  -10, 0,   10,  10,  10,  10,  0,   -10, PD,
  PD, -10, 5,   5,   10,  10,  5,   5,   -10, PD,  PD,  -10, 0,   5,   10,
  10, 5,   0,   -10, PD,  PD,  -10, 0,   0,   0,   0,   0,   0,   -10, PD,
  PD, -20, -10, -10, -10, -10, -10, -10, -20, PD,
};
/*
int WBishopValue[ 128 ] =
{
         PD, PD, PD, PD, PD, PD, PD, PD, PD,  PD,
         PD, PD, PD, PD, PD, PD, PD, PD, PD,  PD,
         PD,  -10,  - 5,  -5, -5, -5,  -5, - 5, -10,  PD,
         PD,  -12,    5,   0,  0,  0,   0,   5, -12,  PD,
         PD,   -5,    0,   7,  0,  0,   7,   0,  -5,  PD,
         PD,   -5,    0,   5,  8,  8,   5,   0,  -5,  PD,
         PD,   -5,    0,   5,  8,  8,   5,   0,  -5,  PD,
         PD,   -5,    0,   5,  0,  0,   5,   0,  -5,  PD,
         PD,  -10,    6,   0,  0,  0,   0,   6, -10,  PD,
         PD,  -11,  - 9, - 8, -8, -8, - 8, - 9, -11,  PD
};
*/
int WRookValue[120]
    = { PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
        PD, PD, PD, PD, 15, 15, 15, 15, 15, 15, 15, 15, PD, PD, 2,  8,  8,
        8,  8,  8,  8,  2,  PD, PD, 2,  5,  5,  6,  6,  5,  5,  2,  PD, PD,
        2,  5,  5,  6,  6,  5,  5,  2,  PD, PD, 2,  5,  5,  6,  6,  5,  5,
        2,  PD, PD, 2,  5,  5,  6,  6,  5,  5,  2,  PD, PD, 4,  8,  9,  9,
        9,  9,  8,  4,  PD, PD, 0,  0,  3,  3,  3,  3,  0,  0,  PD };

int WQueenValue[120] = {

  PD, PD, PD, PD, PD,  PD, PD, PD, PD, PD, PD, PD,  PD, PD, PD, PD, PD,
  PD, PD, PD, PD, -12, -5, 5,  5,  5,  5,  -5, -12, PD, PD, -8, -5, 5,
  5,  5,  5,  -5, -8,  PD, PD, -5, -5, 5,  12, 12,  5,  -5, -5, PD, PD,
  -5, -5, 5,  12, 12,  5,  -5, -5, PD, PD, -2, 0,   5,  12, 12, 5,  0,
  -2, PD, PD, -2, 3,   10, 10, 10, 10, 3,  -2, PD,  PD, 0,  5,  10, 10,
  10, 10, 5,  0,  PD,  PD, -2, 5,  8,  8,  8,  8,   5,  -2, PD
};

int BQueenValue[120] = {
  PD, PD, PD, PD, PD, PD, PD,  PD, PD, PD, PD, PD, PD, PD,  PD, PD, PD,
  PD, PD, PD, PD, -2, 5,  8,   8,  8,  8,  5,  -2, PD, PD,  0,  5,  10,
  10, 10, 10, 5,  0,  PD, PD,  -2, 3,  10, 10, 10, 10, 3,   -2, PD, PD,
  -2, 0,  5,  12, 12, 5,  0,   -2, PD, PD, -5, -5, 5,  12,  12, 5,  -5,
  -5, PD, PD, -5, -5, 5,  12,  12, 5,  -5, -5, PD, PD, -8,  -5, 5,  5,
  5,  5,  -5, -8, PD, PD, -12, -5, 5,  5,  5,  5,  -5, -12, PD,

};

int WPawnPos[120]
    = { 0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,   0,  0,  0,  0,   0,
        0,  0,  0,  0,

        0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,   5,  10, 10, -20, -20,
        10, 10, 5,  0,  0,  5,  -5, -10, 0,  0,  -10, -5, 5,  0,  0,   0,
        0,  0,  20, 20, 0,  0,  0,  0,   0,  5,  5,   10, 25, 25, 10,  5,
        5,  0,  0,  10, 10, 20, 30, 30,  20, 10, 10,  0,  0,  50, 50,  50,
        50, 50, 50, 59, 50, 0,  0,  0,   0,  0,  0,   0,  0,  0,  0,   0,

        0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,   0,  0,  0,  0,   0,
        0,  0,  0,  0 };
/*
int WPawnPos[ 120 ] =
{
        0,  0,  0,   0,   0,   0,   0,  0,  0,  0,
        0,  0,  0,   0,   0,   0,   0,  0,  0,  0,

        0,  0,  0,   0,   0,   0,   0,  0,  0,  0,
        0,  0,  0,  15,  15,  15,  25, 25, 25,  0,
        0,  2,  2,  15,  15,  15,  15, 10,  8,  0,
        0,  2,  2,  15,  32,  32,  15, -8, -8,  0,
        0,  2,  2,  15,  32,  32,  10, -8, -8,  0,
        0, 10, 10,  40,  45,  45,  30, 32, 32,  0,
        0, 40, 40,  40,  45,  45,  45, 45, 45,  0,
        0,  0,  0,   0,   0,   0,   0,  0,  0,  0,

        0,  0,  0,   0,   0,   0,   0,  0,  0,  0,
        0,  0,  0,   0,   0,   0,   0,  0,  0,  0
};
*/
/*
// pawn
 0,  0,  0,  0,  0,  0,  0,  0,
50, 50, 50, 50, 50, 50, 50, 50,
10, 10, 20, 30, 30, 20, 10, 10,
 5,  5, 10, 25, 25, 10,  5,  5,
 0,  0,  0, 20, 20,  0,  0,  0,
 5, -5,-10,  0,  0,-10, -5,  5,
 5, 10, 10,-20,-20, 10, 10,  5,
 0,  0,  0,  0,  0,  0,  0,  0
*/
int BPawnPos[120]
    = { 0,   0,   0,  0,  0,  0,   0,  0,  0,   0,  0,  0,  0,  0,  0,  0,
        0,   0,   0,  0,

        0,   0,   0,  0,  0,  0,   0,  0,  0,   0,  0,  50, 50, 50, 50, 50,
        50,  50,  50, 0,  0,  10,  10, 20, 30,  30, 20, 10, 10, 0,  0,  5,
        5,   10,  25, 25, 10, 5,   5,  0,  0,   0,  0,  9,  20, 20, 0,  0,
        0,   0,   0,  5,  -5, -10, 0,  0,  -10, -5, 5,  0,  0,  0,  10, 10,
        -20, -20, 10, 10, 5,  0,   0,  0,  0,   0,  0,  0,  0,  0,  0,  0,

        0,   0,   0,  0,  0,  0,   0,  0,  0,   0,  0,  0,  0,  0,  0,  0,
        0,   0,   0,  0 };
/*
int BPawnPos[ 120 ] =
{
        0,  0,  0,   0,   0,   0,   0,  0,  0,  0,
        0,  0,  0,   0,   0,   0,   0,  0,  0,  0,

        0,  0,  0,   0,   0,   0,   0,  0,  0,  0,
        0, 40, 40,  40,  45,  45,  45, 45, 45,  0,
        0, 10, 10,  40,  45,  45,  30, 32, 32,  0,
        0,  2,  2,  15,  32,  32,  10, -8, -8,  0,
        0,  2,  0,  15,  32,  32,  15,  0,  0,  0,
        0,  2,  0,  15,  25,  25,  25, 10, 25,  0,
        0,  0,  0,  15,  15,  15,  15, 15, 15,  0,
        0,  0,  0,   0,   0,   0,   0,  0,  0,  0,

        0,  0,  0,   0,   0,   0,   0,  0,  0,  0,
        0,  0,  0,   0,   0,   0,   0,  0,  0,  0
};
*/
int WKingPos[120]
    = { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,

        0,   0,   30,  -2,  -12, -12, -5,  30,  0,   0,   0,   0,   -0,  -10,
        -12, -12, -10, 0,   0,   0,   0,   -15, -15, -17, -20, -20, -20, -17,
        -15, 0,   0,   -24, -24, -24, -24, -24, -24, -24, -24, 0,   0,   -24,
        -24, -24, -24, -24, -24, -24, -24, 0,   0,   -44, -44, -44, -44, -44,
        -44, -44, -44, 0,   0,   -54, -54, -54, -54, -54, -54, -54, -54, 0,
        0,   -64, -64, -64, -64, -64, -64, -64, -64, 0,

        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0 };

int BKingPos[120]
    = { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,

        0,   -64, -64, -64, -64, -64, -64, -64, -64, 0,   0,   -54, -54, -54,
        -54, -54, -54, -54, -54, 0,   0,   -44, -44, -44, -44, -44, -44, -44,
        -44, 0,   0,   -24, -24, -24, -24, -24, -24, -24, -24, 0,   0,   -24,
        -24, -24, -24, -24, -24, -24, -24, 0,   0,   -15, -15, -17, -20, -20,
        -20, -17, -15, 0,   0,   0,   -0,  -10, -12, -12, -10, 10,  10,  0,
        0,   0,   30,  0,   -12, -12, 0,   30,  0,   0,

        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0 };

int movePenalty[13] = { 0, 0, -25, 0, 0, -50, -70, 0, 25, 0, 0, 50, 70 };

int BishopTrappedB[120] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 0,  1,  0,  0,  0,  0,  2,  0,  PD, PD, 1,  0,  0,
  0,  0,  0,  0,  2,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD,
  0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,
  0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 0,  0,  0,  0,
  0,  0,  0,  0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD
};

int BishopTrappedW[120] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 0,  0,  0,
  0,  0,  0,  0,  0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD,
  0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,
  0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 1,  0,  0,  0,
  0,  0,  0,  2,  PD, PD, 0,  1,  0,  0,  0,  0,  2,  0,  PD
};

int RookTrappedB[120] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 1,  1,  0,  0,  0,  0,  2,  2,  PD, PD, 1,  0,  0,
  0,  0,  0,  0,  2,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD,
  0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,
  0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 0,  0,  0,  0,
  0,  0,  0,  0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD
};

int RookTrappedW[120]
    = { PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
        PD, PD, PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 0,  0,  0,
        0,  0,  0,  0,  0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD,
        0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,
        0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 1,  0,  0,  0,
        0,  0,  0,  2,  PD, PD, 1,  1,  0,  0,  0,  0,  2,  2,  PD };

int EDGE[120]
    = { PD, PD, PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD, PD, PD, PD, PD, PD,
        PD, PD, PD,  PD,  PD,  PD,  11,  10,  10,  10, 10, 10, 10, 9,  PD,
        PD, 1,  0,   0,   0,   0,   0,   0,   -1,  PD, PD, 1,  0,  0,  0,
        0,  0,  0,   -1,  PD,  PD,  1,   0,   0,   0,  0,  0,  0,  -1, PD,
        PD, 1,  0,   0,   0,   0,   0,   0,   -1,  PD, PD, 1,  0,  0,  0,
        0,  0,  0,   -1,  PD,  PD,  1,   0,   0,   0,  0,  0,  0,  -1, PD,
        PD, -9, -10, -10, -10, -10, -10, -10, -11, PD, PD, PD, PD, PD, PD,
        PD, PD, PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD, PD, PD, PD, PD, PD };

const int SQRANK[128]
    = { PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
        PD, PD, PD, 1,  1,  1,  1,  1,  1,  1,  1,  PD, PD, 2,  2,  2,  2,  2,
        2,  2,  2,  PD, PD, 3,  3,  3,  3,  3,  3,  3,  3,  PD, PD, 4,  4,  4,
        4,  4,  4,  4,  4,  PD, PD, 5,  5,  5,  5,  5,  5,  5,  5,  PD, PD, 6,
        6,  6,  6,  6,  6,  6,  6,  PD, PD, 7,  7,  7,  7,  7,  7,  7,  7,  PD,
        PD, 8,  8,  8,  8,  8,  8,  8,  8,  PD, PD, PD, PD, PD, PD, PD, PD, PD,
        PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD };

const short WPawnValueQS[128] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 10, 10, 10,
  12, 12, 5,  0,  0,  PD, PD, 5,  5,  10, 12, 12, 5,  0,  0,  PD, PD,
  0,  0,  10, 25, 25, 10, 4,  4,  PD, PD, 0,  0,  10, 25, 25, 12, 5,
  5,  PD, PD, 5,  5,  10, 25, 25, 12, 6,  6,  PD, PD, 0,  0,  10, 23,
  23, 12, 5,  5,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD
};

int WPawnValueOK[128] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 0,  0,  5,
  12, 12, 15, 15, 15, PD, PD, 3,  3,  5,  12, 12, 10, 10, 8,  PD, PD,
  10, 11, 10, 25, 25, 10, 0,  0,  PD, PD, 17, 17, 16, 25, 25, 10, 0,
  0,  PD, PD, 23, 23, 20, 27, 24, 10, 2,  2,  PD, PD, 15, 15, 20, 23,
  23, 10, 0,  0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD
};

int WPawnValueQSOK[128] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 0,  0,  10,
  23, 23, 15, 15, 15, PD, PD, 0,  0,  10, 25, 25, 20, 17, 17, PD, PD,
  0,  0,  10, 24, 27, 22, 23, 23, PD, PD, 0,  0,  10, 25, 25, 15, 11,
  10, PD, PD, 5,  5,  10, 12, 12, 5,  3,  3,  PD, PD, 10, 10, 10, 12,
  12, 5,  0,  0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD
};

int WPawnValueEnd[128] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 4,  5,  5,
  5,  5,  5,  5,  4,  PD, PD, 6,  7,  7,  7,  7,  7,  7,  6,  PD, PD,
  12, 13, 13, 13, 13, 13, 13, 12, PD, PD, 18, 18, 18, 18, 18, 18, 18,
  18, PD, PD, 21, 22, 22, 22, 22, 22, 22, 21, PD, PD, 31, 32, 32, 32,
  32, 32, 32, 31, PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD
};

int WPawnIsolated[128] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 4,  7,  9,
  13, 13, 9,  7,  4,  PD, PD, 5,  8,  10, 13, 13, 10, 8,  5,  PD, PD,
  5,  8,  11, 18, 18, 11, 8,  5,  PD, PD, 5,  9,  11, 18, 18, 11, 9,
  5,  PD, PD, 5,  10, 11, 18, 18, 11, 10, 5,  PD, PD, 7,  10, 11, 18,
  18, 11, 10, 7,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD
};

extern short BPawnIsolated[128];
extern short BPawnValue[128];
extern short BPawnValueQS[128];
extern short BPawnValueOK[128];
extern short BPawnValueQSOK[128];
extern short BPawnValueEnd[128];

// opening
int WKingValue[128] = {

  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,
  PD,  PD,  PD,  PD,  PD,  PD,  0,   10,  -2,  -14, -14, -5,  10,  0,   PD,
  PD,  0,   0,   -12, -14, -14, -12, 0,   0,   PD,  PD,  -15, -15, -17, -19,
  -19, -17, -15, -15, PD,  PD,  -30, -35, -35, -35, -35, -35, -35, -30, PD,
  PD,  -40, -40, -40, -40, -40, -40, -40, -40, PD,  PD,  -50, -50, -50, -50,
  -50, -50, -50, -50, PD,  PD,  -65, -68, -68, -68, -68, -68, -68, -65, PD,
  PD,  -68, -68, -68, -68, -68, -68, -68, -68, PD
};

int BKingValue[128] = {

  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,
  PD,  PD,  PD,  PD,  PD,  PD,  -68, -68, -68, -68, -68, -68, -68, -68, PD,
  PD,  -65, -68, -68, -68, -68, -68, -68, -65, PD,  PD,  -50, -50, -50, -50,
  -50, -50, -50, -50, PD,  PD,  -40, -40, -40, -40, -40, -40, -40, -40, PD,
  PD,  -30, -35, -35, -35, -35, -35, -35, -30, PD,  PD,  -15, -15, -17, -19,
  -19, -17, -15, -15, PD,  PD,  0,   0,   -12, -14, -14, -12, 0,   0,   PD,
  PD,  0,   10,  -2,  -14, -14, -5,  10,  0,   PD

};
int WKingValueKRK[128] = {

  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,  PD,
  PD,  PD,  PD,  PD,  PD,  PD,  -45, -38, -30, -28, -28, -30, -38, -45, PD,
  PD,  -38, -28, -20, -18, -18, -20, -28, -38, PD,  PD,  -30, -22, -15, -10,
  -10, -15, -22, -30, PD,  PD,  -28, -18, -10, 00,  00,  -10, -18, -28, PD,
  PD,  -28, -18, -10, 00,  00,  -10, -18, -28, PD,  PD,  -30, -22, -15, -10,
  -10, -15, -22, -30, PD,  PD,  -38, -28, -20, -18, -18, -20, -28, -38, PD,
  PD,  -45, -38, -30, -28, -28, -30, -38, -45, PD
};

int WKingValueKBNK[128] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 5,  12, 22, 32, 42, 52, 62, 72, PD, PD, 12, 5,  10,
  18, 28, 40, 50, 62, PD, PD, 22, 8,  0,  10, 16, 26, 40, 52, PD, PD,
  32, 20, 10, -5, 0,  16, 28, 42, PD, PD, 42, 28, 20, 0,  -5, 10, 18,
  32, PD, PD, 52, 40, 26, 20, 10, 0,  10, 22, PD, PD, 62, 50, 40, 28,
  20, 10, 5,  12, PD, PD, 72, 62, 52, 42, 32, 22, 12, 5,  PD
};

int WKingValueEnd[128] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 0,  2,  5,  5,  5,  5,  2,  0,  PD, PD, 2,  13, 16,
  16, 16, 16, 13, 0,  PD, PD, 2,  16, 33, 33, 33, 33, 16, 2,  PD, PD,
  2,  16, 33, 43, 43, 33, 16, 2,  PD, PD, 2,  16, 33, 43, 43, 33, 16,
  2,  PD, PD, 2,  16, 33, 33, 33, 33, 16, 2,  PD, PD, 2,  13, 16, 16,
  16, 16, 13, 2,  PD, PD, 0,  2,  5,  5,  5,  5,  2,  0,  PD
};

int SQFILE[128]
    = { PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
        PD, PD, PD, PD, 1,  2,  3,  4,  5,  6,  7,  8,  PD, PD, 1,  2,  3,
        4,  5,  6,  7,  8,  PD, PD, 1,  2,  3,  4,  5,  6,  7,  8,  PD, PD,
        1,  2,  3,  4,  5,  6,  7,  8,  PD, PD, 1,  2,  3,  4,  5,  6,  7,
        8,  PD, PD, 1,  2,  3,  4,  5,  6,  7,  8,  PD, PD, 1,  2,  3,  4,
        5,  6,  7,  8,  PD, PD, 1,  2,  3,  4,  5,  6,  7,  8,  PD };

int SQCOLOR[128] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 2,  1,  2,  1,  2,  1,  2,  1,  PD, PD, 1,  2,  1,
  2,  1,  2,  1,  2,  PD, PD, 2,  1,  2,  1,  2,  1,  2,  1,  PD, PD,
  1,  2,  1,  2,  1,  2,  1,  2,  PD, PD, 2,  1,  2,  1,  2,  1,  2,
  1,  PD, PD, 1,  2,  1,  2,  1,  2,  1,  2,  PD, PD, 2,  1,  2,  1,
  2,  1,  2,  1,  PD, PD, 1,  2,  1,  2,  1,  2,  1,  2,  PD
};

int SQDIAG1[128] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 8,  7,  6,  5,  4,  3,  2,  1,  PD, PD, 9,  8,  7,
  6,  5,  4,  3,  2,  PD, PD, 10, 9,  8,  7,  6,  5,  4,  3,  PD, PD,
  11, 10, 9,  8,  7,  6,  5,  4,  PD, PD, 12, 11, 10, 9,  8,  7,  6,
  5,  PD, PD, 13, 12, 11, 10, 9,  8,  7,  6,  PD, PD, 14, 13, 12, 11,
  10, 9,  8,  7,  PD, PD, 15, 14, 13, 12, 11, 10, 9,  8,  PD
};

const int SQDIAG2[128] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 15, 14, 13, 12, 11, 10, 9,  8,  PD, PD, 14, 13, 12,
  11, 10, 9,  8,  7,  PD, PD, 13, 12, 11, 10, 9,  8,  7,  6,  PD, PD,
  12, 11, 10, 9,  8,  7,  6,  5,  PD, PD, 11, 10, 9,  8,  7,  6,  5,
  4,  PD, PD, 10, 9,  8,  7,  6,  5,  4,  3,  PD, PD, 9,  8,  7,  6,
  5,  4,  3,  2,  PD, PD, 8,  7,  6,  5,  4,  3,  2,  1,  PD
};

const short SQ64[128] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 0,  1,  2,  3,  4,  5,  6,  7,  PD, PD, 8,  9,  10,
  11, 12, 13, 14, 15, PD, PD, 16, 17, 18, 19, 20, 21, 22, 23, PD, PD,
  24, 25, 26, 27, 28, 29, 30, 31, PD, PD, 32, 33, 34, 35, 36, 37, 38,
  39, PD, PD, 40, 41, 42, 43, 44, 45, 46, 47, PD, PD, 48, 49, 50, 51,
  52, 53, 54, 55, PD, PD, 56, 57, 58, 59, 60, 61, 62, 63, PD,
};

const short WQUEENSQ[128] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 22, 23, 24, 25, 26, 27, 28, 29, PD, PD, 22, 23, 24,
  25, 26, 27, 28, 29, PD, PD, 22, 23, 24, 25, 26, 27, 28, 29, PD, PD,
  22, 23, 24, 25, 26, 27, 28, 29, PD, PD, 22, 23, 24, 25, 26, 27, 28,
  29, PD, PD, 22, 23, 24, 25, 26, 27, 28, 29, PD, PD, 22, 23, 24, 25,
  26, 27, 28, 29, PD, PD, 22, 23, 24, 25, 26, 27, 28, 29, PD,
};

const short BQUEENSQ[128] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 92, 93, 94, 95, 96, 97, 98, 99, PD, PD, 92, 93, 94,
  95, 96, 97, 98, 99, PD, PD, 92, 93, 94, 95, 96, 97, 98, 99, PD, PD,
  92, 93, 94, 95, 96, 97, 98, 99, PD, PD, 92, 93, 94, 95, 96, 97, 98,
  99, PD, PD, 92, 93, 94, 95, 96, 97, 98, 99, PD, PD, 92, 93, 94, 95,
  96, 97, 98, 99, PD, PD, 92, 93, 94, 95, 96, 97, 98, 99, PD,
};

// ----------------------------------

const int Simetric[120] = {

  110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 100, 101, 102, 103, 104,
  105, 106, 107, 108, 109, 90,  91,  92,  93,  94,  95,  96,  97,  98,  99,
  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  70,  71,  72,  73,  74,
  75,  76,  77,  78,  79,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  40,  41,  42,  43,  44,
  45,  46,  47,  48,  49,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,
  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  10,  11,  12,  13,  14,
  15,  16,  17,  18,  19,  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,
};

const int KingCorner[128] = {

  PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD, PD,
  PD, PD, PD, PD, 1,  1,  0,  0,  0,  0,  2,  2,  PD, PD, 1,  1,  0,
  0,  0,  0,  2,  2,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD,
  0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,
  0,  PD, PD, 0,  0,  0,  0,  0,  0,  0,  0,  PD, PD, 3,  3,  0,  0,
  0,  0,  4,  4,  PD, PD, 3,  3,  0,  0,  0,  0,  4,  4,  PD,
};
