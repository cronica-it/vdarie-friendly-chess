/*
 * Copyright (c) 2024 Viorel Darie. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

int
LiteEvaluation (int move, int ahead, int depth);

int
AplyExcengeLogic (int SqFrom, int SqTo, int PieceMoved, int PieceTaken,
                  int Indicators, int ahead, int depth);

int
StockAttack (int from, int to, int moved, int taken, int indi);

int nbwhite_list = 0;
int nbblack_list = 0;

int list_hi[100];

struct attacks
{
  int taken[20];
  int value[20];
  int moved[20];
  int from[20];
  int to[20];
  int indi[20];
};

struct attacks list_white_attacks;
struct attacks list_black_attacks;

//
// Take another move from the list, if any
//

int
TakeOneMove (int alpha, int beta, int ahead, int depth, int sortedlevel)
{

  int ix;

  int nn;

  int best, n;

  struct StackElement* wstack;
  struct StackElement2* wstack2;
  wstack = (struct StackElement*)&Stack[gameDepth];
  wstack2 = (struct StackElement2*)&Stack2[gameDepth];

  if (wstack2->Attacks[0] == 99)
    return 0;

  best = 0;

  if (sortedlevel == 0)
    {

      best = 0;

      if (gameDepth > treeDepth)
        {

          for (n = treeDepth; n < gameDepth; n++)
            {

              if (BestLine[n - treeDepth] != Stack2[n].move)
                break;
            }
          if (n == gameDepth)
            {
              best = BestLine[n - treeDepth]; // best move from tree
            }

          if (sortbestmove != NO_BEST && best == 0)
            {
              best = sortbestmove;
            }
        }
      if (gameDepth == treeDepth && best == 0 && BestLine[0] > 0
          && BestLine[0] > 65535)
        {
          best = BestLine[0];
        }

      //	if(best == 0) best = Stack2[gameDepth].bestmove; e mai bun
      // BestLine

      Stack2[gameDepth].moveBestLine = best; // best line found for this level
    }

  if ((wstack2->rankOnLevel >= wstack2->movesOnLevel))
    {
      return 0;
    }
  if (noTime == 1)
    return 0;

  // first find bestmove if exists
  if (wstack2->moveBestLine > 0)
    {
      ix = -1;
      nn = wstack2->nbAttacks;
      for (i = 0; i < nn; i++)
        {
          if (wstack2->Attacks[i] == wstack2->moveBestLine)
            {
              ix = i;
              break;
            }
        }
      // if found a move copy it to MoveToDo and suprim it with the last move
      // from list
      if (ix >= 0)
        {

          MoveToDo = wstack2->Attacks[ix];

          wstack2->Attacks[ix] = wstack2->Attacks[nn - 1];
          wstack2->nbAttacks--;
          // wstack2->rankOnLevel++;
          wstack2->moveBestLine = 0; // consumed
          return 1;
        }
    }
  // merge noAttacks with Attacks, ponder moves and sort them

  if (sortedlevel == 0)
    {
      Stack2[gameDepth].sortedlevel = 1;
      // now main white and black hits are set

      int ikk;

      int hi = 0;
      nn = wstack2->nbAttacks;
      for (ikk = 0; ikk < nn; ikk++)
        {
          int mo = wstack2->Attacks[ikk];

          int SqFrom = (mo >> 8) & 255;
          int SqTo = mo & 255;
          hi = 0;
          int PieceMoved = (mo >> 20) & 15;
          int PieceTaken = (mo >> 16) & 15;
          int Indicators = (mo >> 24) & 15;

          int mm = PieceMoved * 120 + SqTo;

          // if(abs(ValPiesa[PieceTaken]) > abs(ValPiesa[PieceMoved])) va = 8;
          int ran = 100;
          if (Indicators > 0)
            {
              if (PieceTaken > 0)
                ran = 121; // transformare cu luare de piesa
              else
                ran = 120; // transformare simpla
            }
          if (PieceTaken > 0)
            {
              int v1 = abs (ValPiesa[PieceTaken]);
              int v2 = abs (ValPiesa[PieceMoved]);
              if (PieceMoved == WK || PieceMoved == BK)
                ran = 122;
              else if (v1 - 400 > v2)
                ran = 124;
              else if (v1 - 200 > v2)
                ran = 123;
              else if (v1 > v2)
                ran = 122;

              else if (PieceMoved == WQ && PieceTaken == BQ)
                ran = 119; // good moves
              else if (PieceMoved == BQ && PieceTaken == WQ)
                ran = 119;

              else if (PieceMoved == WR && PieceTaken == BR)
                ran = 118;
              else if (PieceMoved == BR && PieceTaken == WR)
                ran = 118;

              else if (PieceMoved == WN && PieceTaken == BB)
                ran = 117;
              else if (PieceMoved == BN && PieceTaken == WB)
                ran = 117;

              else if (PieceMoved == WB && PieceTaken == BB)
                ran = 116;
              else if (PieceMoved == BB && PieceTaken == WB)
                ran = 116;

              else if (PieceMoved == WB && PieceTaken == BN)
                ran = 115;
              else if (PieceMoved == BB && PieceTaken == WN)
                ran = 115;

              else if (PieceMoved == WP && PieceTaken == BP)
                ran = 114;
              else if (PieceMoved == BP && PieceTaken == WP)
                ran = 114;
            }
          if (ran == 100)
            {

              if (mo == Stack2[gameDepth].killers[0])
                ran = 110;
              if (mo == Stack2[gameDepth - 2].killers[0] && gameDepth > 2)
                ran = 108;

              else if (mo == Stack2[gameDepth].killers[1])
                ran = 106;
              else if (mo == Stack2[gameDepth - 2].killers[1] && gameDepth > 2)
                ran = 104;

              else if (PieceMoved < 7 && History.whiteMateHistory[mm] != 0)
                ran = 109; // mate seen from this move

              else if (PieceMoved >= 7 && History.blackMateHistory[mm] != 0)
                ran = 109;

              else if (PieceMoved == WP && SqTo > 70)
                ran = 107; // pawn push
              else if (PieceMoved == BP && SqTo < 40)
                ran = 107;
              else if (PieceMoved == WP
                       && Stack[gameDepth].pieceCounts[WP] <= 4)
                ran = 104; // pawn to move
              else if (PieceMoved == BP
                       && Stack[gameDepth].pieceCounts[BP] <= 4)
                ran = 104;
            }

          int hi = 0;

          if (ran == 100)
            {

              if (PieceMoved < 7 && PieceTaken == 0)
                {
                  hi = History.whiteHistory[mm];
                }
              if (PieceMoved >= 7 && PieceTaken == 0)
                {
                  hi = History.blackHistory[mm];
                }
              if (hi > 100)
                {
                  History.blackHistory[mm] = History.blackHistory[mm] / 2;
                  ran = -350000 + hi + PieceMoved - PieceTaken;
                }

              int xran = -200000 - PieceMoved;

              switch (PieceMoved)
                {
                case WK:
                  if (tabla_black_power_attacks[SqFrom] > 0)
                    {
                      xran = -10000;
                    }
                  if (tabla_black_power_attacks[SqTo] > 0)
                    {
                      ran = -800000 - 10000;
                      break;
                    }
                  else
                    ran = xran + 100 + PieceTaken;
                  break;
                case WQ:
                case WR:
                case WB:
                case WN:
                  if (tabla_black_power_attacks[SqFrom] > 0)
                    {
                      xran += 800 * PieceMoved;
                    }
                  if (tabla_black_power_attacks[SqTo] > 0
                      && abs (ValPiesa[PieceMoved])
                             > abs (ValPiesa[PieceTaken]))
                    {
                      ran = -800000 - 800 * PieceMoved + PieceTaken;
                    }
                  else if (-ValPiesa[PieceTaken] > 300)
                    {
                      ran = xran + 50 - PieceMoved + PieceTaken;
                    }
                  else if (PieceTaken > 0)
                    {
                      ran = xran + 20 - PieceMoved + PieceTaken;
                    }

                  break;
                case BK:
                  if (tabla_white_power_attacks[SqFrom] > 0)
                    {
                      xran = -10000;
                    }
                  if (tabla_white_power_attacks[SqTo] > 0)
                    {
                      ran = -800000 - 10000;
                      break;
                    }
                  else
                    ran = xran + 100 + PieceTaken;
                  break;
                case BQ:
                case BR:
                case BB:
                case BN:
                  if (tabla_white_power_attacks[SqFrom] > 0)
                    {
                      xran += 800 * PieceMoved;
                    }
                  if (tabla_white_power_attacks[SqTo] > 0
                      && abs (ValPiesa[PieceMoved])
                             > abs (ValPiesa[PieceTaken]))
                    {
                      ran = -800000 - 800 * PieceMoved + PieceTaken;
                    }
                  else if (ValPiesa[PieceTaken] > 300)
                    {
                      ran = xran + 50 - PieceMoved + PieceTaken;
                    }
                  else if (PieceTaken > 0)
                    {
                      ran = xran + 20 - PieceMoved + PieceTaken;
                    }
                  break;

                } // switch

              if (ran == 100)
                {
                  int lite = 0;

                  lite = LiteEvaluation (mo, ahead, depth);

                  if (lite > 100000)
                    lite = 100000;

                  ran = -420000 + hi / 4 + PieceMoved + lite;

                } // if = 100
            } // if ran == 100

          wstack2->ponders[ikk] = ran;
        } // for each move on level

    } // if first move on level

  int d, start, iVal, tMove;
  start = wstack2->rankOnLevel;
  nn = wstack2->nbAttacks;
  int max = -1000000;
  int j = -1;
  int mo = 0;

  if (start >= nn)
    return 0;

  for (d = start; d < nn; d++)
    {

      iVal = wstack2->ponders[d];
      tMove = wstack2->Attacks[d];
      if (iVal > max)
        {
          max = iVal;
          mo = tMove;
          j = d;
        }
    }
  int mo1 = wstack2->Attacks[start];
  int po1 = wstack2->ponders[start];
  wstack2->Attacks[start] = mo;
  wstack2->ponders[start] = max;
  wstack2->Attacks[j] = mo1;
  wstack2->ponders[j] = po1;

  if (wstack2->rankAttacks < wstack2->nbAttacks)
    {

      ix = wstack2->rankAttacks;

      MoveToDo = wstack2->Attacks[ix];

      if (MoveToDo == 0)
        return 0;
      wstack2->rankAttacks++;
      wstack2->rankOnLevel++;
      return 1;

    } // if wstack2->rankAttacks < wstack2->nbAttacks

  wstack2->rankOnLevel++; // to be sure that no emty list is considered
  if (noTime == 1)
    return 0; // no moves on levels
  return 0;
}

// Lite Evaluation for sorting moves
int
LiteEvaluation (int move, int ahead, int depth)
{

  // return 0; // nu, caci nu mai pot taia pe latime

  int mo = move;

  int SqFrom = (mo >> 8) & 255;
  int SqTo = mo & 255;
  int PieceMoved = (mo >> 20) & 15;
  int PieceTaken = (mo >> 16) & 15;
  int Indicators = (mo >> 24) & 15;

  int eval
      = Stack[gameDepth].ponder + Stack[gameDepth].positionVal; // material
  if (ahead == 1)
    {
      int penalty = 0;
      int xx = 0;
      if (Stack[treeDepth].nPieces < 10)
        xx = 20;
      if (Indicators == 0 && PieceMoved == WP)
        penalty += xx;
      else if (Indicators == 0 && PieceMoved == BP)
        penalty += -xx;
      if (lastOptimalMove == mo && PieceMoved < BP)
        penalty += 20;
      if (lastOptimalMove == mo && PieceMoved >= BP)
        penalty -= 20;
      eval += penalty;
    }
  // king do not move at beginning
  if (treeDepth < 30 && (int)PieceMoved == WK)
    eval -= 100;
  if (treeDepth < 30 && (int)PieceMoved == BK)
    eval -= 100;

  // penalties for queens early moves
  if (treeDepth < 20 && PieceMoved == WQ)
    eval -= 500;
  if (treeDepth < 20 && PieceMoved == BQ)
    eval -= 500;

  // stimul for pawn moves in midle game
  if (treeDepth > 50 && PieceMoved == WP)
    eval += 100;
  if (treeDepth > 50 && PieceMoved == BP)
    eval += 100;

  if (PieceTaken > 0)
    return eval;

  int nPieces = Stack[gameDepth].nPieces;

  // Penalties for queen at A4
  if (PieceMoved == WQ && SqTo == 51)
    eval -= 800;

  // Knigths moves evaluation
  int tab1[13] = { 0, 1, 5, 3, 3, 5, 0, 1, 5, 3, 3, 5, 20 };
  int tab2[13] = { 0, 1, 5, 3, 3, 5, 20, 1, 5, 3, 3, 5, 0 };

  // White Knights

  if (PieceMoved == WN)
    {
      k1 = SqFrom;
      if (tabla[k1 - 21] >= 0)
        eval -= tab1[tabla[k1 - 21]];
      if (tabla[k1 - 19] >= 0)
        eval -= tab1[tabla[k1 - 19]];
      if (tabla[k1 - 12] >= 0)
        eval -= tab1[tabla[k1 - 12]];
      if (tabla[k1 - 8] >= 0)
        eval -= tab1[tabla[k1 - 8]];

      if (tabla[k1 + 8] >= 0)
        eval -= tab1[tabla[k1 + 8]];
      if (tabla[k1 + 12] >= 0)
        eval -= tab1[tabla[k1 + 12]];
      if (tabla[k1 + 19] >= 0)
        eval -= tab1[tabla[k1 + 19]];
      if (tabla[k1 + 21] >= 0)
        eval -= tab1[tabla[k1 + 21]];

      k1 = SqTo;
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
    } // if

  // Black Knights

  if (PieceMoved == BN)
    {
      k1 = SqFrom;
      if (tabla[k1 - 21] >= 0)
        eval += tab2[tabla[k1 - 21]];
      if (tabla[k1 - 19] >= 0)
        eval += tab2[tabla[k1 - 19]];
      if (tabla[k1 - 12] >= 0)
        eval += tab2[tabla[k1 - 12]];
      if (tabla[k1 - 8] >= 0)
        eval += tab2[tabla[k1 - 8]];

      if (tabla[k1 + 8] >= 0)
        eval += tab2[tabla[k1 + 8]];
      if (tabla[k1 + 12] >= 0)
        eval += tab2[tabla[k1 + 12]];
      if (tabla[k1 + 19] >= 0)
        eval += tab2[tabla[k1 + 19]];
      if (tabla[k1 + 21] >= 0)
        eval += tab2[tabla[k1 + 21]];

      k1 = SqTo;
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
    } // if

  if (nPieces >= 16)
    {
      switch (PieceMoved)
        {
        case WP:
          eval += WPawnPos[SqTo] - WPawnPos[SqFrom];
          break;
        case BP:
          eval += BPawnPos[SqTo] - BPawnPos[SqFrom];
          break;

        case WR:
          eval += WRookValue[SqTo] - WRookValue[SqFrom];
          break;
        case BR:
          eval += WRookValue[SqTo] - WRookValue[SqFrom];
          break;

        case WN:
          eval += WKnightPost[SqTo] - WKnightPost[SqFrom];
          break;
        case BN:
          eval += BKnightPost[SqTo] - BKnightPost[SqFrom];
          break;

        case WB:
          eval += WBishopValue[SqTo] - WBishopValue[SqFrom];
          break;
        case BB:
          eval += WBishopValue[SqTo] - WBishopValue[SqFrom];
          break;

        case WQ:
          eval += WQueenValue[SqTo] - WQueenValue[SqFrom];
          break;
        case BQ:
          eval += BQueenValue[SqTo] - BQueenValue[SqFrom];
          break;
        }
    }
  else
    {
      int bonus;
      int r1;
      int r2;
      int pi1 = 0;
      int pi2 = 0;
      int q1;
      int q2;

      // trofism to advers king
      switch (PieceMoved)
        {
        case WP:
          eval += (SQRANK[SqTo] - SQRANK[SqFrom]) * 50;
          break;
        case BP:
          eval -= (SQRANK[SqTo] - SQRANK[SqFrom]) * 50;
          break;
        case WN:
          eval += (DistKing[SqFrom][(int)Stack[gameDepth].blackKingSquare]
                   - DistKing[SqTo][(int)Stack[gameDepth].blackKingSquare])
                  * 10;

          break;
        case BN:
          eval += (DistKing[SqFrom][(int)Stack[gameDepth].whiteKingSquare]
                   - DistKing[SqTo][(int)Stack[gameDepth].whiteKingSquare])
                  * 10;
          break;

        case WB:
          eval += (DistKing[SqFrom][(int)Stack[gameDepth].blackKingSquare]
                   - DistKing[SqTo][(int)Stack[gameDepth].blackKingSquare])
                  * 10;

          break;
        case BB:
          eval += (DistKing[SqFrom][(int)Stack[gameDepth].whiteKingSquare]
                   - DistKing[SqFrom][(int)Stack[gameDepth].whiteKingSquare])
                  * 10;

          break;
        case WR:
          eval += (DistKing[SqFrom][(int)Stack[gameDepth].blackKingSquare]
                   - DistKing[SqTo][(int)Stack[gameDepth].blackKingSquare])
                  * 10;

          // eval += LineWhite[SQRANK[SqTo] - 1] + ColumnWhite[SqTo % 10 - 1] -
          //	LineWhite[SQRANK[SqFrom] - 1] - ColumnWhite[SqFrom % 10 - 1];

          // eval += LineBlack[SQRANK[SqTo] - 1] + ColumnBlack[SqTo % 10 - 1] -
          //	LineBlack[SQRANK[SqFrom] - 1] - ColumnBlack[SqFrom % 10 - 1];

          // rooks double on column
          r1 = (int)Stack[gameDepth].pieceAPosition[WR][0];
          if (r1 == SqFrom)
            r1 = SqTo;
          r2 = (int)Stack[gameDepth].pieceAPosition[WR][1];
          if (r2 == SqFrom)
            r2 = SqTo;
          bonus = 0;

          pi1 = Stack[gameDepth].pawnWOnFiles[r1 % 10];
          pi2 = Stack[gameDepth].pawnBOnFiles[r1 % 10];

          bonus = 0;
          if (r1 != 0 && ((r1 % 10) == (r2 % 10)) && (pi1 + pi2) <= 1)
            {
              bonus = 20
                      - (pi1 + pi2) * 10; // bonus for rooks on the same column
            }
          else if (r1 > 0 && (pi1 + pi2) == 1)
            bonus = 10; // bonus for rooks protecting/ attacking pawns
          eval += bonus;

          pi1 = Stack[gameDepth].pawnWOnFiles[r2 % 10];
          pi2 = Stack[gameDepth].pawnBOnFiles[r2 % 10];

          bonus = 0;
          if (r2 > 0 && (pi1 + pi2) == 1)
            bonus += 10; // bonus for rooks protecting/ attacking pawns
          eval += bonus;

          break;

        case BR:
          eval += (DistKing[SqFrom][(int)Stack[gameDepth].whiteKingSquare]
                   - DistKing[SqTo][(int)Stack[gameDepth].whiteKingSquare])
                  * 10;

          // eval += LineWhite[SQRANK[SqTo] - 1] + ColumnWhite[SqTo % 10 - 1] -
          //	LineWhite[SQRANK[SqFrom] - 1] - ColumnWhite[SqFrom % 10 - 1];

          // eval += LineBlack[SQRANK[SqTo] - 1] + ColumnBlack[SqTo % 10 - 1] -
          //	LineBlack[SQRANK[SqFrom] - 1] - ColumnBlack[SqFrom % 10 - 1];

          r1 = (int)Stack[gameDepth].pieceAPosition[BR][0];
          if (r1 == SqFrom)
            r1 = SqTo;
          r2 = (int)Stack[gameDepth].pieceAPosition[BR][1];
          if (r2 == SqFrom)
            r2 = SqTo;

          bonus = 0;
          r1 = (int)Stack[gameDepth].pieceAPosition[BR][0];
          r2 = (int)Stack[gameDepth].pieceAPosition[BR][1];

          bonus = 0;
          if (r1 != 0 && ((r1 % 10) == (r2 % 10)) && (pi1 + pi2) <= 1)
            {
              bonus = 20
                      - (pi1 + pi2) * 10; // bonus for rooks on the same column
            }
          else if (r1 > 0 && (pi1 + pi2) == 1)
            bonus = 10; // bonus for rooks protecting/ attacking pawns

          if (r2 > 0 && (pi1 + pi2) == 1)
            bonus += 10; // bonus for rooks protecting/ attacking pawns

          eval -= bonus;
          break;

        case WQ:
          eval += (DistKing[SqFrom][(int)Stack[gameDepth].blackKingSquare]
                   - DistKing[SqTo][(int)Stack[gameDepth].blackKingSquare])
                  * 20;
          // queen activity
          q1 = (int)Stack[gameDepth].pieceAPosition[WQ][0];
          if (q1 == SqFrom)
            q1 = SqTo;
          bonus = 0;

          pi1 = Stack[gameDepth].pawnWOnFiles[q1 % 10];
          pi2 = Stack[gameDepth].pawnBOnFiles[q1 % 10];

          bonus = 0;
          if (q1 > 0 && (pi1 + pi2) == 1)
            bonus = 10; // bonus for queens protecting/ attacking pawns

          eval += bonus;

          break;
        case BQ:
          eval += (DistKing[SqFrom][(int)Stack[gameDepth].whiteKingSquare]
                   - DistKing[SqTo][(int)Stack[gameDepth].whiteKingSquare])
                  * 20;
          // queen activity
          q2 = (int)Stack[gameDepth].pieceAPosition[BQ][0];
          if (q2 == SqFrom)
            q2 = SqTo;
          bonus = 0;

          pi1 = Stack[gameDepth].pawnWOnFiles[q2 % 10];
          pi2 = Stack[gameDepth].pawnBOnFiles[q2 % 10];

          bonus = 0;
          if (q2 > 0 && (pi1 + pi2) == 1)
            bonus -= 10; // bonus for queens protecting/ attacking pawns

          eval += bonus;

          break;

        case WK:
          eval += (KingCenter[SqTo] - KingCenter[SqFrom]) * 10;
          break;
        case BK:
          eval += (KingCenter[SqTo] - KingCenter[SqFrom]) * 10;
          break;
        }
    }
  // apply corection like excenge logic
  return eval;

  if (depth > 0)
    {
      int evalxx = AplyExcengeLogic (SqFrom, SqTo, PieceMoved, PieceTaken,
                                     Indicators, ahead, depth);
      eval += evalxx;
    }

  return eval;

} // end subroutine

int
EvaluateAttacks (int SqFrom, int SqTo, int PieceMoved, int PieceTaken,
                 int ahead, int depth)
{
  int it, k1;

  int pondere_piesa[13] = { 0, 10, 5, 6, 3, 10, 2, 10, 5, 6, 3, 10, 2 };

  int nPieces = Stack[gameDepth].nPieces;

  nbwhite_list = 0;
  nbblack_list = 0;

  if (nPieces <= 16)
    {
      for (i = 0; i < 8; i++)
        {
          LineWhite[i] = 0;
          ColumnWhite[i] = 0;
          LineBlack[i] = 0;
          ColumnBlack[i] = 0;
        }
    }
  int PiecePonders[13] = { 0, 5, 8, 7, 7, 10, 50, 5, 8, 7, 7, 10, 50 };

  for (it = 21; it < 99; it++)
    {
      int pie = tabla[it];
      if (pie > 0 && nPieces <= 16)
        {
          int lin = SQRANK[it];

          if (pie <= 6)
            {
              LineWhite[lin] += PiecePonders[pie];
              ColumnWhite[lin] += PiecePonders[pie];
            }
          else
            {
              LineBlack[lin] += PiecePonders[pie];
              ColumnBlack[lin] += PiecePonders[pie];
            }
        }
      tabla_white_power_attacks[it] = 0;
      tabla_black_power_attacks[it] = 0;
    }
  if (Stack[gameDepth].pieceCounts[WP] > 0)
    {

      for (it = 0; it < Stack[gameDepth].pieceCounts[WP]; it++)
        {
          int loc = Stack[gameDepth].pieceAPosition[WP][it];
          if (tabla[loc + 9] >= 0)
            tabla_white_power_attacks[loc + 9] += pondere_piesa[WP];
          if (tabla[loc + 11] >= 0)
            tabla_white_power_attacks[loc + 11] += pondere_piesa[WP];
        }
    }
  if (Stack[gameDepth].pieceCounts[BP] > 0)
    {
      for (it = 0; it < Stack[gameDepth].pieceCounts[BP]; it++)
        {
          int loc = Stack[gameDepth].pieceAPosition[BP][it];
          if (tabla[loc - 9] >= 0)
            tabla_black_power_attacks[loc - 9] += pondere_piesa[BP];
          if (tabla[loc - 11] >= 0)
            tabla_black_power_attacks[loc - 11] += pondere_piesa[BP];
        }
    }

  // White King

  k1 = Stack[gameDepth].pieceAPosition[WK][0];

  if (tabla[k1 - 11] >= 0)
    tabla_white_power_attacks[k1 - 11] += pondere_piesa[WK];
  if (tabla[k1 - 10] >= 0)
    tabla_white_power_attacks[k1 - 10] += pondere_piesa[WK];
  if (tabla[k1 - 9] >= 0)
    tabla_white_power_attacks[k1 - 9] += pondere_piesa[WK];
  if (tabla[k1 - 1] >= 0)
    tabla_white_power_attacks[k1 - 1] += pondere_piesa[WK];

  if (tabla[k1 + 1] >= 0)
    tabla_white_power_attacks[k1 + 1] += pondere_piesa[WK];
  if (tabla[k1 + 9] >= 0)
    tabla_white_power_attacks[k1 + 9] += pondere_piesa[WK];
  if (tabla[k1 + 10] >= 0)
    tabla_white_power_attacks[k1 + 10] += pondere_piesa[WK];
  if (tabla[k1 + 11] >= 0)
    tabla_white_power_attacks[k1 + 11] += pondere_piesa[WK];

  // White King

  k1 = Stack[gameDepth].pieceAPosition[BK][0];

  if (tabla[k1 - 11] >= 0)
    tabla_black_power_attacks[k1 - 11] += pondere_piesa[WK];
  if (tabla[k1 - 10] >= 0)
    tabla_black_power_attacks[k1 - 10] += pondere_piesa[WK];
  if (tabla[k1 - 9] >= 0)
    tabla_black_power_attacks[k1 - 9] += pondere_piesa[WK];
  if (tabla[k1 - 1] >= 0)
    tabla_black_power_attacks[k1 - 1] += pondere_piesa[WK];

  if (tabla[k1 + 1] >= 0)
    tabla_black_power_attacks[k1 + 1] += pondere_piesa[WK];
  if (tabla[k1 + 9] >= 0)
    tabla_black_power_attacks[k1 + 9] += pondere_piesa[WK];
  if (tabla[k1 + 10] >= 0)
    tabla_black_power_attacks[k1 + 10] += pondere_piesa[WK];
  if (tabla[k1 + 11] >= 0)
    tabla_black_power_attacks[k1 + 11] += pondere_piesa[WK];

  // White Knights
  if (Stack[gameDepth].pieceCounts[WN] > 0)
    {
      for (it = 0; it < Stack[gameDepth].pieceCounts[WN]; it++)
        {

          k1 = Stack[gameDepth].pieceAPosition[WN][it];

          if (tabla[k1 - 21] >= 0)
            tabla_white_power_attacks[k1 - 21] += pondere_piesa[WN];
          if (tabla[k1 - 19] >= 0)
            tabla_white_power_attacks[k1 - 19] += pondere_piesa[WN];
          if (tabla[k1 - 12] >= 0)
            tabla_white_power_attacks[k1 - 12] += pondere_piesa[WN];
          if (tabla[k1 - 8] >= 0)
            tabla_white_power_attacks[k1 - 8] += pondere_piesa[WN];

          if (tabla[k1 + 8] >= 0)
            tabla_white_power_attacks[k1 + 8] += pondere_piesa[WN];
          if (tabla[k1 + 12] >= 0)
            tabla_white_power_attacks[k1 + 12] += pondere_piesa[WN];
          if (tabla[k1 + 19] >= 0)
            tabla_white_power_attacks[k1 + 19] += pondere_piesa[WN];
          if (tabla[k1 + 21] >= 0)
            tabla_white_power_attacks[k1 + 21] += pondere_piesa[WN];
        }
    } // if
  // White Knight

  if (Stack[gameDepth].pieceCounts[BN] > 0)
    {
      for (it = 0; it < Stack[gameDepth].pieceCounts[BN]; it++)
        {

          k1 = Stack[gameDepth].pieceAPosition[BN][it];

          if (tabla[k1 - 21] >= 0)
            tabla_black_power_attacks[k1 - 21] += pondere_piesa[WN];
          if (tabla[k1 - 19] >= 0)
            tabla_black_power_attacks[k1 - 19] += pondere_piesa[WN];
          if (tabla[k1 - 12] >= 0)
            tabla_black_power_attacks[k1 - 12] += pondere_piesa[WN];
          if (tabla[k1 - 8] >= 0)
            tabla_black_power_attacks[k1 - 8] += pondere_piesa[WN];

          if (tabla[k1 + 8] >= 0)
            tabla_black_power_attacks[k1 + 8] += pondere_piesa[WN];
          if (tabla[k1 + 12] >= 0)
            tabla_black_power_attacks[k1 + 12] += pondere_piesa[WN];
          if (tabla[k1 + 19] >= 0)
            tabla_black_power_attacks[k1 + 19] += pondere_piesa[WN];
          if (tabla[k1 + 21] >= 0)
            tabla_black_power_attacks[k1 + 21] += pondere_piesa[WN];
        }
    } // if
  int* p3;

  // White Queen
  if (Stack[gameDepth].pieceCounts[WQ] > 0)
    {
      for (it = 0; it < Stack[gameDepth].pieceCounts[WQ]; it++)
        {

          k1 = Stack[gameDepth].pieceAPosition[WQ][it];

          p3 = (int*)&dk[0];

          for (; *p3 != 0; p3++)
            {
              loc2 = k1;
              for (;;)
                {
                  loc2 = loc2 + *p3;

                  if ((int)tabla[loc2] < 0)
                    break; // e margine

                  tabla_white_power_attacks[loc2] += pondere_piesa[WQ];

                  if ((int)tabla[loc2] > 0)
                    break; // e piesa
                } // end a direction
            } // end for directions for a piece
        }
    }
  // Black Queen
  if (Stack[gameDepth].pieceCounts[BQ] > 0)
    {
      for (it = 0; it < Stack[gameDepth].pieceCounts[BQ]; it++)
        {

          k1 = Stack[gameDepth].pieceAPosition[BQ][it];

          p3 = (int*)&dk[0];

          for (; *p3 != 0; p3++)
            {
              loc2 = k1;
              for (;;)
                {
                  loc2 = loc2 + *p3;

                  if ((int)tabla[loc2] < 0)
                    break; // e margine

                  tabla_black_power_attacks[loc2] += pondere_piesa[BQ];

                  if ((int)tabla[loc2] > 0)
                    break; // e piesa
                } // end a direction
            } // end for directions for a piece
        }
    }
  // White Rook
  if (Stack[gameDepth].pieceCounts[WR] > 0)
    {
      for (it = 0; it < Stack[gameDepth].pieceCounts[WR]; it++)
        {

          k1 = Stack[gameDepth].pieceAPosition[WR][it];

          p3 = (int*)&dr[0];

          for (; *p3 != 0; p3++)
            {
              loc2 = k1;
              for (;;)
                {
                  loc2 = loc2 + *p3;

                  if ((int)tabla[loc2] < 0)
                    break; // e margine

                  tabla_white_power_attacks[loc2] += pondere_piesa[WR];

                  if ((int)tabla[loc2] > 0)
                    break; // e o piesa
                } // end a direction
            } // end for directions for a piece
        }
    }
  // Black Rook
  if (Stack[gameDepth].pieceCounts[BR] > 0)
    {
      for (it = 0; it < Stack[gameDepth].pieceCounts[BR]; it++)
        {

          k1 = Stack[gameDepth].pieceAPosition[BR][it];

          p3 = (int*)&dr[0];

          for (; *p3 != 0; p3++)
            {
              loc2 = k1;
              for (;;)
                {
                  loc2 = loc2 + *p3;

                  if ((int)tabla[loc2] < 0)
                    break; // e margine

                  tabla_black_power_attacks[loc2] += pondere_piesa[BR];

                  if ((int)tabla[loc2] > 0)
                    break; // e piesa
                } // end a direction
            } // end for directions for a piece
        }
    }

  // White Bishop
  if (Stack[gameDepth].pieceCounts[WB] > 0)
    {
      for (it = 0; it < Stack[gameDepth].pieceCounts[WB]; it++)
        {

          k1 = Stack[gameDepth].pieceAPosition[WB][it];

          p3 = (int*)&db[0];

          for (; *p3 != 0; p3++)
            {
              loc2 = k1;
              for (;;)
                {
                  loc2 = loc2 + *p3;

                  if ((int)tabla[loc2] < 0)
                    break; // e margine

                  tabla_white_power_attacks[loc2] += pondere_piesa[WB];

                  if ((int)tabla[loc2] > 0)
                    break; // e piesa
                } // end a direction
            } // end for directions for a piece
        }
    }
  // Black Bishop
  if (Stack[gameDepth].pieceCounts[BB] > 0)
    {
      for (it = 0; it < Stack[gameDepth].pieceCounts[BB]; it++)
        {

          k1 = Stack[gameDepth].pieceAPosition[BB][it];

          p3 = (int*)&db[0];

          for (; *p3 != 0; p3++)
            {
              loc2 = k1;
              for (;;)
                {
                  loc2 = loc2 + *p3;

                  if ((int)tabla[loc2] < 0)
                    break; // e margine

                  tabla_black_power_attacks[loc2] += pondere_piesa[BB];

                  if ((int)tabla[loc2] > 0)
                    break; // e piesa
                } // end a direction
            } // end for directions for a piece
        }
    }

  return 0;

  // x_centru = suma_xp / suma_po;
  // y_centru = suma_yp / suma_po;

  // sorting white and black attacks using insertion algorithm
  int nn = nbwhite_list; // nb attacks

  int d, start, tvalue, tfrom, tto, tmoved, ttaken, tindi;
  start = 0;
  for (d = start + 1; d < nn; d++)
    {
      if (nn <= 1)
        break;

      i = d;
      tvalue = list_white_attacks.value[i];
      tfrom = list_white_attacks.from[i];
      tto = list_white_attacks.to[i];
      tmoved = list_white_attacks.moved[i];
      ttaken = list_white_attacks.taken[i];
      tindi = list_white_attacks.indi[i];

      while (i > start && tvalue > list_white_attacks.value[i - 1])
        {
          list_white_attacks.value[i] = list_white_attacks.value[i - 1];
          list_white_attacks.from[i] = list_white_attacks.from[i - 1];
          list_white_attacks.to[i] = list_white_attacks.to[i - 1];
          list_white_attacks.moved[i] = list_white_attacks.moved[i - 1];
          list_white_attacks.taken[i] = list_white_attacks.taken[i - 1];
          list_white_attacks.indi[i] = list_white_attacks.indi[i - 1];

          i--;
        }
      list_white_attacks.value[i] = tvalue;
      list_white_attacks.from[i] = tfrom;
      list_white_attacks.to[i] = tto;
      list_white_attacks.moved[i] = tmoved;
      list_white_attacks.taken[i] = ttaken;
      list_white_attacks.indi[i] = tindi;
    }
  nn = nbblack_list; // nb attacks

  start = 0;
  for (d = start + 1; d < nn; d++)
    {
      if (nn <= 1)
        break;

      i = d;
      tvalue = list_black_attacks.value[i];
      tfrom = list_black_attacks.from[i];
      tto = list_black_attacks.to[i];
      tmoved = list_black_attacks.moved[i];
      ttaken = list_black_attacks.taken[i];
      tindi = list_black_attacks.indi[i];

      while (i > start && tvalue > list_black_attacks.value[i - 1])
        {
          list_black_attacks.value[i] = list_black_attacks.value[i - 1];
          list_black_attacks.from[i] = list_black_attacks.from[i - 1];
          list_black_attacks.to[i] = list_black_attacks.to[i - 1];
          list_black_attacks.moved[i] = list_black_attacks.moved[i - 1];
          list_black_attacks.taken[i] = list_black_attacks.taken[i - 1];
          list_black_attacks.indi[i] = list_black_attacks.indi[i - 1];
          i--;
        }
      list_black_attacks.value[i] = tvalue;
      list_black_attacks.from[i] = tfrom;
      list_black_attacks.to[i] = tto;
      list_black_attacks.moved[i] = tmoved;
      list_black_attacks.taken[i] = ttaken;
      list_black_attacks.indi[i] = tindi;
    }

  return 0;
}
